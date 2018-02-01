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
	di.m_vSync		= true;
	di.m_outWindow	= window.data(); /// Set output window for rendering.
	gtPtr_t(gtDriver,driver,mainSystem->createVideoDriver( di, GT_UID_RENDER_D3D11 ));


	///	Get scene system
	gtSceneSystem * scene = mainSystem->getSceneSystem();

	///	Load images and create textures
	///	Use gtPtr_t for automatic deletion
	gtPtr_t( gtImage, image_Frog, mainSystem->loadImage( u"../media/Frog.png" ));
	gtPtr_t( gtTexture, texture_Frog, driver->createTexture( image_Frog.data(), gtTextureFilterType::FILTER_PPP ));


	gtSprite * Frog = scene->addSprite(
		texture_Frog.data(),
		driver.data(),
		v2f_t( 25 / 100.f, 25 / 100.f ));

	///	Add another sprite for test z-order.
	gtSprite * Frog2 = scene->addSprite(
		texture_Frog.data(),
		driver.data(),
		v2f_t( 25 / 100.f, 25 / 100.f ));

	///	Move up
	Frog2->setPosition( Frog2->getPosition() + v3f_t( 0.f, 1.f, 0.f ) );
	/// Set z position, depends on Y coordinate.
	Frog2->setPosition( v3f_t( Frog2->getPosition()[0u], Frog2->getPosition()[1u], Frog2->getPosition()[1u]*0.1f + 3.f ) );

	///	Create animation automatically. 28 num of frames, 21 frame width, 24 frame height
	Frog->createAnimation( 28, v2u_t( 21, 24 ) );
	Frog->setFrameRate( 10.f );
	Frog->playAnimation();

	// zero based - 7 frames
	Frog->setLoopSegment( 0, 6 ); /// walk down
	//Frog->setLoopSegment( 7, 13 ); /// walk up
	//Frog->setLoopSegment( 14, 20 ); /// walk left
	//Frog->setLoopSegment( 21, 27 ); /// walk right
	 

	///	Add 2D camera
	gtCamera * camera = scene->addCamera2D( v4f_t( 0.f, 0.f, 800.f, 600.f ) );

	/// Add background
	gtPtr_t( gtImage, image_bg, mainSystem->loadImage( u"../media/Kajar.png" ));
	gtPtr_t( gtTexture, texture_bg, driver->createTexture( image_bg.data(), gtTextureFilterType::FILTER_PPP ));


	gtSprite * background = scene->addSprite(
		texture_bg.data(),
		driver.data(),
		v2f_t( 511 / 100.f, 503 / 100.f ));
	background->setPosition( v3f_t( 0.f, 0.f, 10.f ) );
	
	///	for pollEvent
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

			Frog->pauseAnimation(); /// stand still

			if( mainSystem->isKeyPressed( gtKey::K_W ) ){
				Frog->setPosition( Frog->getPosition() + v3f_t( 0.f, move_speed * delta, 0.f ) );
			
				Frog->setPosition( v3f_t( Frog->getPosition()[0], Frog->getPosition()[1], Frog->getPosition()[1u]*0.1f + 3.f  ) ); /// update z

				Frog->playAnimation();
				if( Frog->getCurrentFrame() < 7 || Frog->getCurrentFrame() > 13 )
					Frog->setLoopSegment( 7, 13 ); /// walk up

			}else if( mainSystem->isKeyPressed( gtKey::K_S ) ){
				Frog->setPosition( Frog->getPosition() - v3f_t( 0.f, move_speed * delta, 0.f ) );
			
				Frog->setPosition( v3f_t( Frog->getPosition()[0], Frog->getPosition()[1], Frog->getPosition()[1u]*0.1f + 3.f ) ); /// update z

				Frog->playAnimation();
				if( Frog->getCurrentFrame() > 6 )
					Frog->setLoopSegment( 0, 6 ); /// walk down

			}else if( mainSystem->isKeyPressed( gtKey::K_A ) ){
				Frog->setPosition( Frog->getPosition() + v3f_t( move_speed * delta, 0.f, 0.f ) );
			
				Frog->playAnimation();
				if( Frog->getCurrentFrame() < 14 || Frog->getCurrentFrame() > 20 )
					Frog->setLoopSegment( 14, 20 ); /// walk left

			}else if( mainSystem->isKeyPressed( gtKey::K_D ) ){
				Frog->setPosition( Frog->getPosition() - v3f_t( move_speed * delta, 0.f, 0.f ) );
			
				Frog->playAnimation();
				if( Frog->getCurrentFrame() < 21 || Frog->getCurrentFrame() > 27 )
					Frog->setLoopSegment( 21, 27 ); /// walk right
			}

			///	The camera follows the player
			v3f camera_position = camera->getPosition();
			camera_position[ 0u ] = -Frog->getPosition()[ 0u ];
			camera_position[ 1u ] = -Frog->getPosition()[ 1u ];
			camera->setPosition(  camera_position  );

			driver->beginRender( true, gtColor( 0.7372549019607843f, 0.8901960784313725f, 1.f, 1.f ) ); /// RGBA.
			scene->renderScene(); /// Draw all
			driver->endRender();

			delta = (f32)(now - time)*0.0001f;
			time = now;
		}
	}

	return 0;
}