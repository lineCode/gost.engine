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
	
	gtWindowInfo wi;
	gtPtr_t(gtWindow,window,mainSystem->createSystemWindow( wi ));


	gtDriverInfo di;
	di.m_outWindow = window.data();
	di.m_vSync = true;
	gtPtr_t(gtDriver,driver,mainSystem->createVideoDriver( di, GT_UID_RENDER_D3D11 ));

	gtSceneSystem * scene = mainSystem->getSceneSystem( driver.data() );

	gtCamera * camera = scene->addCamera( v3f(0.f,0.f,0.f) );
	camera->setCameraType( gtCameraType::CT_FPS );
	//camera->setFar( 30.f );
	camera->setAspect( 1.f );

	gtStaticObject * room = scene->addStaticObject( driver->getModel(u"../media/m9.obj") );
	room->showAabb( true );
	room->getModel()->getMaterial(0)->textureLayer[0].texture = driver->getTexture(u"../media/Tex_0009_1.png");
	
	gtModel * cube = mainSystem->getModelSystem()->createCube(0.125f);
	gtRenderModel * rcube = driver->createModel( cube );

	f32 x = 0.f, y = 0.f;
	for( int i = 0; i < 100; ++i ){
		scene->addStaticObject( rcube, v3f( x, 0.f, y ) );
		x += 1.f;
		if( x > 10.f ){
			x = 0.f;
			y += 1.f;
		}
	}

	gtEvent event;

	f32 delta = 0.f;
	f32 pos = 1000.f;
	u32 time = mainSystem->getTime();

	f32 angle = 0.f;

	while( mainSystem->update() ){


		f32 sn = std::sinf( angle )/180.f*PI;
		f32 cs = std::cosf( angle )/180.f*PI;
		angle += 10.f * delta; if( angle > 360.f ) angle = 0.f;

		room->setRotation( v3f( 0.f, angle, 0.f ) );

		//camera->setPosition( v3f_t( pos*sn, 5.f, pos*cs ) );

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
			if( mainSystem->isKeyPressed( gtKey::K_Q ) ) camera->setRotation( camera->getRotation() - v3f( 0.f, 0.01f, 0.f ) );
			if( mainSystem->isKeyPressed( gtKey::K_E ) ) camera->setRotation( camera->getRotation() + v3f( 0.f, 0.01f, 0.f ) );
			if( mainSystem->isKeyPressed( gtKey::K_W ) ) camera->setRotation( camera->getRotation() - v3f( 0.01f, 0.0f, 0.f ) );
			if( mainSystem->isKeyPressed( gtKey::K_S ) ) camera->setRotation( camera->getRotation() + v3f( 0.01f, 0.0f, 0.f ) );
			if( mainSystem->isKeyPressed( gtKey::K_A ) ) camera->setRotation( camera->getRotation() - v3f( 0.0f, 0.0f, 0.01f ) );
			if( mainSystem->isKeyPressed( gtKey::K_D ) ) camera->setRotation( camera->getRotation() + v3f( 0.0f, 0.0f, 0.01f ) );
			
			if( mainSystem->isKeyPressed( gtKey::K_UP ) ) camera->setPosition( camera->getPosition() + v3f( 0.0f, 0.0f, 0.1f ) );
			if( mainSystem->isKeyPressed( gtKey::K_DOWN ) ) camera->setPosition( camera->getPosition() - v3f( 0.0f, 0.0f, 0.1f ) );
			if( mainSystem->isKeyPressed( gtKey::K_LEFT ) ) camera->setPosition( camera->getPosition() - v3f( 0.1f, 0.0f, 0.0f ) );
			if( mainSystem->isKeyPressed( gtKey::K_RIGHT ) ) camera->setPosition( camera->getPosition() + v3f( 0.1f, 0.0f, 0.0f ) );


			driver->beginRender( true, gtColor( 0.2f, 0.2f, 0.2f, 1.f ) ); /// RGBA.


			scene->renderScene(); /// Draw all

			driver->endRender();

			delta = (f32)(now - time)*0.0001f;
					
			time = now;
		}

	}

	return 0;
}