#include <gost.h>

using namespace gost;


#if defined( GT_PLATFORM_WIN32 )
int WINAPI WinMain( HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nCmdShow*/ ){
#endif

	auto mainSystem = InitializeGoSTEngine();

	gtWindowInfo wi;
	auto window = mainSystem->createSystemWindow( wi );


	gtDriverInfo di;
	di.m_outWindow = window.data(); /// Set output window for rendering.
	di.m_vSync = true;
	auto driver = mainSystem->createVideoDriver( di, GT_UID_RENDER_D3D11 );


	//	Get scene system
	gtSceneSystem * scene = mainSystem->getSceneSystem( driver.data() );

	//	Load images and create textures
	auto image_tank = mainSystem->loadImage( u"../media/tank_a.png" );
	auto image_cannon = mainSystem->loadImage( u"../media/tank_b.png" );
	auto texture_tank = driver->createTexture( image_tank.data(), gtTextureFilterType::FILTER_PPP );
	auto texture_cannon = driver->createTexture( image_cannon.data(), gtTextureFilterType::FILTER_PPP );


	gtSprite * tank = scene->addSprite(
		texture_tank.data(),
		v2f( (f32)texture_tank->getWidth() / 100.f,
		(f32)texture_tank->getHeight() / 100.f ),
		v3f(0.f,0.f,10.f));

	gtSprite * cannon = scene->addSprite(
		texture_cannon.data(),
		v2f( (f32)texture_cannon->getWidth() / 100.f,
		(f32)texture_cannon->getHeight() / 100.f ),
		v3f(0.f,0.f,-0.1f) // set closer to camera, like v3f(0.f,0.f, 10.f - 0.1f )
	);

	//	Attach the gun to the tank
	cannon->setParent( tank );

	//	Add 2D camera
	gtCamera * camera = scene->addCamera2D( v4f( 0.f, 0.f, 800.f, 600.f ) );

	
	//	for pollEvent
	gtEvent event;

	f32 delta = 0.f;
	f32 move_speed = 10.f;
	u32 time = mainSystem->getTime();

	while( mainSystem->update() ){


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


			if( mainSystem->isKeyPressed( gtKey::K_W ) ){
				tank->setPosition( tank->getPosition() + v3f( 0.f, move_speed * delta, 0.f ) );
				tank->setRotation( v3f( 0.f, 0.f, -PI/2.f ) ); /// use math::radToDeg if you need

			}else if( mainSystem->isKeyPressed( gtKey::K_S ) ){
				tank->setPosition( tank->getPosition() - v3f( 0.f, move_speed * delta, 0.f ) );
				tank->setRotation( v3f( 0.f, 0.f, PI/2.f ) );

			}else if( mainSystem->isKeyPressed( gtKey::K_A ) ){
				tank->setPosition( tank->getPosition() + v3f( move_speed * delta, 0.f, 0.f ) );
				tank->setRotation( v3f( 0.f, 0.f, 0.f ) );

			}else if( mainSystem->isKeyPressed( gtKey::K_D ) ){
				tank->setPosition( tank->getPosition() - v3f( move_speed * delta, 0.f, 0.f ) );
				tank->setRotation( v3f( 0.f, 0.f, PI ) );
			}


			if( mainSystem->isKeyPressed( gtKey::K_Q ) )
				cannon->setRotation( cannon->getRotation() + v3f( 0.f, 0.f, 10.f * delta ) );
        
			if( mainSystem->isKeyPressed( gtKey::K_E ) )
				cannon->setRotation( cannon->getRotation() - v3f( 0.f, 0.f, 10.f * delta ) );

			driver->beginRender( true, gtColor( 0.7372549019607843f, 0.8901960784313725f, 1.f, 1.f ) ); /// RGBA.

			scene->renderScene(); /// Draw all

			driver->endRender();

			delta = (f32)(now - time)*0.0001f;
					
			time = now;
		}

	}

	return 0;
}