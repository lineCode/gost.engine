#include "creator.h"

demo::DemoApplication::DemoApplication( void ):
m_guiSystem( nullptr ),
m_gamepad( nullptr ),
m_backgroundTexture( nullptr ),
m_gamepadTexture( nullptr ),
m_languageID( 0u ),
m_activeDemoType( 0 ),
m_activeDemoTypeSelected( 0 ),
m_activeDemoSelected( -1 ),
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

	memset(m_DPad,0,4u);
	m_DPadOnce = false;
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

	addDemo( DEMO_COMMON, demo::DemoElement( u"Basic initialization", u"Full code located in GoSTDir/Demo/Samples/BasicInitialization.cpp" ) );
	addDemo( DEMO_COMMON, demo::DemoElement( u"Time", u"time" ) );
	
	addDemo( DEMO_GAME_OBJECTS, demo::DemoElement( u"Hierarchy", u"Parent-child" ) );
	addDemo( DEMO_GAME_OBJECTS, demo::DemoElement( u"Camera", u"Parent-child" ) );
	addDemo( DEMO_GAME_OBJECTS, demo::DemoElement( u"Sprite", u"Parent-child" ) );
	
	addDemo( DEMO_GUI, demo::DemoElement( u"FPS", u"Simple way to show FPS" ) );
	
	addDemo( DEMO_OTHER, demo::DemoElement( u"Supported formats", u"Get supported formats" ) );

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

void demo::DemoApplication::rebuildMainMenuFirstColon( void ){
	v4i r = m_welcomeText->getRect();
	u32 top = r.w+50;
	for( u32 i = 0u; i < 12u; ++i ){
		v4i rc;
		rc.x = 10;
		rc.y = top;
		rc.z = 200;

		m_leftColonEntity[ i ] = m_guiSystem->createTextField( rc, m_mainFont.data(), false );
		m_leftColonEntity[ i ]->setText( m_stringArray[ m_languageID ].m_stringArray[ i + 1u ] );
		m_leftColonEntity[ i ]->getBackgroundShape()->setOpacity( 0.f );
	//	m_leftColonEntity[ i ]->getBackgroundShape()->setColor( gtColorAqua );

		top = m_leftColonEntity[ i ]->getRect().w-1;
	}

	m_leftColorShape = m_guiSystem->createShapeRectangle( v4i( 8, r.w + 48, 202, top + 2 ), gtColorGrey );
	m_leftColorShape->setOpacity( 0.f );

	updateLeftColon();
}

void demo::DemoApplication::rebuildMainMenuSecondColon( void ){
}

bool demo::DemoApplication::rebuildMainMenu( void ){
	v4i wndrc = m_mainWindow->getRect();

	f32 wndH = (f32)wndrc.getHeight();
	f32 wndW = (f32)wndrc.getWidth();

	v4i bgrc;
	if( m_backgroundTexture ){
		f32 txH = (f32)m_backgroundTexture->getHeight();
		f32 txW = (f32)m_backgroundTexture->getWidth();

		bgrc.z = wndW;
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
	gprc.x = wndW - 64;
	gprc.y = wndH - 41;
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

	m_welcomeText = m_guiSystem->createTextField( v4i( 20, 0, wndrc.getWidth()-20, 0 ), m_mainFont.data(), false );
	m_welcomeText->setText( m_stringArray[ m_languageID ].m_stringArray[ 0u ] );
	m_welcomeText->setOpacity( 0.9f );
	m_welcomeText->getBackgroundShape()->setOpacity( 0.f );

	rebuildMainMenuFirstColon();
	rebuildMainMenuSecondColon();

	return true;
}

void demo::DemoApplication::Run( void ){

	m_mainSystem->setTimer( 300 );

	while( m_mainSystem->update() ){

		if( m_gamepad ){
			m_gamepad->poll();
			m_DPad[ 0u ] = m_gamepad->m_POV1 == 0 || m_gamepad->m_lY == 0;
			m_DPad[ 1u ] = m_gamepad->m_POV1 == 9000;
			m_DPad[ 2u ] = m_gamepad->m_POV1 == 18000 || m_gamepad->m_lY == 0xffff;
			m_DPad[ 3u ] = m_gamepad->m_POV1 == 27000;
			printf( "%i\n", m_gamepad->m_lY );
			if( m_gamepad->m_POV1 == -1 && m_gamepad->m_lY == 32767 )
				m_DPadOnce = false;
		}

		switch( m_state ){
		case demo::DemoState::MainMenu:
			inputMainMenu();
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

	m_leftColorShape->render();
	for( u32 i = 0u; i < 12u; ++i ){
		m_leftColonEntity[ i ]->render();
	}

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

void demo::DemoApplication::addDemo( u32 index, const demo::DemoElement& element ){
	m_demoArrays[ index ].push_back( element );
}

void demo::DemoApplication::updateLeftColon( void ){
	for( u32 i = 0u; i < 12u; ++i ){
		m_leftColonEntity[ i ]->getBackgroundShape()->setOpacity( 0.f );
		m_leftColonEntity[ i ]->setBackgroundColor( gtColorBlack );
		m_leftColonEntity[ i ]->setTextColor( gtColorLightGray );
	}

	if( !m_activeDemoType ){
		m_leftColorShape->setOpacity( 0.125f );
		m_leftColonEntity[ m_activeDemoTypeSelected ]->getBackgroundShape()->setOpacity( 1.f );
		m_leftColonEntity[ m_activeDemoTypeSelected ]->setBackgroundColor( gtColorLightGray );
		m_leftColonEntity[ m_activeDemoTypeSelected ]->setTextColor( gtColorBlack );
	}else{
		m_leftColorShape->setOpacity( 0.f );
	}
}

void demo::DemoApplication::inputMainMenu( void ){
	if( m_mainSystem->isKeyPressed( gtKey::K_ESCAPE ) ){
		m_mainSystem->shutdown();
	}

	if( m_eventConsumer->keyDown( gtKey::K_UP ) || inputGamepadMainMenuUp() ){
		--m_activeDemoTypeSelected;
		if( m_activeDemoTypeSelected == -1 )
			m_activeDemoTypeSelected = 11;
		updateLeftColon();
	}

	if( m_eventConsumer->keyDown( gtKey::K_DOWN ) || inputGamepadMainMenuDown() ){
		++m_activeDemoTypeSelected;
		if( m_activeDemoTypeSelected > 11 )
			m_activeDemoTypeSelected = 0;
		updateLeftColon();
	}
}

bool demo::DemoApplication::inputGamepadMainMenuUp( void ){
	if( m_gamepad ){
		if( !m_DPad[ 0u ] ) return false;

		if( !m_DPadOnce ){
			m_DPadOnce = true;
			return m_DPad[ 0u ];
		}
	}
	return false;
}

bool demo::DemoApplication::inputGamepadMainMenuDown( void ){
	if( m_gamepad ){
		if( !m_DPad[ 2u ] ) return false;

		if( !m_DPadOnce ){
			m_DPadOnce = true;
			return m_DPad[ 2u ];
		}
	}
	return false;
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