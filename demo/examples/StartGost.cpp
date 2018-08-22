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
	void	shutdown(){
		if( m_isInit )
			if( FreeConsole() )
				m_isInit = false;
	}

	// Returns true if window has been initiliazed correctly
	bool	isInit(){
		return m_isInit;
	}

	bool	isShow(){
		return true;
	}

  //Theese methods show/hide output window. Because we are using console, they are empty.
	void	show(){}
	void	hide(){}
	void    clear(){}

	void	print( const gtString& s ){
		gtString stru = s;
		gtStringA stra;
		util::stringUTF16_to_UTF8(stru,stra);
		printf( "%s\n", stra.c_str() );
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

	{ 
		gtFile_t file =  util::createFileForWriteText( u"out_text.txt" );
		gtTextFileInfo fi;
		fi.m_endian = gtTextFileEndian::Little;
		fi.m_format = gtTextFileFormat::UTF_16;
		fi.m_hasBOM = true;
		file->setTextFileInfo( fi );
		file->write( u"Hello world!" );
		
	}
	
	gtDeviceCreationParameters params;

#ifdef UseCustomOutput
	auto CustomOutputWindow = gtPtrNew<CustomOutput>(new CustomOutput());
	params.m_outputWindow = CustomOutputWindow.data();
#endif

#ifdef UseEventConsumer
	EventConsumer events;
	params.m_consumer = &events;
#endif

	auto mainSystem = gost_create_main_system( params );
 
	auto * output = mainSystem->getOutputWindow();
	output->setWindowText( u"Hello world" );
	output->print( u"Message text" );
#ifdef UseCustomOutput
	printf("printf text");
#endif
	gtLogWriter::printInfo(u"Info text %i %u %f %c %s", -123, 0xFFFFFFFF, 0.2412f, u'c', u"String" );
	gtLogWriter::printWarning(u"Warning text %i %u %f %c %s", -123, 0xFFFFFFFF, 0.2412f, u'c', u"String" );
	gtLogWriter::printError(u"Error text %i %u %f %c %s", -123, 0xFFFFFFFF, 0.2412f, u'c', u"String" );
	mainSystem->getLog()->print( gtLog::msgType::Info, u"mainSystem->getLog()->print" );
 

	gtWindowInfo wi;
    wi.m_style = gtWindowInfo::style_standart;
    wi.m_style |= gtWindowInfo::style_maximize;
    wi.m_style |= gtWindowInfo::style_resize;
	auto window = mainSystem->createSystemWindow( &wi );

	gtGraphicsSystemInfo gsi;
	gsi.m_outWindow = window.data();
	
	/*
		auto audioDriver = mainSystem->createAudioSystem( GT_UID_AUDIO_XADUDIO2 );
		auto sound = audioDriver->createAudioObject( u"sound.ogg" );
		sound->play();
	*/
	
#define ___ASD
#ifdef ___ASD	
	auto graphicsSystem = mainSystem->createGraphicsSystem( gsi, GT_UID_RENDER_D3D11 );

	
	gtPtr<gtImage> image_original = mainSystem->loadImage( u"D:/1.png" );
	
	gtImage image_converted;
	image_converted	= image_original.data();
	
	gtImage::_convert( gtImageFormat::One_bit, &image_converted );
//	image_converted.convert(gtImageFormat::R8G8B8A8);
	gtImage::_convert( gtImageFormat::R8G8B8A8, &image_converted );
	
	auto texture = graphicsSystem->createTexture( &image_converted );
	
	gtMaterial mat;
	mat.textureLayer[ 0 ].texture = texture.data();
	mat.flags = (u32)gtMaterialFlag::AlphaBlend;
#endif	

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

		if( mainSystem->getInputSystem()->isKeyDown( gtKey::K_NUM_ADD ) ){
			r += 0.001f;
		}
		if( mainSystem->getInputSystem()->isKeyDown( gtKey::K_NUM_SUB ) ){
			r -= 0.001f;
		}

#ifdef UseEventConsumer
		auto * curPos = &mainSystem->getInputSystem()->getCursorPosition();
		g = curPos->x * 0.001f;
		b = curPos->y * 0.001f;
#endif
		if( mainSystem->getInputSystem()->isLMBDown() ){/**/}
		if( mainSystem->getInputSystem()->isRMBDown() ){/**/}
		if( mainSystem->getInputSystem()->isMMBDown() ){/**/}
		Function(); //LMB

		if( mainSystem->isRun() ){
#ifdef ___ASD	
			if( graphicsSystem ){
				graphicsSystem->beginRender( true, gtColor(r,g,b,1.f) );
				graphicsSystem->draw2DImage( v4i(0, 0, 512, 512), mat );
				graphicsSystem->endRender();
			}
#endif
		}
	}
	

	return 0;
}

void Function(){
	if( gtMainSystem::getInstance()->getInputSystem()->isLMBDown() ){
		gtLogWriter::printInfo( u"Hello from %s", gtStringA( GT_FUNCTION ).to_utf16String().data() );
	}
}