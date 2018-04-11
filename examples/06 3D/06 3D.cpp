﻿#include <gost.h>

using namespace gost;


#if defined( GT_PLATFORM_WIN32 )
int WINAPI WinMain( HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nCmdShow*/ ){
#endif


	auto mainSystem = InitializeGoSTEngine();
	
	gtWindowInfo wi;
	auto window = mainSystem->createSystemWindow( wi );


	gtDriverInfo di;
	di.m_outWindow = window.data();

	auto driver = mainSystem->createVideoDriver( di, GT_UID_RENDER_D3D11 );

	gtSceneSystem * scene = mainSystem->getSceneSystem( driver.data() );

	gtStaticObject * room = scene->addStaticObject( driver->getModel(u"../media/room.obj") );
//	room->showBV( true );
	room->getModel()->getMaterial(0)->textureLayer[0].texture = driver->getTexture(u"../media/room.png");

	gtCamera * camera = scene->addCamera( v3f(0.f,1.f,0.f) );
	camera->setCameraType( gtCameraType::CT_FPS );
	//camera->setFar( 30.f );
	camera->setAspect( 1.f );
	camera->setFar( 250.f );

	auto cube = mainSystem->getModelSystem()->createCube(0.125f);
	auto rcube = driver->createModel( cube.data() );

	f32 x = 0.f, y = 0.f;
	gtStaticObject * cubs[1000];
	for( int i = 0; i < 1000; ++i ){
		cubs[ i ] = scene->addStaticObject( rcube.data(), v3f( x, 0.f, y ) );
		cubs[ i ]->showBV( true );
		x += 1.f;
		if( x > 100.f ){
			x = 0.f;
			y += 1.f;
		}
	}

	gtEvent event;

	f32 delta = 0.f;
	f32 pos = 1000.f;
	u32 time = mainSystem->getTime();
	 
	f32 angle = 0.f;

	gtGUISystem * guiSystem = mainSystem->getGUISystem( driver.data() );
	auto font = guiSystem->createFont( u"../media/fonts/NotoSans.xml" ); 
	auto text = guiSystem->createStaticText( u"", 5, 25 );
	text->setFont( font );

	auto text2 = guiSystem->createStaticText( u"Noto Sans Русский текст. きゃっかんてき<>",
5, 50 );
	text2->setFont( font );
	
	gtString s;
	u32 i = 0;

	f32 fpstime = 0.f;
	u32 fps = 0u;
	u32 fps_counter = 0u;

	while( mainSystem->update() ){

		text->clear();

		s = u"FPS: ";
		s += fps;

		text->setText( s );

		f32 sn = std::sinf( angle )/180.f*PI;
		f32 cs = std::cosf( angle )/180.f*PI;
		angle += 10.f * delta; if( angle > 360.f ) angle = 0.f;

		for( int i = 0; i < 100; ++i ){
			cubs[ i ]->setRotation( v3f( angle - 0.01f, angle, angle + 0.01f ) );
		}

		//camera->setPosition( v3f_t( pos*sn, 5.f, pos*cs ) );

		u32 now = mainSystem->getTime();

		while( mainSystem->pollEvent( event ) ){
			switch( event.type ){
				case gtEventType::keyboard:
				if( event.keyboardEvent.isReleased( gtKey::K_ESCAPE ) ){
					mainSystem->shutdown(); // exit when key released
				}
				break;
			}
		}

		if( mainSystem->isRun() ){

			// Camera zoom
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
			if( mainSystem->isKeyPressed( gtKey::K_PGUP ) ) camera->setPosition( camera->getPosition() + v3f( 0.f, 0.1f, 0.0f ) );
			if( mainSystem->isKeyPressed( gtKey::K_PGDOWN ) ) camera->setPosition( camera->getPosition() - v3f( 0.f, 0.1f, 0.0f ) );


			driver->beginRender( true, gtColor( 0.2f, 0.2f, 0.2f, 1.f ) ); // RGBA.


			scene->renderScene(); // Draw all


			
			driver->setDepthState( false );
			text->render();
			text2->render();
			driver->setDepthState();

			driver->endRender();

			delta = (f32)(now - time)*0.0001f;
			time = now;

			fpstime += delta;

			fps_counter += 1 * 8;
			if( fpstime > 0.0125f ){
				fpstime = 0.f;
				fps = fps_counter;
				fps_counter = 0u;
			}
		}

	}

	return 0;
}