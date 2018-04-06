#include <gost.h>

using namespace gost;

#if defined( GT_PLATFORM_WIN32 )
int WINAPI WinMain( HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPSTR /*lpCmdLine*/, int /*nCmdShow*/ ){
#endif

	auto mainSystem = InitializeGoSTEngine();

	auto window = mainSystem->createSystemWindow( gtWindowInfo() );

	window->setWindowTitle( u"05 Play sound" );

	gtDriverInfo di;
	di.m_outWindow = window.data();
	di.m_vSync = true;
	auto driver = mainSystem->createVideoDriver( di, GT_UID_RENDER_D3D11 );


	auto audioSystem = mainSystem->createAudioSystem( GT_UID_AUDIO_XADUDIO2 );

	// Without `gtPtr_t` `audioSource` will be removed automatically( when destroy `audio` )
	//	If you need audioSource, add `audioSource->addRef();`
	gtAudioSource* audioSource = audioSystem->loadAudioSource( u"../media/escapade.wav" );

		//	Try load full music file. Very slow decoding (in Debug), and it takes a lot of memory. 
	//gtAudioSource* audioSource = audioSystem->loadAudioSource( u"../media/Static-X - The Only.ogg" );

		//	Create audio object. Run this example, press F1 multiple times, see result. Go back here, 
		//	uncomment second argument and see what happens if you try press F1 again.
	gtPtr_t( gtAudioObject, audio, audioSystem->createAudioObject( audioSource /*,2*/ ) );
	audio->play();
	//audio->setLoop( true );


		/// one line version
	//gtPtr_t( gtAudioObject, audio, audioSystem->createAudioObject( u"../media/Static-X - The Only.ogg" ) );


	
	gtPtr_t( gtAudioStream, stream, audioSystem->createStream( u"../media/Static-X - The Only.ogg" ) );
	if( stream.data() ){
		stream->play();
		stream->setLoop( true );
	}

	gtEvent event;
	f32 delta = 0.f;
	f32 move_speed = 10.f;
	u32 time = mainSystem->getTime();

	while( mainSystem->update() ){
		u32 now = mainSystem->getTime();
		while( mainSystem->pollEvent( event ) ){
			switch( event.type ){ 
				
				case gtEventType::keyboard:

				if( event.keyboardEvent.isPressed( gtKey::K_LEFT ) ){
					stream->setPlaybackPosition( stream->getPlaybackPosition() - 0.1f );
				}
				
				if( event.keyboardEvent.isPressed( gtKey::K_RIGHT ) ){
					stream->setPlaybackPosition( stream->getPlaybackPosition() + 0.1f );
				}

				if( event.keyboardEvent.isPressed( gtKey::K_UP ) )
					stream->setVolume( stream->getVolume() + 0.01f );
				if( event.keyboardEvent.isPressed( gtKey::K_DOWN ) )
					stream->setVolume( stream->getVolume() - 0.01f );

				if( event.keyboardEvent.isPressed( gtKey::K_P ) )
					stream->play();

				if( event.keyboardEvent.isPressed( gtKey::K_S ) )
					stream->stop();

				if( event.keyboardEvent.isPressed( gtKey::K_O ) )
					stream->pause();

				/*if( event.keyboardEvent.isPressed( gtKey::K_F1 ) )
					audio->play();
				if( event.keyboardEvent.isPressed( gtKey::K_F2 ) )
					audio->pause();
				if( event.keyboardEvent.isPressed( gtKey::K_F3 ) )
					audio->stop();*/
				

				if( event.keyboardEvent.isReleased( gtKey::K_ESCAPE ) ){
					mainSystem->shutdown(); /// exit when key released
				}
				break;
			}
		}

	/*	if( mainSystem->isKeyPressed( gtKey::K_NUM_ADD ) )
			audio->setVolume( audio->getVolume() + 1.f * delta );
		if( mainSystem->isKeyPressed( gtKey::K_NUM_SUB ) )
			audio->setVolume( audio->getVolume() - 1.f * delta );*/

		if( mainSystem->isRun() ){

			if( mainSystem->isKeyPressed( gtKey::K_ESCAPE ) )
				mainSystem->shutdown();

			driver->beginRender( true, gtColor( 0.7372549019607843f, 0.8901960784313725f, 1.f, 1.f ) ); /// RGBA.
			driver->endRender();
		}

		delta = (f32)(now - time)*0.0001f;
		time = now;
	}
	

	return 0;
}