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

	window->setWindowTitle( u"05 Play sound" );

	gtDriverInfo di;
	di.m_outWindow = window.data();
	gtPtr_t(gtDriver,driver,mainSystem->createVideoDriver( di, GT_UID_RENDER_D3D11 ));


	gtPtr_t( gtAudioSystem, audioSystem, mainSystem->createAudioSystem( GT_UID_AUDIO_XADUDIO2 ) );

	/// Without `gtPtr_t` `audioSource` will be removed automatically( when destroy `audio` )
	///	If you need audioSource, add `audioSource->addRef();`
	gtAudioSource* audioSource = audioSystem->loadAudioSource( u"../media/escapade.wav" );

		///	Try load full music file. Very slow decoding (in Debug), and it takes a lot of memory. 
	//gtAudioSource* audioSource = audioSystem->loadAudioSource( u"../media/Static-X - The Only.ogg" );

		///	Create audio object. Run this example, press F1 multiple times, see result. Go back here, 
		///	uncomment second argument and see what happens if you try press F1 again.
	gtPtr_t( gtAudioObject, audio, audioSystem->createAudioObject( audioSource /*,2*/ ) );


		/// one line version
	///	gtPtr_t( gtAudioObject, audio, audioSystem->createAudioObject( u"../media/escapade.wav" ) );


	//audio->setLoop( true );
	audio->play();

	gtEvent event;
	f32 delta = 0.f;
	f32 move_speed = 10.f;
	u32 time = mainSystem->getTime();

	while( mainSystem->update() ){

		u32 now = mainSystem->getTime();

		while( mainSystem->pollEvent( event ) ){
			switch( event.type ){
				case gtEventType::keyboard:

				if( event.keyboardEvent.isPressed( gtKey::K_F1 ) )
					audio->play();
				if( event.keyboardEvent.isPressed( gtKey::K_F2 ) )
					audio->pause();
				if( event.keyboardEvent.isPressed( gtKey::K_F3 ) )
					audio->stop();
				

				if( event.keyboardEvent.isReleased( gtKey::K_ESCAPE ) ){
					mainSystem->shutdown(); /// exit when key released
				}
				break;
			}
		}

		if( mainSystem->isKeyPressed( gtKey::K_NUM_ADD ) )
			audio->setVolume( audio->getVolume() + 1.f * delta );
		if( mainSystem->isKeyPressed( gtKey::K_NUM_SUB ) )
			audio->setVolume( audio->getVolume() - 1.f * delta );

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

///	If you want use your own output window, or want use printf family functions, this class show you how to do it.
/// Put this code after line with #include <Windows.h>

/*
class CustomOutput : public gtOutputWindow {
private:
		HWND m_hWnd;
		bool m_isInit;
		~CustomOutput( void ){
			shutdown();
		}
public:

	CustomOutput( void ) : m_hWnd( 0 ), m_isInit( false ){
	#ifdef GT_DEBUG
			this->setDebugName( u"OutputWindow" );
	#endif
			init();
	}

	// Call this method before using the output window
	void	init( void ){

		 //In this example, all output will be displayed in console.
		if( !AllocConsole() ){
			return;
		}

		freopen("CONOUT$", "w", stdout);
		m_hWnd = GetConsoleWindow();
		m_isInit = true;
	}

	// Call when output window is no longer needed
	void	shutdown( void ){
		if( m_isInit )
			if( FreeConsole() )
				m_isInit = false;
	}

	// Returns true if window has been initiliazed correctly
	bool	isInit( void ){
		return m_isInit;
	}

  //Theese methods show/hide output window. Because we are using console, they are empty.
	void	show( void ){}
	void	hide( void ){}


	void	print( const gtString& s ){
		if( m_isInit )
			wprintf( L"%s\n", (wchar_t*)s.data() );
	}

	void	setWindowText( const gtString& s ){
		if( m_isInit )
			SetWindowText( m_hWnd, (wchar_t*)s.data() );
	 }
};
*/