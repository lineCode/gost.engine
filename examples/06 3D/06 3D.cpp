#include <gost.h>

#ifdef _DEBUG
#pragma comment(lib, "gost_d.lib")
#else 
#pragma comment(lib, "gost.lib")
#endif

using namespace gost;


#if defined( GT_PLATFORM_WIN32 )
#include <Windows.h>
int WINAPI WinMain( HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nCmdShow*/ ){
#endif

	gtDeviceCreationParameters params;
	gtPtr_t(gtMainSystem,mainSystem,InitializeGoSTEngine(params));
	gtPtr_t(gtWindow,window,mainSystem->createSystemWindow( gtWindowInfo() ));


	gtDriverInfo di;
	di.m_outWindow = window.data();
	di.m_vSync = true;
	gtPtr_t(gtDriver,driver,mainSystem->createVideoDriver( di, GT_UID_RENDER_D3D11 ));

	gtSceneSystem * scene = mainSystem->getSceneSystem();

	gtCamera * camera = scene->addCamera( v3f_t(5.f,5.f,5.f) );

	gtStaticObject * room = scene->addStaticObject( driver->getModel(u"../media/room.obj") );
	room->getModel()->getMaterial(0)->textureLayer[0].texture = driver->getTexture(u"../media/room.png");
	
	

	gtEvent event;

	f32 delta = 0.f;
	f32 pos = 1000.f;
	u32 time = mainSystem->getTime();

	f32 angle = 0.f;

	while( mainSystem->update() ){


		f32 sn = std::sinf( angle )/180.f*PI;
		f32 cs = std::cosf( angle )/180.f*PI;
		angle += 10.f * delta; if( angle > 360.f ) angle = 0.f;

		camera->setPosition( v3f_t( pos*sn, 5.f, pos*cs ) );

		u32 now = mainSystem->getTime();

		while( mainSystem->pollEvent( event ) ){
			switch( event.type ){
				case gtEventType::keyboard:
				if( event.keyboardEvent.isReleased( gtKey::K_ESCAPE ) ){
					mainSystem->shutdown(); /// exit when key released
				}
				break;
			}
		}

		if( mainSystem->isRun() ){

			/// Camera zoom
			if( mainSystem->isKeyPressed( gtKey::K_X ) ) camera->setFOV( camera->getFOV() + 10.f * delta );
			if( mainSystem->isKeyPressed( gtKey::K_Z ) ) camera->setFOV( camera->getFOV() - 10.f * delta );


			driver->beginRender( true, gtColor( 0.2f, 0.2f, 0.2f, 1.f ) ); /// RGBA.

			scene->renderScene(); /// Draw all

			driver->endRender();

			delta = (f32)(now - time)*0.0001f;
					
			time = now;
		}

	}

	return 0;
}