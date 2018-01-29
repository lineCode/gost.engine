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
	/// params.m_device_type = gtDeviceType::windows; /// Default - windows.
	/// params.m_fontName = "myFont.ttf"; /// Your font. Default - consolas.ttf, or Courier new.

	///	About this, see code in bottom.
	///	gtPtr_t( CustomOutput, output, new CustomOutput );
	///	params.m_outputWindow = output.data(); /// If you want create your own output window, set this parameter.
	
	/// Like EventReceiver in IrrLicht. Handle engine events in special class with callback method.
	///	Use it if you don't want use gtMainSystem::pollEvent.
	///	params.m_consumer = &eventConsumer;

	gtPtr<gtMainSystem> mainSystem( gtPtrNew<gtMainSystem>( InitializeGoSTEngine(params) ) );
	/// Alternative version. Use helper macros.
	/// gtPtr_t(gtMainSystem,mainSystem,InitializeGoSTEngine(params));

	auto * output = mainSystem->getOutputWindow();
	output->print( u"Hello world" );


	///	Create main window.
	gtWindowInfo wi;
    wi.m_style |= gtWindowInfo::maximize;
    wi.m_style |= gtWindowInfo::resize;
	
	gtPtr_t(gtWindow,window,mainSystem->createSystemWindow( wi ));


	///	Initialize graphics engine.

	///	1. You need get gtPluginSystem.
	gtPluginSystem * pluginSystem = mainSystem->getPluginSystem();

	///	2. Get standart render plugin.
	///	All plugins have Unique IDs. All UIDs for standart plugins defined in gtPluginSystem.h.
	///	Call pluginSystem->getPlugin with GT_UID_RENDER_D3D11 argument for get Direct3D 11 plugin.
	///	pluginSystem->getPlugin will return polymorphic class, common for all plugin types.
	///	Convert gtPlugin to gtPluginRender calling pluginSystem->getAsPluginRender.
	gtPluginRender * d3d11Plugin = pluginSystem->getAsPluginRender( pluginSystem->getPlugin( GT_UID_RENDER_D3D11 ) );

	///	3. Set up render plugin parameters.
	gtDriverInfo di;
	/// di.m_fullScreen	=	true;
	///	di.m_backBufferSize	=	v2i_t( 1600, 900 ); /// Or calculate from wi.rect if you need.
	///	di.m_vSync = true;
	di.m_outWindow = window.data(); /// Set output window for rendering.

	///	4. Initialize graphics engine.
	gtPtr_t(gtDriver,driver,d3d11Plugin->loadDriver( di ));

	while( mainSystem->update() ){

		///	If you do not use gtEventConsumer, use this method.
		///	This method update all events in event queue.
		while( mainSystem->pollEvent( gtEvent() ) );

		///	This 'if' will help when program terminated
		if( mainSystem->isRun() ){

			///	After 'pollEvent', you can get key state.
			if( mainSystem->isKeyPressed( gtKey::K_ESCAPE ) ){
				mainSystem->shutdown(); /// mainSystem->update() return false
			}

			///	Before starts rendering, call driver->beginRender
			///	true - clear back buffer
			driver->beginRender( true, gtColor( 0.7372549019607843f, 0.8901960784313725f, 1.f, 1.f ) ); /// RGBA.
			// driver->beginRender( true, gtColor( 188, 227, 255, 255 ) ); /// RGBA
			// driver->beginRender( true, gtColor( 0xFFBCE3FF ) ); /// ARGB

			///	After rendering, call driver->endRender
			driver->endRender();
		}
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
