#include "creator.h"

demo::DemoApplication::DemoApplication( void ):
m_guiSystem( nullptr ),
m_gamepad( nullptr ),
m_backgroundTexture( nullptr ),
m_gamepadTexture( nullptr ),
m_state( DemoState::MainMenu ){

	demo::DemoApplicationContext context;
	context.app = this;
	m_eventConsumer = new demo::DemoApplicationEventConsumer( context );

#ifdef GT_PLATFORM_WIN32
	m_params.m_device_type = gtDeviceType::Windows;

	m_outputWindow	=	gtPtrNew<gtOutputWindow>( new DemoApplicationOutputWindow );
	m_params.m_outputWindow = m_outputWindow.data();
#elif GT_PLATFORM_LINUX
	m_params.m_device_type = gtDeviceType::Linux;
#endif

	m_params.m_consumer		= m_eventConsumer;
}

demo::DemoApplication::~DemoApplication( void ){
	delete m_eventConsumer;
}

gtMainSystem	*	demo::DemoApplication::GetMainSystem( void ){
	return m_mainSystem.data();
}

bool demo::DemoApplication::Init( void ){
	if( !initEngine() )
		return false;

	if( !initStrings() )
		return false;

	if( !initWindow() )
		return false;

	if( !initVideoDriver() )
		return false;

	if( !initMainMenu() )
		return false;
	
	m_gamepadSystem	=	m_mainSystem->createGameContoller( GT_UID_INPUT_DINPUT );

	return true;
}

bool demo::DemoApplication::initEngine( void ){
	m_mainSystem = gost::InitializeGoSTEngine( m_params );
	if( !m_mainSystem.data() )
		return false;
	return true;
}

bool demo::DemoApplication::initWindow( void ){
	m_windowInfo.m_style |= gtWindowInfo::maximize;
    m_windowInfo.m_style |= gtWindowInfo::resize;
	m_windowInfo.m_style |= gtWindowInfo::center;
	
	// make ~1600/900 aspect ration
	v2i screenSize = m_mainSystem->getScreenSize();
	s32 y = screenSize.y;
	if( (f32)((f32)screenSize.x / (f32)y) < 1.7f ){
		while(true){
			y -= 1;
			if( (f32)((f32)screenSize.x / (f32)y) < 1.7f )
				continue;
			else break;
		}
	}
	m_windowInfo.m_rect.set( 0, 0, screenSize.x, y );

	m_mainWindow = m_mainSystem->createSystemWindow( &m_windowInfo );
	if( !m_mainWindow.data() ){
		gtLogWriter::printError( u"Can not create main window" );
		return false;
	}

	return true;
}

bool demo::DemoApplication::initVideoDriver( void ){
	m_driverInfo.m_adapterID		=	0;
	m_driverInfo.m_backBufferSize.x	=	m_windowInfo.m_rect.getWidth();
	m_driverInfo.m_backBufferSize.y	=	m_windowInfo.m_rect.getHeight();
	m_driverInfo.m_colorDepth		=	32;
	m_driverInfo.m_doubleBuffer		=	true;
	m_driverInfo.m_fullScreen		=	false;
	m_driverInfo.m_outWindow		=	m_mainWindow.data();
	m_driverInfo.m_stencilBuffer	=	true;
	m_driverInfo.m_vSync			=	false;

	m_driver = m_mainSystem->createVideoDriver( m_driverInfo, GT_UID_RENDER_D3D11 );
	if( !m_driver.data() )
		return false;

	m_guiSystem	=	m_mainSystem->getGUISystem( m_driver.data() );

	return true;
}

bool demo::DemoApplication::initMainMenu( void ){
	gtString logoPath(u"../demo/media/logo.png");
	gtString gamepadPath(u"../demo/media/gamepad.png");

	if( gtFileSystem::existFile( logoPath ) )
		m_backgroundTexture = m_driver->getTexture( logoPath );
	else
		gtLogWriter::printWarning( u"Can not load background texture. File %s not exist.", logoPath.data() );

	if( gtFileSystem::existFile( gamepadPath ) )
		m_gamepadTexture = m_driver->getTexture( gamepadPath );
	else
		gtLogWriter::printWarning( u"Can not load gamepad icon texture. File %s not exist.", gamepadPath.data() );
	
	m_mainFont	=	m_guiSystem->createBuiltInFont();

	return rebuildMainMenu();
}

bool demo::DemoApplication::initStrings( void ){
	gtArray<gtString> strings;
	util::getFilesFromDir( &strings, u"../demo/langs/" );
	
	DemoLang dl;
	u32 dlcount = -1;

	u32 sz = strings.size();
	for( u32 i = 0u; i < sz; ++i ){
		auto ext = util::stringGetExtension( strings[ i ] );
		util::stringToLower( ext );
		if( ext == u"xml" ){

			auto xml = m_mainSystem->XMLRead( strings[ i ] );
			if( xml.data() ){

				auto root = xml->getRootNode();
				if( root ){
					
					if( root->name == u"Lang" ){

						m_stringArray.push_back( dl );
						++dlcount;

						if( root->attributeList.size() ){
							if( root->attributeList[ 0u ]->name == u"n" ){
								m_stringArray[ dlcount ].m_langName = root->attributeList[ 0u ]->value;

								auto nodes = xml->selectNodes( u"/Lang/t" );
								if( nodes.size() ){

									u32 nsz = nodes.size();
									for( u32 i2 = 0u; i2 < nsz; ++i2 ){
										if( nodes[ i2 ]->name == u"t" ){
											m_stringArray[ dlcount ].m_stringArray.push_back( nodes[ i2 ]->text );
										}
									}

								}
							}
						}

					}

				}

			}

		}
	}

	return true;
}

void demo::DemoApplication::RebuildGUI( void ){

	switch( m_state ){
	case demo::DemoState::MainMenu:
		rebuildMainMenu();
		break;
	case demo::DemoState::DemoMenu:
		break;
	case demo::DemoState::DemoRun:
		break;
	}
}

bool demo::DemoApplication::rebuildMainMenu( void ){
	v4i wndrc = m_mainWindow->getRect();

	f32 wndH = (f32)wndrc.getHeight();
	f32 wndW = (f32)wndrc.getWidth();

	v4i bgrc;
	if( m_backgroundTexture ){
		f32 txH = (f32)m_backgroundTexture->getHeight();
		f32 txW = (f32)m_backgroundTexture->getWidth();

		bgrc.z = wndrc.getWidth();
		bgrc.w = txH;


		if( wndH > txH ){
			f32 v = (wndH - txH)*0.5f;
			bgrc.y += v;
			bgrc.w += v;
		}

		if( wndW < txW ){
			f32 v = (wndW/txW);
			bgrc.w *= v;
			v = (wndH * 0.5f) - (bgrc.w * 0.5f);
			bgrc.y += v;
			bgrc.w += v;
		}
	}

	m_backgroundShape	= m_guiSystem->createShapeRectangle( bgrc, gtColor( gtColorWhite ) );
	if( !m_backgroundShape.data() ){
		gtLogWriter::printError( u"Can not create background shape." );
		return false;
	}

	if( m_backgroundTexture ){
		m_backgroundShape->setTexture( m_backgroundTexture );
	}else{
		m_backgroundShape->setColor( gtColor( gtColorBlack ) );
	}
	m_backgroundShape->setOpacity( 0.8f );

	v4i gprc;
	gprc.x = m_windowInfo.m_rect.z - 64;
	gprc.y = m_windowInfo.m_rect.w - 41;
	gprc.z = gprc.x + 64;
	gprc.w = gprc.y + 41;

	gtColor gamepadColor(1.f);
	if( m_gamepad )
		gamepadColor = gtColor( gtColorGreen );
	
	m_gamepadiconShape	= m_guiSystem->createShapeRectangle( gprc, gamepadColor );
	if( !m_gamepadiconShape.data() ){
		gtLogWriter::printError( u"Can not create gamepad icon shape." );
		return false;
	}
	if( m_gamepadTexture ){
		m_gamepadiconShape->setTexture( m_gamepadTexture );
	}else{
		m_gamepadiconShape->setColor( gtColor( gtColorBlack ) );
	}
	
	if( !m_gamepad )
		m_gamepadiconShape->setOpacity( 0.25f );

	m_welcomeText = m_guiSystem->createTextField( v4i( 20, 0, f32(wndrc.getWidth()-20) * ( (f32)m_driverInfo.m_backBufferSize.x / (f32)wndrc.getWidth() ), 0 ), m_mainFont.data(), false );
	m_welcomeText->setText( m_stringArray[ 0u ].m_stringArray[ 0u ] );
	m_welcomeText->setOpacity( 0.9f );
	m_welcomeText->getBackgroundShape()->setOpacity( 0.f );

	return true;
}

void demo::DemoApplication::Run( void ){

	m_mainSystem->setTimer( 300 );

	while( m_mainSystem->update() ){

		if( m_gamepad )
			m_gamepad->poll();

		switch( m_state ){
		case demo::DemoState::MainMenu:
			if( m_mainSystem->isKeyPressed( gtKey::K_ESCAPE ) ){
				m_mainSystem->shutdown();
			}
			renderMainMenu();
			break;
		case demo::DemoState::DemoMenu:
			break;
		case demo::DemoState::DemoRun:
			break;
		}


	}
}

void demo::DemoApplication::renderMainMenu( void ){
	m_driver->beginRender();

	m_driver->setDepthState( false );
	m_backgroundShape->render();
	m_gamepadiconShape->render();
	m_welcomeText->render();
	m_driver->setDepthState();

	m_driver->endRender();
}

void demo::DemoApplication::ScanGamepads( void ){
	if( m_gamepadSystem.data() ){
		m_mainSystem->setTimer( 300u );
		if( !m_gamepad )
			m_gamepadSystem->update();
	}
}

void demo::DemoApplication::ActivateGamepad( bool value, gtGameControllerDevice* g ){
	if( value ){
		if( !m_gamepad ){
			m_gamepad = g;
			m_gamepadiconShape->setColor( gtColorGreen );
			m_gamepadiconShape->setOpacity( 1.f );
		}
	}else{
		m_gamepad = nullptr;
		m_gamepadiconShape->setColor( gtColorWhite );
		m_gamepadiconShape->setOpacity( 0.25f );
	}
}

/*
Copyright (c) 2018 532235

Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
and associated documentation files (the "Software"), to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT 
LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/