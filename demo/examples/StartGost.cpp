#include <gost.h>
using namespace gost;

// GoST have various ways for event handling. 
// Like eventReceiver in IrrLicht
#define UseEventConsumer
#ifdef UseEventConsumer
class EventConsumer : public gtEventConsumer{
public:
	EventConsumer(){
	}
	~EventConsumer( void ){
	}

	void processEvent( const gtEvent& ev ){
		switch( ev.type ){
			case gtEventType::GUI:{
			}break;
			case gtEventType::Joystick:{
			}break;
			case gtEventType::Keyboard:{
				if( ev.keyboardEvent.key == gtKey::K_ESCAPE )
					if( ev.keyboardEvent.state.bits.b0 )
						gtMainSystem::getInstance()->shutdown();
			}break;
			case gtEventType::Mouse:{
			}break;
			case gtEventType::System:{
			}break;
			case gtEventType::Window:{
			}break;
		}
	}
};
#endif


// if you want standart console in Windows
//#define UseCustomOutput
#ifdef UseCustomOutput
class CustomOutput : public gtOutputWindow {
private:
		HWND m_hWnd;
		bool m_isInit;
		~CustomOutput( void ){
			shutdown();
		}
public:

	CustomOutput( void ) : m_hWnd( 0 ), m_isInit( false ){
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

	bool	isShow( void ){
		return true;
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
#endif

void Function();

// In Windows, default Subsystem = Windows
#if defined( GT_PLATFORM_WIN32 )
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ){
#else
int main(){
#endif

	gtDeviceCreationParameters params;

#ifdef UseCustomOutput
	auto CustomOutputWindow = gtPtrNew<CustomOutput>(new CustomOutput());
	params.m_outputWindow = CustomOutputWindow.data();
#endif

#ifdef UseEventConsumer
	EventConsumer events;
	params.m_consumer = &events;
#endif

	auto mainSystem = InitializeGoSTEngine( params );
 
	auto * output = mainSystem->getOutputWindow();
	output->setWindowText( u"Hello world" );
	output->print( u"Message text" );
#ifdef UseCustomOutput
	printf("printf text");
#endif
	gtLogWriter::printInfo(u"Info text %i %u %f %c %s", -123, 0xFFFFFFFF, 0.2412f, u'c', u"String" );
	gtLogWriter::printWarning(u"Warning text %i %u %f %c %s", -123, 0xFFFFFFFF, 0.2412f, u'c', u"String" );
	gtLogWriter::printError(u"Error text %i %u %f %c %s", -123, 0xFFFFFFFF, 0.2412f, u'c', u"String" );
	mainSystem->getLog()->print( gtLog::msgType::info, u"mainSystem->getLog()->print" );
 

	gtWindowInfo wi;
    wi.m_style = gtWindowInfo::standart;
    wi.m_style |= gtWindowInfo::maximize;
    wi.m_style |= gtWindowInfo::resize;
	auto window = mainSystem->createSystemWindow( &wi );

	gtDriverInfo di;
	di.m_outWindow = window.data();

	auto driver = mainSystem->createVideoDriver( di, GT_UID_RENDER_D3D11 );


	f32 r = 0.f;
	f32 g = 0.f;
	f32 b = 0.f;

	gtEvent event;

	while( mainSystem->update() ){
#ifndef UseEventConsumer
		//Like in SFML
		while( mainSystem->pollEvent( event ) ){
        switch( event.type ){
            case gtEventType::Keyboard:
                if( event.keyboardEvent.isPressed( gtKey::K_ESCAPE ) )
                    mainSystem->shutdown();
            break;
			case gtEventType::Mouse:
				g = event.mouseEvent.x * 0.001f;
				b = event.mouseEvent.y * 0.001f;
			break;
        }
		}
#endif

		if( mainSystem->isKeyPressed( gtKey::K_NUM_ADD ) ){
			r += 0.001f;
		}
		if( mainSystem->isKeyPressed( gtKey::K_NUM_SUB ) ){
			r -= 0.001f;
		}

#ifdef UseEventConsumer
		auto * curPos = &mainSystem->getCursorPosition();
		g = curPos->x * 0.001f;
		b = curPos->y * 0.001f;
#endif
		if( mainSystem->isLMBDown() ){/**/}
		if( mainSystem->isRMBDown() ){/**/}
		if( mainSystem->isMMBDown() ){/**/}
		Function(); //LMB

		if( mainSystem->isRun() ){
			driver->beginRender( true, gtColor(r,g,b,1.f) );
			driver->endRender();
		}
	}
 
	return 0;
}

void Function(){
	if( gtMainSystem::getInstance()->isLMBDown() ){
		gtLogWriter::printInfo( u"Hello from %s", gtStringA( GT_FUNCTION ).to_utf16String().data() );
	}
}