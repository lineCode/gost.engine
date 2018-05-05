#include "creator.h"

#include "examples\Get_supported_import_formats.hpp"

demo::DemoApplication::DemoApplication( void ):
m_guiSystem( nullptr ),
m_gamepad( nullptr ),
m_backgroundTexture( nullptr ),
m_gamepadTexture( nullptr ),
m_pauseMainMenuSelectedId( 0 ),
m_isPause( false ),
m_languageID( 0u ),
m_activeDemoType( 0 ),
m_activeDemoTypeSelected( 0 ),
m_activeDemoSelected( 0 ),
m_rightColonFirstID( 0 ),
m_currentDemoColonIndex( 0 ),
m_state( DemoState::MainMenu ),
m_delta( 0.f ){

	demo::DemoApplicationContext context;
	context.app = this;
	m_eventConsumer = new demo::DemoApplicationEventConsumer( context );

#ifdef GT_PLATFORM_WIN32

	m_params.m_device_type = gtDeviceType::Windows;

	m_outputWindow = gtPtrNew<gtOutputWindow>( new DemoApplicationOutputWindow );
	m_params.m_outputWindow = m_outputWindow.data();
	m_params.m_outputWindow->init();
	m_params.m_outputWindow->addRef();
	 
#elif GT_PLATFORM_LINUX

	m_params.m_device_type = gtDeviceType::Linux;

#endif

	m_params.m_consumer		= m_eventConsumer;

	memset(m_DPad,0,4u);
	memset(m_gamepadButtons,0,32u);
	m_DPadOnce = false;
}

demo::DemoApplication::~DemoApplication( void ){
	for( u32 i = 0u; i < m_demoArrays->size(); ++i ){
		for( u32 o = 0u; o < m_demoArrays[i].size(); ++i ){
			m_demoArrays[ i ][ o ].clear();
		}
	}

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

	initAudio();
	
	m_gamepadSystem	=	m_mainSystem->createGameContoller( GT_UID_INPUT_DINPUT );

	addDemo( DEMO_COMMON, demo::DemoElement( getString( u"14" ), getString( u"15" ) ) );

	addDemo( DEMO_OTHER, demo::DemoElement( getString( u"20" ), getString( u"21" ), true, new DemoExample_GetSupportedImportFormats ) );
	
	return true;
}

void demo::DemoApplication::initAudio( void ){
	m_audioSystem = m_mainSystem->createAudioSystem( GT_UID_AUDIO_XADUDIO2 );
	if( m_audioSystem.data() ){
		m_audioSelect = m_audioSystem->createAudioObject( u"../demo/media/select.ogg" );
		m_audioAccept = m_audioSystem->createAudioObject( u"../demo/media/accept.ogg" );
		m_audioCancel = m_audioSystem->createAudioObject( u"../demo/media/cancel.ogg" );
	}
}

void demo::DemoApplication::playAudio( DemoAudioType type ){
	switch( type ){
	case demo::DemoAudioType::Select:
		if( m_audioSelect.data() ){
			m_audioSelect->play();
		}
		break;
	case demo::DemoAudioType::Accept:
		if( m_audioAccept.data() ){
			m_audioAccept->play();
		}
		break;
	case demo::DemoAudioType::Cancel:
		if( m_audioCancel.data() ){
			m_audioCancel->play();
		}
		break;
	}
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

	auto rc = m_mainWindow->getRect();
	auto w  = rc.getWidth();
	auto h  = rc.getHeight();
	auto centerx = w / 2;
	auto centery = h / 2;

	m_pauseBackgroundShape = m_guiSystem->createShapeRectangle( v4i(0,0,w,h), gtColorBlack );
	m_pauseBackgroundShape->setOpacity( 0.f );

	m_pauseTextContinueShape = m_guiSystem->createTextField( v4i(centerx-48,centery-98,centerx+48,0), m_mainFont.data(), false );
	m_pauseTextContinueShape->setText( getString( u"17" ) );
	m_pauseTextSettingsShape = m_guiSystem->createTextField( v4i(centerx-48,m_pauseTextContinueShape->getRect().w,centerx+48,0), m_mainFont.data(), false );
	m_pauseTextSettingsShape->setText( getString( u"19" ) );
	m_pauseTextExitShape = m_guiSystem->createTextField( v4i(centerx-48,m_pauseTextSettingsShape->getRect().w,centerx+48,0), m_mainFont.data(), false );
	m_pauseTextExitShape->setText( getString( u"18" ) );

	m_pauseTextSettingsShape->setBackgroundColor( gtColorLightGray );
	m_pauseTextSettingsShape->setTextColor( gtColorBlack );
	m_pauseTextExitShape->setBackgroundColor( gtColorLightGray );
	m_pauseTextExitShape->setTextColor( gtColorBlack );

	m_pauseShape = m_guiSystem->createShapeRectangle( v4i(centerx-50,centery-100,centerx+50,m_pauseTextExitShape->getRect().w), gtColorLightGray );
	m_pauseShape->setOpacity( 0.f );

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

											gtString a;
											if( nodes[ i2 ]->attributeList.size() ){
												a = nodes[ i2 ]->attributeList[ 0 ]->value;
											}

											m_stringArray[ dlcount ].m_stringArray.push_back( gtPair<gtString,gtString>(a,nodes[ i2 ]->text) );
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

const gtString& demo::DemoApplication::getString( const gtString& a ){
	u32 sz = m_stringArray[ m_languageID ].m_stringArray.size();
	for( u32 i = 0u; i < sz; ++i ){
		if( m_stringArray[ m_languageID ].m_stringArray[ i ].m_first == a ){
			return m_stringArray[ m_languageID ].m_stringArray[ i ].m_second;
		}
	}
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

void demo::DemoApplication::rebuildMainMenuColons( void ){

	if( !m_welcomeText.data() ) return;

	v4i r = m_welcomeText->getRect();
	u32 top = r.w+50;
	for( u32 i = 0u; i < DEMO_TYPE_NUM; ++i ){
		v4i rc;
		rc.x = 10;
		rc.y = top;
		rc.z = 200;

		m_leftColonEntity[ i ] = m_guiSystem->createTextField( rc, m_mainFont.data(), false );
		m_leftColonEntity[ i ]->setText( m_stringArray[ m_languageID ].m_stringArray[ i + 1u ].m_second );
		m_leftColonEntity[ i ]->getBackgroundShape()->setOpacity( 0.f );

		top = m_leftColonEntity[ i ]->getRect().w-1;
	}

	m_rightColonDefaultRect = v4i( 8, r.w + 48, 202, top + 2 );

	m_leftColonShape = m_guiSystem->createShapeRectangle( m_rightColonDefaultRect, gtColorGrey );
	m_leftColonShape->setOpacity( 0.f );

	m_rightColonDefaultRect.x = 208;
	m_rightColonDefaultRect.z = 502;
	m_rightColonShape = m_guiSystem->createShapeRectangle( m_rightColonDefaultRect, gtColorGrey );
	m_rightColonShape->setOpacity( 0.f );

	m_rightColonDefaultText = m_guiSystem->createTextField( m_rightColonDefaultRect, m_mainFont.data(), false );
	m_rightColonDefaultText->setText( getString( u"16" ) );
	m_rightColonDefaultText->setTextColor( gtColorLightGray );

	top = r.w+50;
	for( u32 i = 0u; i < 24u; ++i ){
		v4i rc;
		rc.x = 210;
		rc.y = top;
		rc.z = 500;

		m_rightColonEntity[ i ] = m_guiSystem->createTextField( rc, m_mainFont.data(), false );
		m_rightColonEntity[ i ]->setText( u" " );
		m_rightColonEntity[ i ]->getBackgroundShape()->setOpacity( 0.f );

		top = m_rightColonEntity[ i ]->getRect().w-1;
	}

	r = m_rightColonEntity[ 0 ]->getRect();
	
	v4i wndrc = m_mainWindow->getRect();

	
	m_descriptionRect.x = r.z;
	m_descriptionRect.y = r.y;
	m_descriptionRect.z = wndrc.getWidth()-10;
	m_descriptionRect.w = wndrc.getHeight()-10;

	m_descriptionBackgroundShape = m_guiSystem->createShapeRectangle( m_descriptionRect, gtColorGrey );

	updateColons();
}


bool demo::DemoApplication::rebuildMainMenu( void ){

	if( !m_driver ) return true;

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

	if( m_mainFont.data() ){
		m_welcomeText = m_guiSystem->createTextField( v4i( 20, 0, wndrc.getWidth()-20, 0 ), m_mainFont.data(), false );
		m_welcomeText->setText( getString( u"0" ) );
		m_welcomeText->setOpacity( 0.9f );
		m_welcomeText->getBackgroundShape()->setOpacity( 0.f );
	}


	rebuildMainMenuColons();

	return true;
}

void demo::DemoApplication::updatePauseMainMenu( void ){

	if( m_pauseMainMenuSelectedId < 0 )
		m_pauseMainMenuSelectedId = 2;
	else if( m_pauseMainMenuSelectedId == 3 )
		m_pauseMainMenuSelectedId = 0;

	if( m_pauseMainMenuSelectedId == 0 ){
		m_pauseTextContinueShape->setBackgroundColor( gtColorBlack );
		m_pauseTextContinueShape->setTextColor( gtColorLightGray );
	}else{
		m_pauseTextContinueShape->setBackgroundColor( gtColorLightGray );
		m_pauseTextContinueShape->setTextColor( gtColorBlack );
	}

	if( m_pauseMainMenuSelectedId == 1 ){
		m_pauseTextSettingsShape->setBackgroundColor( gtColorBlack );
		m_pauseTextSettingsShape->setTextColor( gtColorLightGray );
	}else{
		m_pauseTextSettingsShape->setBackgroundColor( gtColorLightGray );
		m_pauseTextSettingsShape->setTextColor( gtColorBlack );
	}

	if( m_pauseMainMenuSelectedId == 2 ){
		m_pauseTextExitShape->setBackgroundColor( gtColorBlack );
		m_pauseTextExitShape->setTextColor( gtColorLightGray );
	}else{
		m_pauseTextExitShape->setBackgroundColor( gtColorLightGray );
		m_pauseTextExitShape->setTextColor( gtColorBlack );
	}

}

void demo::DemoApplication::Run( void ){
	m_mainSystem->setTimer( 300 );

	u32 last = 0u;
	u32 now = 0u;

	f32 timer_input = 0.f;
	f32 timer_input_limit_first = 0.4f;
	f32 timer_input_limit_second = 0.05f;
	f32 timer_input_limit = timer_input_limit_first;

	while( m_mainSystem->update() ){

		now = m_mainSystem->getTime();
		
		m_delta = f32(now - last)*0.001f;
		
		timer_input += m_delta;

		if( m_gamepad ){
			m_gamepad->poll();
			m_DPad[ 0u ] = m_gamepad->m_POV1 == 0 || m_gamepad->m_lY == -1000;
			m_DPad[ 1u ] = m_gamepad->m_POV1 == 9000 || m_gamepad->m_lX == 1000;
			m_DPad[ 2u ] = m_gamepad->m_POV1 == 18000 || m_gamepad->m_lY == 1000;
			m_DPad[ 3u ] = m_gamepad->m_POV1 == 27000 || m_gamepad->m_lX == -1000;
			if( m_gamepad->m_POV1 == -1 && (m_gamepad->m_lY > -10 && m_gamepad->m_lY < 10) ){
				timer_input = 0.f;
				timer_input_limit = timer_input_limit_first;
				m_DPadOnce = false;
			}
			for( u32 i = 0u; i < 32u; ++i ){
				if( !m_gamepad->m_buttons[ i ] ){
					m_gamepadButtons[ i ] = false;
				}
			}
		}


		switch( m_state ){
		case demo::DemoState::MainMenu:
			if( m_isPause ){
				auto opBG = m_pauseBackgroundShape->getOpacity();
				if( opBG < 0.75f ){
					opBG += 10.f * m_delta;
					m_pauseBackgroundShape->setOpacity( opBG );
				}

				auto op = m_pauseShape->getOpacity();
				if( op < 1.f ){
					op += 10.f * m_delta;
					m_pauseShape->setOpacity( op );
				}

				inputMainMenuPause();
			}else{
				if( m_pauseBackgroundShape ){
					auto opBG = m_pauseBackgroundShape->getOpacity();
					if( opBG > 0.f ){
						opBG -= 10.f * m_delta;
						m_pauseBackgroundShape->setOpacity( opBG );
					}
				}

				if( m_pauseShape ){
					auto op = m_pauseShape->getOpacity();
					if( op > 0.f ){
						op -= 10.f * m_delta;
						m_pauseShape->setOpacity( op );
					}
				}

				inputMainMenu();
			}
			renderMainMenu();
			if( timer_input > timer_input_limit ){
				m_DPadOnce = false;
				timer_input = 0.f;
				timer_input_limit = timer_input_limit_second;
			}
			break;
		case demo::DemoState::DemoMenu:
			break;
		case demo::DemoState::DemoRun:
			break;
		}

		last = now;
	}
}

void demo::DemoApplication::renderMainMenu( void ){

	if( !m_driver ) return;

	m_driver->beginRender();

	m_driver->setDepthState( false );

	if( m_backgroundShape )
		m_backgroundShape->render();

	if( m_gamepadiconShape )
		m_gamepadiconShape->render();

	if( m_welcomeText )
		m_welcomeText->render();

	if( m_leftColonShape ){
		m_leftColonShape->render();
		for( u32 i = 0u; i < DEMO_TYPE_NUM; ++i ){
			m_leftColonEntity[ i ]->render();
		}
	}

	if( m_rightColonShape )
		m_rightColonShape->render();

	if( m_rightColonDefaultText ){
		m_rightColonDefaultText->render();

		for( u32 i = 0u; i < 24u; ++i ){
			m_rightColonEntity[ i ]->render();
		}
	}

	if( m_descriptionBackgroundShape )
		m_descriptionBackgroundShape->render();

	if( m_description )
		m_description->render();

	if( m_pauseBackgroundShape )
		m_pauseBackgroundShape->render();

	if( m_isPause ){
		m_pauseShape->render();
		m_pauseTextContinueShape->render();
		m_pauseTextSettingsShape->render();
		m_pauseTextExitShape->render();
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

void demo::DemoApplication::updateColons( void ){

	if( !m_driver ) return;

	for( u32 i = 0u; i < DEMO_TYPE_NUM; ++i ){
		m_leftColonEntity[ i ]->getBackgroundShape()->setOpacity( 0.f );
		m_leftColonEntity[ i ]->setBackgroundColor( gtColorBlack );
		m_leftColonEntity[ i ]->setTextColor( gtColorLightGray );
	}
	for( u32 i = 0u; i < 24u; ++i ){
		m_rightColonEntity[ i ]->setOpacity( 0.f );
		m_rightColonEntity[ i ]->setBackgroundColor( gtColorBlack );
		m_rightColonEntity[ i ]->setTextColor( gtColorLightGray );
	}

	if( !m_activeDemoType ){//left

		m_descriptionBackgroundShape->setOpacity( 0.f );
		m_rightColonShape->setOpacity( 0.f );
		m_leftColonShape->setOpacity( 0.15f );
		m_leftColonEntity[ m_activeDemoTypeSelected ]->getBackgroundShape()->setOpacity( 1.f );
		m_leftColonEntity[ m_activeDemoTypeSelected ]->setBackgroundColor( gtColorLightGray );
		m_leftColonEntity[ m_activeDemoTypeSelected ]->setTextColor( gtColorBlack );

		if( m_demoArrays[ m_activeDemoTypeSelected ].size() ){ 
			m_rightColonDefaultText->setOpacity( 0.f );

			v4i r = m_welcomeText->getRect();
			u32 top = r.w+50;
			u32 sz = m_demoArrays[ m_activeDemoTypeSelected ].size();
			for( u32 i = 0u; i < sz; ++i ){


				u32 actualIndex = i;
				if( actualIndex > 23u ) break;

				v4i rc;
				rc.x = 210;
				rc.y = top;
				rc.z = 500;

				m_rightColonEntity[ actualIndex ] = m_guiSystem->createTextField( rc, m_mainFont.data(), false );
				m_rightColonEntity[ actualIndex ]->setFont( m_mainFont.data() );

				m_rightColonEntity[ actualIndex ]->setText( m_demoArrays[m_activeDemoTypeSelected][m_rightColonFirstID+actualIndex].GetTitle() );

				m_rightColonEntity[ actualIndex ]->getBackgroundShape()->setOpacity( 0.f );

				top = m_rightColonEntity[ actualIndex ]->getRect().w-1;
			}

		}else{
			m_rightColonDefaultText->setOpacity( 1.f );
			m_rightColonDefaultText->getBackgroundShape()->setOpacity( 0.f );
		}
		
		m_description = m_guiSystem->createTextField( v4i( 0, 0, 0, 0 ), m_mainFont.data(), false );

	}else{//right

		if( m_demoArrays[ m_activeDemoTypeSelected ].size() ){

			m_descriptionBackgroundShape->setOpacity( 0.85f );

			v4i rc;
			rc.x = m_rightColonEntity[ 0u ]->getRect().x;
			rc.y = m_rightColonEntity[ 0u ]->getRect().y;
			u32 sz = m_demoArrays[ m_activeDemoTypeSelected ].size();

			if( m_activeDemoSelected == sz ){
				m_activeDemoSelected = 0;
				m_rightColonFirstID = 0;
				m_currentDemoColonIndex = 0;
			}else if( m_activeDemoSelected < 0 ){
				m_activeDemoSelected = sz - 1;
			}

			if( m_currentDemoColonIndex < 0 ){
				if( m_activeDemoSelected == sz - 1 ){
					if( sz > 23 ){
						m_currentDemoColonIndex = 23;
						m_rightColonFirstID = m_activeDemoSelected - 23;
					}else{
						m_currentDemoColonIndex = 0;
					}
				}else{
					m_currentDemoColonIndex = 0;
					--m_rightColonFirstID;
				}
			}else if( m_currentDemoColonIndex == 24 ){

				if( m_activeDemoSelected > 23 ){
					++m_rightColonFirstID;
					m_currentDemoColonIndex = 23;
				}else{
					m_currentDemoColonIndex = 0;
				}

			}

			for( u32 i = 0u; i < 24u; ++i ){
				if( i == sz ) break;

				m_rightColonEntity[ i ]->setText( 
					m_demoArrays[m_activeDemoTypeSelected][ m_rightColonFirstID + i ].GetTitle() );
			}

			for( u32 i = 0u; i < sz; ++i ){
				if( i > 23u ) break;

				m_rightColonEntity[ i ]->setOpacity( 1.f );
				m_rightColonEntity[ i ]->getBackgroundShape()->setOpacity( 0.f );
				m_rightColonEntity[ i ]->setBackgroundColor( gtColorBlack );
				m_rightColonEntity[ i ]->setTextColor( gtColorLightGray );

				rc.z = m_rightColonEntity[ i ]->getRect().z;
				rc.w = m_rightColonEntity[ i ]->getRect().w;
			}

			if( rc.w < m_rightColonDefaultRect.w )
				rc.w = m_rightColonDefaultRect.w;

			m_rightColonShape = m_guiSystem->createShapeRectangle( rc, gtColorGrey );

			m_rightColonEntity[ m_currentDemoColonIndex ]->getBackgroundShape()->setOpacity( 1.f );
			m_rightColonEntity[ m_currentDemoColonIndex ]->setBackgroundColor( gtColorLightGray );
			m_rightColonEntity[ m_currentDemoColonIndex ]->setTextColor( gtColorBlack );

			m_rightColonDefaultText->setOpacity( 0.f );
			
			m_description = m_guiSystem->createTextField( m_descriptionRect, m_mainFont.data(), false );
			m_description->setText( m_demoArrays[m_activeDemoTypeSelected][m_activeDemoSelected].GetDesc() );
			m_description->setTextColor( gtColorBlack );
			m_description->getBackgroundShape()->setOpacity( 0.f );
		}else{
			m_rightColonShape = m_guiSystem->createShapeRectangle( m_rightColonDefaultRect, gtColorGrey );
			m_rightColonDefaultText->setOpacity( 1.f );
			m_rightColonDefaultText->getBackgroundShape()->setOpacity( 0.f );
			m_description = m_guiSystem->createTextField( v4i( 0, 0, 0, 0 ), m_mainFont.data(), false );
		}

		m_leftColonShape->setOpacity( 0.f );
		m_rightColonShape->setOpacity( 0.15f );

	}
}

void demo::DemoApplication::inputMainMenuPause( void ){
	if( m_eventConsumer->keyDown( gtKey::K_ESCAPE ) || inputGamepadMainMenuStart() || inputGamepadMainMenuEscape() ){
		playAudio(DemoAudioType::Cancel);
		m_isPause = false;
		m_pauseMainMenuSelectedId = 0;
		updatePauseMainMenu();
	}

	if( m_eventConsumer->keyDown( gtKey::K_UP ) || inputGamepadMainMenuUp() ){
		playAudio(DemoAudioType::Select);
		--m_pauseMainMenuSelectedId;
		updatePauseMainMenu();
	}

	if( m_eventConsumer->keyDown( gtKey::K_DOWN ) || inputGamepadMainMenuDown() ){
		playAudio(DemoAudioType::Select);
		++m_pauseMainMenuSelectedId;
		updatePauseMainMenu();
	}

	if( m_eventConsumer->keyDown( gtKey::K_ENTER ) || inputGamepadMainMenuEnter() ){
		playAudio(DemoAudioType::Accept);
		if( m_pauseMainMenuSelectedId == 0 ){
			m_isPause = false;
		}else if( m_pauseMainMenuSelectedId == 1 ){
		}else if( m_pauseMainMenuSelectedId == 2 ){
			m_mainSystem->shutdown();
		}
	}
}

void demo::DemoApplication::inputMainMenu( void ){
	if( m_eventConsumer->keyDown( gtKey::K_ESCAPE ) ){
		if( !m_activeDemoType )
			m_isPause = true;
		else{
			--m_activeDemoType;
			updateColons();
		}
		playAudio(DemoAudioType::Cancel);
	}

	if( inputGamepadMainMenuStart() ){
		m_isPause = true;
		updatePauseMainMenu();
		playAudio(DemoAudioType::Cancel);
	}

	if( inputGamepadMainMenuEscape() ){
		if( m_activeDemoType ){
			--m_activeDemoType;
			updateColons();
			playAudio(DemoAudioType::Cancel);
		}
	}

	if( m_eventConsumer->keyDown( gtKey::K_ENTER ) || inputGamepadMainMenuEnter() ){
		if( !m_activeDemoType ){
			++m_activeDemoType;
			updateColons();
			playAudio(DemoAudioType::Accept);
		}else{
			if( m_demoArrays[m_activeDemoTypeSelected][m_activeDemoSelected].isDemo() ){
				m_demoArrays[m_activeDemoTypeSelected][m_activeDemoSelected].Run();
				playAudio(DemoAudioType::Accept);
			}
		}
	}

	if( m_eventConsumer->keyDown( gtKey::K_UP ) || inputGamepadMainMenuUp() ){
		if( !m_activeDemoType ){
			--m_activeDemoTypeSelected;
			if( m_activeDemoTypeSelected == -1 )
				m_activeDemoTypeSelected = 12;

			m_currentDemoColonIndex = m_activeDemoSelected = 0;
			m_rightColonFirstID = 0;
		}else{// right colon in focus
			--m_activeDemoSelected;
			--m_currentDemoColonIndex;
		}
		updateColons();
		playAudio(DemoAudioType::Select);
	}

	if( m_eventConsumer->keyDown( gtKey::K_DOWN ) || inputGamepadMainMenuDown() ){
		if( !m_activeDemoType ){
			++m_activeDemoTypeSelected;
			if( m_activeDemoTypeSelected > 12 )
				m_activeDemoTypeSelected = 0;

			m_currentDemoColonIndex = m_activeDemoSelected = 0;
			m_rightColonFirstID = 0;
		}else{// right colon in focus
			++m_activeDemoSelected;
			++m_currentDemoColonIndex;
		}
		playAudio(DemoAudioType::Select);
		updateColons();
	}

	if( m_eventConsumer->keyDown( gtKey::K_LEFT ) || inputGamepadMainMenuLeft() ){
		if( m_activeDemoType ){
			--m_activeDemoType;
			playAudio(DemoAudioType::Select);
			updateColons();
		}
	}
	if( m_eventConsumer->keyDown( gtKey::K_RIGHT ) || inputGamepadMainMenuRight() ){
		if( !m_activeDemoType ){
			++m_activeDemoType;
			playAudio(DemoAudioType::Select);
			updateColons();
		}
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

bool demo::DemoApplication::inputGamepadMainMenuRight( void ){
	if( m_gamepad ){
		if( !m_DPad[ 1u ] ) return false;

		if( !m_DPadOnce ){
			m_DPadOnce = true;
			return m_DPad[ 1u ];
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

bool demo::DemoApplication::inputGamepadMainMenuLeft( void ){
	if( m_gamepad ){
		if( !m_DPad[ 3u ] ) return false;

		if( !m_DPadOnce ){
			m_DPadOnce = true;
			return m_DPad[ 3u ];
		}
	}
	return false;
}

// cross
bool demo::DemoApplication::inputGamepadMainMenuEnter( void ){
	if( m_gamepad ){
		if( m_gamepad->m_buttons[ 2 ] ){
			if( !m_gamepadButtons[ 2 ] ){
				m_gamepadButtons[ 2 ] = true;
				return true;
			}
		}
	}
	return false;
}

// triangle
bool demo::DemoApplication::inputGamepadMainMenuEscape( void ){
	if( m_gamepad ){
		if( m_gamepad->m_buttons[ 0 ] ){
			if( !m_gamepadButtons[ 0 ] ){
				m_gamepadButtons[ 0 ] = true;
				return true;
			}
		}
	}
	return false;
}

bool demo::DemoApplication::inputGamepadMainMenuStart( void ){
	if( m_gamepad ){
		if( m_gamepad->m_buttons[ 9 ] ){
			if( !m_gamepadButtons[ 9 ] ){
				m_gamepadButtons[ 9 ] = true;
				return true;
			}
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