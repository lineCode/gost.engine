#include <gost.h>

using namespace gost;


#if defined( GT_PLATFORM_WIN32 )
int WINAPI WinMain( HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nCmdShow*/ ){
#endif

	auto mainSystem = InitializeGoSTEngine();

	gtWindowInfo wi;
	auto window = mainSystem->createSystemWindow( &wi );

	
	gtDriverInfo di;
	di.m_vSync		= true;
	di.m_outWindow	= window.data(); // Set output window for rendering.
	auto driver = mainSystem->createVideoDriver( di, GT_UID_RENDER_D3D11 );


	//	Get scene system
	gtSceneSystem * scene = mainSystem->getSceneSystem( driver.data() );

	//	Load images and create textures
	//	Use gtPtr_t for automatic deletion
	auto image_Frog = mainSystem->loadImage( u"../media/Frog.png" );
	auto texture_Frog = driver->createTexture( image_Frog.data(), gtTextureFilterType::FILTER_PPP );


	gtSprite * Frog = scene->addSprite(
		texture_Frog.data(),
		v2f( 25 / 100.f, 25 / 100.f ));

	//	Add another sprite for test z-order.
	gtSprite * Frog2 = scene->addSprite(
		texture_Frog.data(),
		v2f( 25 / 100.f, 25 / 100.f ));

	//	Move up
	Frog2->setPosition( Frog2->getPosition() + v3f( 0.f, 1.f, 0.f ) );
	// Set z position, depends on Y coordinate.
	Frog2->setPosition( v3f( Frog2->getPosition().x, Frog2->getPosition().y, Frog2->getPosition().y*0.1f + 3.f ) );

	//	Create animation automatically. 28 num of frames, 21 frame width, 24 frame height
	Frog->createAnimation( 28, v2u( 21, 24 ) );
	Frog->setFrameRate( 10.f );
	Frog->playAnimation();

	// zero based - 7 frames
	Frog->setLoopSegment( 0, 6 ); // walk down
	//Frog->setLoopSegment( 7, 13 ); // walk up
	//Frog->setLoopSegment( 14, 20 ); // walk left
	//Frog->setLoopSegment( 21, 27 ); // walk right
	 

	//	Add 2D camera
	gtCamera * camera = scene->addCamera2D( v4f( 0.f, 0.f, 800.f, 600.f ) );

	// Add background
	auto image_bg = mainSystem->loadImage( u"../media/Kajar.png" );
	auto texture_bg = driver->createTexture( image_bg.data(), gtTextureFilterType::FILTER_PPP );


	gtSprite * background = scene->addSprite(
		texture_bg.data(),
		v2f( 511 / 100.f, 503 / 100.f ));
	background->setPosition( v3f( 0.f, 0.f, 10.f ) );
	
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
					mainSystem->shutdown(); // exit when key released
				}
				break;
			}
		}

		if( mainSystem->isRun() ){


			// Camera zoom
			if( mainSystem->isKeyPressed( gtKey::K_X ) ) camera->setFOV( camera->getFOV() + 10.f * delta );
			if( mainSystem->isKeyPressed( gtKey::K_Z ) ) camera->setFOV( camera->getFOV() - 10.f * delta );

			Frog->pauseAnimation(); // stand still

			if( mainSystem->isKeyPressed( gtKey::K_W ) ){
				Frog->setPosition( Frog->getPosition() + v3f( 0.f, move_speed * delta, 0.f ) );
			
				Frog->setPosition( v3f( Frog->getPosition().x, Frog->getPosition().y, Frog->getPosition().y*0.1f + 3.f  ) ); // update z

				Frog->playAnimation();
				if( Frog->getCurrentFrame() < 7 || Frog->getCurrentFrame() > 13 )
					Frog->setLoopSegment( 7, 13 ); // walk up

			}else if( mainSystem->isKeyPressed( gtKey::K_S ) ){
				Frog->setPosition( Frog->getPosition() - v3f( 0.f, move_speed * delta, 0.f ) );
			
				Frog->setPosition( v3f( Frog->getPosition().x, Frog->getPosition().y, Frog->getPosition().y*0.1f + 3.f ) ); // update z

				Frog->playAnimation();
				if( Frog->getCurrentFrame() > 6 )
					Frog->setLoopSegment( 0, 6 ); // walk down

			}else if( mainSystem->isKeyPressed( gtKey::K_A ) ){
				Frog->setPosition( Frog->getPosition() + v3f( move_speed * delta, 0.f, 0.f ) );
			
				Frog->playAnimation();
				if( Frog->getCurrentFrame() < 14 || Frog->getCurrentFrame() > 20 )
					Frog->setLoopSegment( 14, 20 ); // walk left

			}else if( mainSystem->isKeyPressed( gtKey::K_D ) ){
				Frog->setPosition( Frog->getPosition() - v3f( move_speed * delta, 0.f, 0.f ) );
			
				Frog->playAnimation();
				if( Frog->getCurrentFrame() < 21 || Frog->getCurrentFrame() > 27 )
					Frog->setLoopSegment( 21, 27 ); // walk right
			}

			//	The camera follows the player
			v3f camera_position = camera->getPosition();
			camera_position.x = Frog->getPosition().x;
			camera_position.y = Frog->getPosition().y;
			camera->setPosition(  camera_position  );

			driver->beginRender( true, gtColor( 0.7372549019607843f, 0.8901960784313725f, 1.f, 1.f ) ); // RGBA.
			scene->renderScene(); // Draw all
			driver->endRender();

			delta = (f32)(now - time)*0.0001f;
			time = now;
		}
	}

	return 0;
}