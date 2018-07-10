#include "creator.h"

#include "examples\Get_supported_import_formats.hpp"
#include "examples\Camera.hpp"
#include "examples\RTT.hpp"
#include "examples\phy_init.hpp"

void RedrawWindow(){
	auto This = demo::DemoApplication::GetThis();
	if( This ){
		This->Render();
	}
}

void ResizeWindow(){
	auto This = demo::DemoApplication::GetThis();
	if( This ){ 
		auto ms = This->GetMainSystem();
		auto ss = ms->getSceneSystem(nullptr);
		auto gs = ms->getMainVideoDriver();
		auto c  = ss->getActiveCamera();
		auto w  = gs->getParams().m_outWindow;
		auto r  = w->getClientRect();
		c->setAspect( (f32)r.getWidth() / (f32)r.getHeight() );
	}
	RedrawWindow();
}

demo::DemoApplication*	demo::DemoApplication::GetThis(){
	return s_this;
}

demo::DemoApplication* demo::DemoApplication::s_this = nullptr;

demo::DemoApplication::DemoApplication():
m_guiSystem( nullptr ),
m_gamepad( nullptr ),
m_backgroundTexture( nullptr ),
m_gamepadTexture( nullptr ),
m_pauseMainMenuSelectedId( 0 ),
m_isPause( false ),
m_isSettings( false ),
m_useSound( true ),
m_showDescription( true ),
m_sceneInitialized( false ),
m_settingsTypeID( 0 ),
m_demoPauseMenuID( 0 ),
m_demoClearColor( 0xff6BB5FF ),
m_languageID( gtConst0U ),
m_activeDemoType( 0 ),
m_activeDemoTypeSelected( 0 ),
m_activeDemoSelected( 0 ),
m_rightColonFirstID( 0 ),
m_currentDemoColonIndex( 0 ),
m_state( DemoState::MainMenu ),
m_delta( 0.f )
{
	
	memset( m_rightColonEntity, 0, sizeof(gtPtr<gtGUITextField>) * 24u );

	demo::DemoApplicationContext context;
	context.app = this;
	context.activeDemoTypeSelected = &m_activeDemoTypeSelected;
	context.activeDemoType = &m_activeDemoType;
	context.activeDemoSelected = &m_activeDemoSelected;
	context.currentDemoColonIndex = &m_currentDemoColonIndex;
	context.pauseMainMenuSelectedId = &m_pauseMainMenuSelectedId;
	context.demoState = &m_state;
	context.demoPauseMenuID = &m_demoPauseMenuID;

	m_eventConsumer = new demo::DemoApplicationEventConsumer( context );

#ifdef GT_PLATFORM_WIN32

	m_params.m_device_type = gtDeviceType::Windows;

//	m_outputWindow = gtPtrNew<gtOutputWindow>( new DemoApplicationOutputWindow );
//	m_params.m_outputWindow = m_outputWindow.data();
//	m_params.m_outputWindow->init();
	 
#elif GT_PLATFORM_LINUX

	m_params.m_device_type = gtDeviceType::Linux;

#endif

	m_params.m_consumer		= m_eventConsumer;

	memset(m_DPad,0,gtConst4U);
	memset(m_gamepadButtons,0,32u);
	m_DPadOnce = false;

	m_xmlPath = u"../demo/media/settings.xml";

	demo::DemoApplication::s_this = this;

}

demo::DemoApplication::~DemoApplication(){
	for( u32 i = gtConst0U; i < m_demoArrays->size(); ++i ){
		for( u32 o = gtConst0U; o < m_demoArrays[i].size(); ++i ){
			m_demoArrays[ i ][ o ].clear();
		}
	}
	delete m_eventConsumer;
}

gtMainSystem	*	demo::DemoApplication::GetMainSystem(){
	return m_mainSystem.data();
}

demo::DemoApplicationEventConsumer	*	demo::DemoApplication::GetEventConsumer(){
	return m_eventConsumer;
}

bool demo::DemoApplication::Init(){
	if( !initEngine() )
		return false;

	if( !initStrings() )
		return false;

	xmlLoadSettings();

	if( !initWindow() )
		return false;

	if( !initVideoDriver() )
		return false;

	if( !initMainMenu() )
		return false;


	initAudio();

	m_sceneSystem	=   m_mainSystem->getSceneSystem( m_gs.data() );
	m_gamepadSystem	=	m_mainSystem->getInputSystem()->createInputContoller( GT_UID_INPUT_DINPUT );

	addDemo( DEMO_COMMON, demo::DemoElement( u"14", u"15" ) );
	addDemo( DEMO_COMMON, demo::DemoElement( u"30", u"31", true, new DemoExample_RTT( this ) ) );
	addDemo( DEMO_GAME_OBJECTS, demo::DemoElement( u"22", u"23", true, new DemoExample_Camera( this ) ) );
	addDemo( DEMO_PHYSICS, demo::DemoElement( u"32", u"33", true, new DemoExample_phy_init( this ) ) );
	addDemo( DEMO_OTHER, demo::DemoElement( u"20", u"21", true, new DemoExample_GetSupportedImportFormats( this ) ) );

	updateDemoText();

	return true;
}

void demo::DemoApplication::initAudio(){
	m_audioSystem = m_mainSystem->createAudioSystem( GT_UID_AUDIO_XADUDIO2 );
	if( m_audioSystem.data() ){
		m_audioSelect = m_audioSystem->createAudioObject( u"../demo/media/select.ogg" );
		m_audioAccept = m_audioSystem->createAudioObject( u"../demo/media/accept.ogg" );
		m_audioCancel = m_audioSystem->createAudioObject( u"../demo/media/cancel.ogg" );
	}
}

void demo::DemoApplication::playAudio( DemoAudioType type ){
	if( !m_useSound ) return;

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

bool demo::DemoApplication::initEngine(){
	m_mainSystem = gost::InitializeGoSTEngine( m_params );
	if( !m_mainSystem )
		return false;

	m_input = m_mainSystem->getInputSystem();

	return true;
}

bool demo::DemoApplication::initWindow(){
	m_windowInfo.m_style |= gtWindowInfo::maximize;
    m_windowInfo.m_style |= gtWindowInfo::resize;
	m_windowInfo.m_style |= gtWindowInfo::center;
	//m_windowInfo.m_style = gtWindowInfo::popup;
	
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

	m_mainWindow->setOnMove( RedrawWindow );
	m_mainWindow->setOnSize( ResizeWindow );

	m_eventConsumer->m_context.oldWindowSize = m_windowInfo.m_rect;

	return true;
}

bool demo::DemoApplication::initVideoDriver(){
	m_gsInfo.m_adapterID		=	0;
	m_gsInfo.m_backBufferSize.x	=	m_windowInfo.m_rect.getWidth();
	m_gsInfo.m_backBufferSize.y	=	m_windowInfo.m_rect.getHeight();
	m_gsInfo.m_colorDepth		=	32;
	m_gsInfo.m_doubleBuffer		=	true;
	m_gsInfo.m_fullScreen		=	false;
	m_gsInfo.m_outWindow		=	m_mainWindow.data();
	m_gsInfo.m_stencilBuffer	=	true;
	m_gsInfo.m_vSync			=	false;

	m_gs = m_mainSystem->createGraphicsSystem( m_gsInfo, GT_UID_RENDER_D3D11 );
	if( !m_gs.data() )
		return false;

	m_guiSystem	=	m_mainSystem->getGUISystem( m_gs.data() );

	return true;
}

void demo::DemoApplication::showDemoHUD(){
	m_demoText_camera->setVisible( true );
}

void demo::DemoApplication::hideDemoHUD(){
	m_demoText_camera->setVisible( false );
}

void demo::DemoApplication::updateHUD(){

	auto camera = m_sceneSystem->getActiveCamera();
	if( !camera ) return;

	gtString str( u"Camera " );

	auto stra = camera->getName();
	auto pos = camera->getPosition();
	str += stra.data();
	str += u" X: ";
	str += pos.x;
	str += u" Y: ";
	str += pos.y;
	str += u" Z: ";
	str += pos.z;

	m_demoText_camera->setText( str );
}

bool demo::DemoApplication::initMainMenu(){
	gtString logoPath(u"../demo/media/logo.png");
	gtString gamepadPath(u"../demo/media/gamepad.png");

	if( gtFileSystem::existFile( logoPath ) ){
		auto image = m_mainSystem->loadImage( logoPath );
		m_backgroundTexture = m_gs->createTexture( image.data() );
		m_backgroundTexture->setName( "m_backgroundTexture" );
	}else
		gtLogWriter::printWarning( u"Can not load background texture. File %s not exist.", logoPath.data() );

	if( gtFileSystem::existFile( gamepadPath ) ){
		auto image = m_mainSystem->loadImage( gamepadPath );
		m_gamepadTexture = m_gs->createTexture( image.data() );
		//m_gamepadTexture = m_gs->getTexture( gamepadPath );
		m_gamepadTexture->setName( "m_gamepadTexture" );
	}else
		gtLogWriter::printWarning( u"Can not load gamepad icon texture. File %s not exist.", gamepadPath.data() );
	
	m_mainFont	=	m_guiSystem->createBuiltInFont();
	m_infoFont	=	m_guiSystem->createFont( u"../demo/media/fonts/Montserrat/Montserrat.xml" );

	auto rc = m_mainWindow->getRect();
	auto w  = rc.getWidth();
	auto h  = rc.getHeight();
	auto centerx = w / 2;
	auto centery = h / 2;

	m_demoText_camera = m_guiSystem->createTextField( v4i( 10, 10, 0, 0 ), m_infoFont.data(), false, false );
	m_demoText_camera->setTextColor( gtColorWhite );
	m_demoText_camera->getBackgroundShape()->setTransparent( 1.f );


	hideDemoHUD();

	m_pauseBackgroundShape = m_guiSystem->createShapeRectangle( v4i(0,0,w,h), gtColorBlack );
	m_pauseBackgroundShape->setTransparent( 1.f );

	m_pauseTextContinueShape = m_guiSystem->createTextField( v4i(centerx-48,centery-98,centerx+148,0), m_mainFont.data(), false );
	if( m_pauseTextContinueShape ){
		m_pauseTextContinueShape->setTextColor( gtColorLightGray );
		m_guiSystem->addToUserInput( m_pauseTextContinueShape.data(), DEMO_GUI_ID_PAUSE_CONTINUE );
	}

	m_pauseTextSettingsShape = m_guiSystem->createTextField( v4i(centerx-48,m_pauseTextContinueShape->getRect().w,centerx+148,0), m_mainFont.data(), false );
	if( m_pauseTextSettingsShape ){
		m_guiSystem->addToUserInput( m_pauseTextSettingsShape.data(), DEMO_GUI_ID_PAUSE_SETTINGS );
	}

	m_pauseTextMainMenuShape = m_guiSystem->createTextField( v4i(centerx-48,m_pauseTextContinueShape->getRect().w,centerx+148,0), m_mainFont.data(), false );
	if( m_pauseTextMainMenuShape ){
		m_guiSystem->addToUserInput( m_pauseTextMainMenuShape.data(), DEMO_GUI_ID_PAUSE_MAIN_MENU );
	}
	
	m_pauseTextExitShape = m_guiSystem->createTextField( v4i(centerx-48,m_pauseTextSettingsShape->getRect().w,centerx+148,0), m_mainFont.data(), false );
	if( m_pauseTextExitShape ){
		m_guiSystem->addToUserInput( m_pauseTextExitShape.data(), DEMO_GUI_ID_PAUSE_EXIT );
	}

	m_pauseTextSettingsShape->setBackgroundColor( gtColorLightGray );
	m_pauseTextSettingsShape->setTextColor( gtColorBlack );
	m_pauseTextMainMenuShape->setBackgroundColor( gtColorLightGray );
	m_pauseTextMainMenuShape->setTextColor( gtColorBlack );
	 

	m_pauseTextExitShape->setBackgroundColor( gtColorLightGray );
	m_pauseTextExitShape->setTextColor( gtColorBlack );

	m_pauseShape = m_guiSystem->createShapeRectangle( v4i(centerx-50,centery-100,centerx+150,m_pauseTextExitShape->getRect().w), gtColorLightGray );
	//m_pauseShape = m_guiSystem->createShapeRectangle( v4i( 300, 100, 600,300), gtColorLightGray );
	m_pauseShape->setTransparent( 1.f );

	m_settingsBackgroundShape = m_guiSystem->createShapeRectangle( v4i(centerx-50,centery-100,centerx+350,centery+200), gtColorLightGray );
	m_settingsBackgroundShape->setTransparent( 1.f );
	m_settingsTextLanguage = m_guiSystem->createTextField( v4i(centerx-48,centery-98,centerx+100,0), m_mainFont.data(), false );
	m_settingsTextLanguage->setBackgroundColor( gtColorBlack );
	m_settingsTextLanguage->setTextColor( gtColorLightGray );

	m_settingsTextLanguageName = m_guiSystem->createTextField( v4i(m_settingsTextLanguage->getRect().z + 50, m_settingsTextLanguage->getRect().y, centerx+348,0), m_mainFont.data(), false );
	m_settingsTextLanguageName->setBackgroundColor( gtColorLightGray );
	m_settingsTextLanguageName->setTextColor( gtColorBlack );
	 
	m_settingsTextSound = m_guiSystem->createTextField( v4i(centerx-48,m_settingsTextLanguage->getRect().w,centerx+100,0), m_mainFont.data(), false );
	m_settingsTextSound->setBackgroundColor( gtColorLightGray );
	m_settingsTextSound->setTextColor( gtColorBlack );

	m_settingsTextSoundUse = m_guiSystem->createTextField( v4i(m_settingsTextSound->getRect().z + 50,m_settingsTextSound->getRect().y,centerx+348,0), m_mainFont.data(), false );
	m_settingsTextSoundUse->setBackgroundColor( gtColorLightGray );
	m_settingsTextSoundUse->setTextColor( gtColorBlack );

	updateSettingsText();
	m_settingsTextLanguage->setTransparent( 1.f );
	m_settingsTextLanguageName->setTransparent( 1.f );
	m_settingsTextSound->setTransparent( 1.f );
	m_settingsTextSoundUse->setTransparent( 1.f );

	HideMenu();

	return rebuildMainMenu();
}

void demo::DemoApplication::HideMenu(){
	if( m_pauseTextContinueShape ){ m_pauseTextContinueShape->setVisible( false ); }
	if( m_pauseTextSettingsShape ){ m_pauseTextSettingsShape->setVisible( false ); }
	if( m_pauseTextMainMenuShape ){ m_pauseTextMainMenuShape->setVisible( false ); }
	if( m_pauseTextExitShape ){ m_pauseTextExitShape->setVisible( false ); }
}

void demo::DemoApplication::ShowMenu(){
	if( m_pauseTextContinueShape ){ m_pauseTextContinueShape->setVisible( true ); }
	if( m_pauseTextSettingsShape ){ m_pauseTextSettingsShape->setVisible( true ); }
	if( m_pauseTextMainMenuShape ){ m_pauseTextMainMenuShape->setVisible( true ); }
	if( m_pauseTextExitShape ){ m_pauseTextExitShape->setVisible( true ); }
}

bool demo::DemoApplication::initStrings(){
	gtArray<gtString> strings;
	util::getFilesFromDir( &strings, u"../demo/langs/" );
	
	DemoLang dl;
	u32 dlcount = -1;

	u32 sz = strings.size();
	for( u32 i = gtConst0U; i < sz; ++i ){
		auto ext = util::stringGetExtension( strings[ i ] );
		util::stringToLower( ext );
		if( ext == u"xml" ){

			auto xml = gtFileSystem::XMLRead( strings[ i ] );
			if( xml.data() ){

				auto root = xml->getRootNode();
				if( root ){
					
					if( root->name == u"Lang" ){

						m_stringArray.push_back( dl );
						++dlcount;

						if( root->attributeList.size() ){
							if( root->attributeList[ gtConst0U ]->name == u"n" ){
								m_stringArray[ dlcount ].m_langName = root->attributeList[ gtConst0U ]->value;

								auto nodes = xml->selectNodes( u"/Lang/t" );
								if( nodes.size() ){

									u32 nsz = nodes.size();
									for( u32 i2 = gtConst0U; i2 < nsz; ++i2 ){
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
	for( u32 i = gtConst0U; i < sz; ++i ){
		if( m_stringArray[ m_languageID ].m_stringArray[ i ].m_first == a ){
			return m_stringArray[ m_languageID ].m_stringArray[ i ].m_second;
		}
	}
	return a;
}

void demo::DemoApplication::RebuildGUI(){
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

void demo::DemoApplication::RefreshGUI(){
	if( m_isPause ){
		switch( m_state ){
		case demo::DemoState::MainMenu:
			updatePauseMainMenu();
			break;
		case demo::DemoState::DemoMenu:
			updateDemoPause();
			break;
		case demo::DemoState::DemoRun:
			break;
		}
	}else
		updateColons();
}

void demo::DemoApplication::rebuildMainMenuColons(){

	if( !m_welcomeText.data() ) return;

	v4i r = m_welcomeText->getRect();
	u32 top = r.w+50;
	for( u32 i = gtConst0U; i < DEMO_TYPE_NUM; ++i ){
		v4i rc;
		rc.x = 10;
		rc.y = top;
		rc.z = 200;

		if( m_leftColonEntity[ i ] )
			m_guiSystem->removeFromUserInput( m_leftColonEntity[ i ].data() );

		m_leftColonEntity[ i ] = m_guiSystem->createTextField( rc, m_mainFont.data(), false );

		if( m_leftColonEntity[ i ] ){
			m_leftColonEntity[ i ]->setText( m_stringArray[ m_languageID ].m_stringArray[ i + gtConst1U ].m_second );
			m_leftColonEntity[ i ]->getBackgroundShape()->setTransparent( 1.f );

			top = m_leftColonEntity[ i ]->getRect().w-1;

			m_guiSystem->addToUserInput( m_leftColonEntity[ i ].data(), DEMO_GUI_ID_LEFT_COLON + i );
		}
	}

	m_rightColonDefaultRect = v4i( 8, r.w + 48, 202, top + 2 );

	m_leftColonShape = m_guiSystem->createShapeRectangle( m_rightColonDefaultRect, gtColorGrey );
	m_leftColonShape->setTransparent( 1.f );

	m_rightColonDefaultRect.x = 208;
	m_rightColonDefaultRect.z = 502;
	m_rightColonShape = m_guiSystem->createShapeRectangle( m_rightColonDefaultRect, gtColorGrey );
	m_rightColonShape->setTransparent( 1.f );

	m_rightColonDefaultText = m_guiSystem->createTextField( m_rightColonDefaultRect, m_mainFont.data(), false );
	if( m_rightColonDefaultText ){
		m_rightColonDefaultText->setText( getString( u"16" ) );
	}
	m_rightColonDefaultText->setTextColor( gtColorLightGray );
	

	top = r.w+50;
	for( u32 i = gtConst0U; i < 24u; ++i ){
		v4i rc;
		rc.x = 210;
		rc.y = top;
		rc.z = 500;

		if( m_rightColonEntity[ i ] )
			m_guiSystem->removeFromUserInput( m_rightColonEntity[ i ].data() );

		m_rightColonEntity[ i ] = m_guiSystem->createTextField( rc, m_mainFont.data(), false );

		if( m_rightColonEntity[ i ] ){
			m_rightColonEntity[ i ]->setText( u" " );
			m_rightColonEntity[ i ]->getBackgroundShape()->setTransparent( 1.f );
			m_rightColonEntity[ i ]->setTextColor( gtColorLightGray );

			top = m_rightColonEntity[ i ]->getRect().w-1;

			m_guiSystem->addToUserInput( m_rightColonEntity[ i ].data(), DEMO_GUI_ID_RIGHT_COLON + i );
		}
	}

	r = m_rightColonEntity[ 0 ]->getRect();
	
	v4i wndrc = m_mainWindow->getRect();

	if( m_state == DemoState::MainMenu ){
		m_descriptionRect.x = r.z;
		m_descriptionRect.y = r.y;
		m_descriptionRect.z = wndrc.getWidth()-10;
		m_descriptionRect.w = wndrc.getHeight()-10;
	}else{
		m_descriptionRect.x = 0;
		m_descriptionRect.y = 0;
		m_descriptionRect.z = r.z;
		m_descriptionRect.w = r.y;
	}

	m_descriptionBackgroundShape = m_guiSystem->createShapeRectangle( m_descriptionRect, 0xFF111111 );

	updateColons();
}


bool demo::DemoApplication::rebuildMainMenu(){

	if( !m_gs ) return true;

	v4i wndrc = m_mainWindow->getClientRect();

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
		m_backgroundShape->setTexture( m_backgroundTexture.data() );
	}else{
		m_backgroundShape->setColor( gtColor( gtColorBlack ) );
	}
	m_backgroundShape->setTransparent( 0.2f );

	v4i gprc;
	gprc.x = wndW - 64;
	gprc.y = wndH - 41;
	gprc.z = gprc.x + 64;
	gprc.w = gprc.y + 41;

	gtColor gamepadColor(1.f);
	if( m_gamepad )
		gamepadColor = gtColor( gtColorGreen );
	
	m_gamepadiconShape	= m_guiSystem->createShapeRectangle( gprc, gamepadColor );
	if( m_gamepadiconShape ){
		if( m_gamepadTexture ){
			m_gamepadiconShape->setTexture( m_gamepadTexture.data() );
		}else{
			m_gamepadiconShape->setColor( gtColor( gtColorBlack ) );
		}

	}
	
	if( !m_gamepad ){
		if( m_gamepadiconShape )
			m_gamepadiconShape->setTransparent( 0.75f );
	}

	if( m_mainFont.data() ){

		if( m_welcomeText )
			m_guiSystem->removeFromUserInput( m_welcomeText.data() );

		m_welcomeText = m_guiSystem->createTextField( v4i( 20, 0, wndrc.getWidth()-20, 0 ), m_mainFont.data(), false );
		m_welcomeText->setText( getString( u"0" ) );
		m_welcomeText->setTransparent( 0.1f );
		m_welcomeText->getBackgroundShape()->setTransparent( 1.f );
		m_welcomeText->getBackgroundShape()->setColor( gtColorBlack );
		m_welcomeText->setBackgroundGradient( true, gtColorRed, gtColorDarkRed );
		m_welcomeText->setTextColor( gtColorWhite );

		m_guiSystem->addToUserInput( m_welcomeText.data(), DEMO_GUI_ID_WELCOME_TEXT );

	}

	if( m_pauseTextContinueShape ){

		auto new_windowRect = m_gs->getParams().m_outWindow->getClientRect();

		f32 H = (f32)new_windowRect.getHeight() / (f32)m_eventConsumer->m_context.oldWindowSize.getHeight();
		f32 W = (f32)new_windowRect.getWidth() / (f32)m_eventConsumer->m_context.oldWindowSize.getWidth();

		v4i area = m_pauseTextContinueShape->getActiveArea();
		area.x *= W;		area.z *= W;		area.y *= H;		area.w *= H;
		m_pauseTextContinueShape->setActiveArea( area );

		if( m_pauseTextSettingsShape ){
			area = m_pauseTextSettingsShape->getActiveArea();
			area.x *= W;		area.z *= W;		area.y *= H;		area.w *= H;
			m_pauseTextSettingsShape->setActiveArea( area );
		}

		if( m_pauseTextMainMenuShape ){
			area = m_pauseTextMainMenuShape->getActiveArea();
			area.x *= W;		area.z *= W;		area.y *= H;		area.w *= H;
			m_pauseTextMainMenuShape->setActiveArea( area );
		}

		if( m_pauseTextExitShape ){
			area = m_pauseTextExitShape->getActiveArea();
			area.x *= W;		area.z *= W;		area.y *= H;		area.w *= H;
			m_pauseTextExitShape->setActiveArea( area );
		}

		m_eventConsumer->m_context.oldWindowSize = new_windowRect;
	}


	rebuildMainMenuColons();

	return true;
}

void demo::DemoApplication::updatePauseMainMenu(){

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

void demo::DemoApplication::pauseBackgroundFadeOut(){
	auto trBG = m_pauseBackgroundShape->getTransparent();
	if( trBG > 0.25f ){

		trBG -= 10.f * m_delta;

		if( trBG < 0.25f )
			trBG = 0.25f;

		m_pauseBackgroundShape->setTransparent( trBG );
	}

	auto tr = m_pauseShape->getTransparent();
	if( tr > 0.f ){

		tr -= 10.f * m_delta;

		if( tr < 0.f )
			tr = 0.f;

		m_pauseShape->setTransparent( tr );
	}

}

void demo::DemoApplication::pauseBackgroundFadeIn(){
	auto trBG = m_pauseBackgroundShape->getTransparent();
	if( trBG < 1.f ){
		trBG += 10.f * m_delta;

		if( trBG > 1.f )
			trBG = 1.f;

		m_pauseBackgroundShape->setTransparent( trBG );
	}

	auto tr = m_pauseShape->getTransparent();
	if( tr < 1.f ){

		tr += 10.f * m_delta;

		if( tr > 1.f )
			tr = 1.f;

		m_pauseShape->setTransparent( tr );
	}else{
		if( m_state == DemoState::DemoMenu ){
			m_state = DemoState::DemoRun;
		}
	}
}

void demo::DemoApplication::Run(){
	m_mainSystem->setTimer( 300 );

	u32 last = gtConst0U;
	u32 now = gtConst0U;

	timer_input = 0.f;
	timer_input_limit_first = 0.4f;
	timer_input_limit_second = 0.05f;
	timer_input_limit = timer_input_limit_first;

	while( m_mainSystem->update() ){

		now = m_mainSystem->getTime();
		
		m_delta = f32(now - last)*0.001f;
		
		UpdateGamepad();
		
		switch( m_state ){
		case demo::DemoState::MainMenu:
			runMainMenu();			
			break;
		case demo::DemoState::DemoMenu:
			runDemoMenu();
			break;
		case demo::DemoState::DemoRun:{
			runDemo();
		}break;
		}

		if( m_mainSystem->isRun() )
			Render();

		last = now;

	}
}

void demo::DemoApplication::renderMainMenu(){

	if( !m_gs ) return;

	m_gs->beginRender();

	m_gs->setDepthState( false );

	if( m_backgroundShape )	m_backgroundShape->render();

	if( m_gamepadiconShape )
		m_gamepadiconShape->render();

	if( m_welcomeText )
		m_welcomeText->render();

	if( m_leftColonShape ){
		m_leftColonShape->render();
		for( u32 i = gtConst0U; i < DEMO_TYPE_NUM; ++i ){
			m_leftColonEntity[ i ]->render();
		}
	}

	if( m_rightColonShape )
		m_rightColonShape->render();

	if( m_rightColonDefaultText ){
		m_rightColonDefaultText->render();

		for( u32 i = gtConst0U; i < 24u; ++i ){
			m_rightColonEntity[ i ]->render();
		}
	}

	m_descriptionBackgroundShape->render();

	m_description->render();

	m_pauseBackgroundShape->render();

	if( m_isPause ){
		m_pauseShape->render();
		m_pauseTextContinueShape->render();
		m_pauseTextSettingsShape->render();
		m_pauseTextExitShape->render();
		m_settingsBackgroundShape->render();
		m_settingsTextLanguage->render();
		m_settingsTextSound->render();
		m_settingsTextLanguageName->render();
		m_settingsTextSoundUse->render();
	}

	m_gs->setDepthState();

	m_gs->endRender();
}

void demo::DemoApplication::renderDemoMenu(){
	m_gs->beginRender( true, m_demoClearColor );

	m_demoArrays[m_activeDemoTypeSelected][m_activeDemoSelected].Render();

	m_gs->setDepthState( false );
	updateHUD();
	m_pauseBackgroundShape->render();
	
	if( m_isPause ){
		m_description->render();
		m_pauseShape->render();
		m_pauseTextContinueShape->render();
		m_pauseTextExitShape->render();
		m_pauseTextMainMenuShape->render();

	}

	m_gs->setDepthState();

	m_gs->endRender();
}

const gtColor& demo::DemoApplication::GetDefaultClearColor(){
	return m_demoClearColor;
}

void demo::DemoApplication::renderDemo(){
	if( m_state == DemoState::MainMenu ) return;

	m_gs->beginRender( true, m_demoClearColor );

	m_demoArrays[m_activeDemoTypeSelected][m_activeDemoSelected].Render();

	m_gs->setDepthState( false );

	updateHUD();
	m_demoText_camera->render();

	if( m_showDescription ){
		m_pauseBackgroundShape->render();
		m_description->render();
		m_showDescription = false;
	}else{
	}
	m_demoArrays[m_activeDemoTypeSelected][m_activeDemoSelected].Render2D();
	m_gs->setDepthState();


	m_gs->endRender();
}

void demo::DemoApplication::ScanGamepads(){
	if( m_gamepadSystem.data() ){
		m_mainSystem->setTimer( 300u );
		if( !m_gamepad )
			m_gamepadSystem->update();
	}
}

void demo::DemoApplication::ActivateGamepad( bool value, gtInputDevice* g ){
	if( value ){
		if( !m_gamepad ){
			m_gamepad = g;
			if( m_gamepadiconShape ){
				m_gamepadiconShape->setColor( gtColorGreen );
				m_gamepadiconShape->setTransparent();
			}
		}
	}else{
		m_gamepad = nullptr;
		if( m_gamepadiconShape ){
			m_gamepadiconShape->setColor( gtColorWhite );
			m_gamepadiconShape->setTransparent( 0.75f );
		}
	}
}

void demo::DemoApplication::updateDemoText(){
	for( u32 i = gtConst0U; i < DEMO_TYPE_NUM; ++i ){
		u32 sz = m_demoArrays[ i ].size();
		for( u32 o = gtConst0U; o < sz; ++o ){
			m_demoArrays[ i ][ o ].SetTitle( getString( m_demoArrays[ i ][ o ].GetTitleID() ) );
			m_demoArrays[ i ][ o ].SetDesc( getString( m_demoArrays[ i ][ o ].GetDescID() ) );
		}
	}
}

void demo::DemoApplication::addDemo( u32 index, const demo::DemoElement& element ){
	m_demoArrays[ index ].push_back( element );
}

void demo::DemoApplication::updateColons(){

	if( !m_gs ) return;

	for( u32 i = gtConst0U; i < DEMO_TYPE_NUM; ++i ){
		m_leftColonEntity[ i ]->getBackgroundShape()->setTransparent( 1.f );
		m_leftColonEntity[ i ]->setBackgroundColor( gtColorBlack );
		m_leftColonEntity[ i ]->setTextColor( gtColorLightGray );
	}
	for( u32 i = gtConst0U; i < 24u; ++i ){
		m_rightColonEntity[ i ]->setTransparent( 1.f );
		m_rightColonEntity[ i ]->setBackgroundColor( gtColorBlack );
		m_rightColonEntity[ i ]->setTextColor( gtColorLightGray );
	}

	if( !m_activeDemoType ){//left

		m_descriptionBackgroundShape->setTransparent( 1.f );
		m_rightColonShape->setTransparent( 1.f );
		m_leftColonShape->setTransparent( 0.85f );
		m_leftColonEntity[ m_activeDemoTypeSelected ]->getBackgroundShape()->setTransparent( 0.f );
		m_leftColonEntity[ m_activeDemoTypeSelected ]->setBackgroundColor( gtColorLightGray );
		m_leftColonEntity[ m_activeDemoTypeSelected ]->setTextColor( gtColorBlack );

		if( m_demoArrays[ m_activeDemoTypeSelected ].size() ){ 
			m_rightColonDefaultText->setTransparent( 1.f );

			v4i r = m_welcomeText->getRect();
			u32 top = r.w+50;
			u32 sz = m_demoArrays[ m_activeDemoTypeSelected ].size();
			for( u32 i = gtConst0U; i < sz; ++i ){


				u32 actualIndex = i;
				if( actualIndex > 23u ) break;

				v4i rc;
				rc.x = 210;
				rc.y = top;
				rc.z = 500;

				if( m_rightColonEntity[ actualIndex ] )
					m_guiSystem->removeFromUserInput( m_rightColonEntity[ actualIndex ].data() );

				m_rightColonEntity[ actualIndex ] = m_guiSystem->createTextField( rc, m_mainFont.data(), false );
				m_rightColonEntity[ actualIndex ]->setFont( m_mainFont.data() );

				m_rightColonEntity[ actualIndex ]->setText( m_demoArrays[m_activeDemoTypeSelected][m_rightColonFirstID+actualIndex].GetTitle() );

				m_rightColonEntity[ actualIndex ]->getBackgroundShape()->setTransparent( 1.f );
				m_rightColonEntity[ actualIndex ]->setTextColor( gtColorLightGray );

				top = m_rightColonEntity[ actualIndex ]->getRect().w-1;

				m_guiSystem->addToUserInput( m_rightColonEntity[ actualIndex ].data(), DEMO_GUI_ID_RIGHT_COLON + actualIndex );
			}

		}else{
			m_rightColonDefaultText->setTransparent( 0.f );
			m_rightColonDefaultText->getBackgroundShape()->setTransparent( 1.f );
		}
		
		m_description = m_guiSystem->createTextField( v4i( 0, 0, 0, 0 ), m_mainFont.data(), false );

	}else{//right

		if( m_demoArrays[ m_activeDemoTypeSelected ].size() ){

			m_descriptionBackgroundShape->setTransparent( 0.85f );

			v4i rc;
			rc.x = m_rightColonEntity[ gtConst0U ]->getRect().x;
			rc.y = m_rightColonEntity[ gtConst0U ]->getRect().y;
			u32 sz = m_demoArrays[ m_activeDemoTypeSelected ].size();

			m_eventConsumer->m_context.demoArraySize = sz;

			if( m_activeDemoSelected >= (s32)sz ){
				if( sz > 23u ){
					m_activeDemoSelected = 0;
					m_rightColonFirstID = 0;
					m_currentDemoColonIndex = 0;
				}{
					m_activeDemoSelected = m_currentDemoColonIndex = sz - 1u;
				}
			}else if( m_activeDemoSelected < 0 ){
				m_activeDemoSelected = sz - 1;
			}

			if( m_currentDemoColonIndex < 0 ){
				if( m_activeDemoSelected == sz - 1 ){
					if( sz > 23 ){
						m_currentDemoColonIndex = 23;
						m_rightColonFirstID = m_activeDemoSelected - 23;
					}else{
						//m_currentDemoColonIndex = 0;
						m_currentDemoColonIndex = sz - 1u;
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

			for( u32 i = gtConst0U; i < 24u; ++i ){
				if( i == sz ) break;

				m_rightColonEntity[ i ]->setText( 
					m_demoArrays[m_activeDemoTypeSelected][ m_rightColonFirstID + i ].GetTitle() );
			}

			for( u32 i = gtConst0U; i < sz; ++i ){
				if( i > 23u ) break;

				m_rightColonEntity[ i ]->setTransparent( 0.f );
				m_rightColonEntity[ i ]->getBackgroundShape()->setTransparent( 1.f );
				m_rightColonEntity[ i ]->setBackgroundColor( gtColorBlack );
				m_rightColonEntity[ i ]->setTextColor( gtColorLightGray );

				rc.z = m_rightColonEntity[ i ]->getRect().z;
				rc.w = m_rightColonEntity[ i ]->getRect().w;
			}

			if( rc.w < m_rightColonDefaultRect.w )
				rc.w = m_rightColonDefaultRect.w;

			m_rightColonShape = m_guiSystem->createShapeRectangle( rc, gtColorGrey );

			m_rightColonEntity[ m_currentDemoColonIndex ]->getBackgroundShape()->setTransparent( 0.f );
			m_rightColonEntity[ m_currentDemoColonIndex ]->setBackgroundColor( gtColorLightGray );
			m_rightColonEntity[ m_currentDemoColonIndex ]->setTextColor( gtColorBlack );

			m_rightColonDefaultText->setTransparent( 1.f );
			
			m_description = m_guiSystem->createTextField( m_descriptionRect, m_mainFont.data(), false );

			m_description->setText( m_demoArrays[m_activeDemoTypeSelected][m_activeDemoSelected].GetDesc() );
			m_description->setTextColor( gtColorWhite );
			m_description->getBackgroundShape()->setTransparent( 1.f );
		}else{
			m_rightColonShape = m_guiSystem->createShapeRectangle( m_rightColonDefaultRect, gtColorGrey );
			m_rightColonDefaultText->setTransparent( 0.f );
			m_rightColonDefaultText->getBackgroundShape()->setTransparent( 1.f );
			m_description = m_guiSystem->createTextField( v4i( 0, 0, 0, 0 ), m_mainFont.data(), false );
		}

		m_leftColonShape->setTransparent( 1.f );
		m_rightColonShape->setTransparent( 0.85f );

	}
}

void demo::DemoApplication::inputMainMenuPause(){
	if( m_eventConsumer->keyDown( gtKey::K_ESCAPE ) || inputGamepadMainMenuStart() || inputGamepadMainMenuEscape() ){
		playAudio(DemoAudioType::Cancel);
		if( m_isSettings ){
			m_settingsBackgroundShape->setTransparent( 1.f );
			m_settingsTextLanguage->setTransparent( 1.f );
			m_settingsTextSound->setTransparent( 1.f );
			m_settingsTextSoundUse->setTransparent( 1.f);
			m_settingsTextLanguageName->setTransparent( 1.f );
			
			m_isSettings = false;
		}else{
			HideMenu();
			m_isPause = false;
			xmlSaveSettings();
			m_pauseMainMenuSelectedId = 0;
			updatePauseMainMenu();
		}
	}

	if( m_eventConsumer->keyDown( gtKey::K_UP ) || inputGamepadMainMenuUp() ){
		playAudio(DemoAudioType::Select);
		if( m_isSettings ){
			--m_settingsTypeID;
			updateSettings();
		}else{
			--m_pauseMainMenuSelectedId;
			updatePauseMainMenu();
		}
	}

	if( m_eventConsumer->keyDown( gtKey::K_DOWN ) || inputGamepadMainMenuDown() ){
		playAudio(DemoAudioType::Select);
		if( m_isSettings ){
			++m_settingsTypeID;
			updateSettings();
		}else{
			++m_pauseMainMenuSelectedId;
			updatePauseMainMenu();
		}
	}

	if( m_eventConsumer->keyDown( gtKey::K_LEFT ) || inputGamepadMainMenuLeft() ){
		playAudio(DemoAudioType::Select);
		if( m_isSettings ){
			if( m_settingsTypeID == 0 ){//lang
				--m_languageID;
				if( m_languageID < 0 ){
					m_languageID = m_stringArray.size() - gtConst1U;
				}
				m_settingsTextLanguageName->setText( m_stringArray[ m_languageID ].m_langName );
				updateDemoText();
				updateSettingsText();
				rebuildMainMenu();
			}else{
				if( m_useSound ){
					m_useSound = false;
					m_settingsTextSoundUse->setText( getString( u"28" ) );
				}else{
					m_useSound = true;
					m_settingsTextSoundUse->setText( getString( u"27" ) );
				}
			}
		}
	}

	if( m_eventConsumer->keyDown( gtKey::K_RIGHT ) || inputGamepadMainMenuRight() ){
		playAudio(DemoAudioType::Select);
		if( m_isSettings ){
			if( m_settingsTypeID == 0 ){//lang
				++m_languageID;
				if( m_languageID == m_stringArray.size() ){
					m_languageID = 0;
				}
				m_settingsTextLanguageName->setText( m_stringArray[ m_languageID ].m_langName );
				updateDemoText();
				updateSettingsText();
				rebuildMainMenu();
			}else{
				if( m_useSound ){
					m_useSound = false;
					m_settingsTextSoundUse->setText( getString( u"28" ) );
				}else{
					m_useSound = true;
					m_settingsTextSoundUse->setText( getString( u"27" ) );
				}
			}
		}
	}

	if( m_eventConsumer->keyDown( gtKey::K_ENTER ) || inputGamepadMainMenuEnter() ){
		playAudio(DemoAudioType::Accept);
		if( m_pauseMainMenuSelectedId == 0 ){
			ReturnToMainMenu();
		}else if( m_pauseMainMenuSelectedId == 1 ){
			m_settingsBackgroundShape->setTransparent(  );
			m_settingsTextLanguage->setTransparent( 0.f );
			m_settingsTextSound->setTransparent( 0.f );
			m_settingsTextSoundUse->setTransparent( 0.f );
			m_settingsTextLanguageName->setTransparent( 0.f );
			m_isSettings = true;

		}else if( m_pauseMainMenuSelectedId == 2 ){
			TerminateProgram();
		}
	}
}

void demo::DemoApplication::ReturnToMainMenu(){
	playAudio(DemoAudioType::Accept);
	switch( m_state ){
	case demo::DemoState::MainMenu:
		if( m_isPause ){
			m_isPause = false;
			HideMenu();
			xmlSaveSettings();
		}
		break;
	case demo::DemoState::DemoMenu:
		break;
	case demo::DemoState::DemoRun:
		break;
	}
}

void demo::DemoApplication::TerminateProgram(){
	playAudio(DemoAudioType::Accept);
	switch( m_state ){
	case demo::DemoState::MainMenu:
		xmlSaveSettings();
		m_mainSystem->shutdown();
		break;
	case demo::DemoState::DemoMenu:
		break;
	case demo::DemoState::DemoRun:
		break;
	}
}

void demo::DemoApplication::inputMainMenu(){
	if( m_eventConsumer->keyDown( gtKey::K_ESCAPE ) ){
		if( !m_activeDemoType ){
			m_isPause = true;
			ShowMenu();
		}else{
			--m_activeDemoType;
			updateColons();
		}
		playAudio(DemoAudioType::Cancel);
	}

	if( inputGamepadMainMenuStart() ){
		ShowMenu();
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
			if( m_demoArrays[m_activeDemoTypeSelected].size() ){
				if( m_demoArrays[m_activeDemoTypeSelected][m_activeDemoSelected].isDemo() ){
					playAudio(DemoAudioType::Accept);
					if( m_demoArrays[m_activeDemoTypeSelected][m_activeDemoSelected].Init() ){
						m_state = DemoState::DemoRun;
						showDemoHUD();
					}else{
						gtLogWriter::printWarning( u"%s", getString( u"24" ).data() );
					}
				}
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
		playAudio(DemoAudioType::Select);
		if( m_activeDemoType ){
			--m_activeDemoType;
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

bool demo::DemoApplication::inputGamepadMainMenuUp(){
	if( m_gamepad ){
		if( !m_DPadOnce ){
			if( m_DPad[ gtConst0U ] ){
				m_DPadOnce = true;
				return m_DPadOnce;
			}
		}
	}
	return false;
}

bool demo::DemoApplication::inputGamepadMainMenuRight(){
	if( m_gamepad ){
		if( !m_DPadOnce ){
			if( m_DPad[ gtConst1U ] ){
				m_DPadOnce = true;
				return m_DPadOnce;
			}
		}
	}
	return false;
}

bool demo::DemoApplication::inputGamepadMainMenuDown(){
	if( m_gamepad ){
		if( !m_DPadOnce ){
			if( m_DPad[ gtConst2U ] ){
				m_DPadOnce = true;
				return m_DPadOnce;
			}
		}
	}
	return false;
}

bool demo::DemoApplication::inputGamepadMainMenuLeft(){
	if( m_gamepad ){
		if( !m_DPadOnce ){
			if( m_DPad[ gtConst3U ] ){
				m_DPadOnce = true;
				return m_DPadOnce;
			}
		}
	}
	return false;
}

// cross
bool demo::DemoApplication::inputGamepadMainMenuEnter(){
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
bool demo::DemoApplication::inputGamepadMainMenuEscape(){
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

bool demo::DemoApplication::inputGamepadMainMenuStart(){
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

bool demo::DemoApplication::inputGamepadMainMenuSelect(){
	if( m_gamepad ){
		if( m_gamepad->m_buttons[ 8 ] ){
			if( !m_gamepadButtons[ 8 ] ){
				m_gamepadButtons[ 8 ] = true;
				return true;
			}
		}
	}
	return false;
}

bool demo::DemoApplication::inputGamepadSelectHold(){
	if( m_gamepad ){
		return m_gamepad->m_buttons[ 8 ];
	}
	return false;
}

bool demo::DemoApplication::InitDefaultScene(){

	auto m1 = m_sceneSystem->addStaticObject( m_gs->getModel(u"../demo/media/scene/1.obj") );
	auto m2 = m_sceneSystem->addStaticObject( m_gs->getModel(u"../demo/media/scene/2.obj") );
	auto m3 = m_sceneSystem->addStaticObject( m_gs->getModel(u"../demo/media/scene/3.obj") );
	auto m4 = m_sceneSystem->addStaticObject( m_gs->getModel(u"../demo/media/scene/4.obj") );
	auto m5 = m_sceneSystem->addStaticObject( m_gs->getModel(u"../demo/media/scene/5.obj") );

	const v3f sc( 0.15f );

	m1->setScale( sc );
	m2->setScale( sc );
	m3->setScale( sc );
	m4->setScale( sc );
	m5->setScale( sc );

	m1->getMaterial( gtConst0U ).textureLayer[ gtConst0U ].texture = m_gs->getTexture( u"../demo/media/scene/1.png" );//load
	m2->getMaterial( gtConst0U ).textureLayer[ gtConst0U ].texture = m_gs->getTexture( u"../demo/media/scene/1.png" );//get 
	m3->getMaterial( gtConst0U ).textureLayer[ gtConst0U ].texture = m_gs->getTexture( u"../demo/media/scene/1.png" );//get 
	m4->getMaterial( gtConst0U ).textureLayer[ gtConst0U ].texture = m_gs->getTexture( u"../demo/media/scene/1.png" );//get 
	m5->getMaterial( gtConst0U ).textureLayer[ gtConst0U ].texture = m_gs->getTexture( u"../demo/media/scene/1.png" );//get 

	m1->getMaterial( gtConst0U ).flags = (u32)gtMaterialFlag::UseLight;
	//m1->getMaterial( gtConst0U ).flags |= (u32)gtMaterialFlag::AlphaDiscard;
	m1->getMaterial( gtConst0U ).flags |= (u32)gtMaterialFlag::AlphaBlend;
	m1->getMaterial( gtConst0U ).transparent = 1.f;

	m_sceneInitialized = true;
	return true;
}

void demo::DemoApplication::ShutdownDefaultScene(){
	if( m_sceneInitialized ){
		m_gs->clearModelCache();
		m_gs->clearTextureCache();
		//m_gs->removeTexture(m_gs->getTexture( u"../demo/media/scene/1.png" ));
		m_sceneSystem->setActiveCamera( nullptr );
		m_sceneInitialized = false;
	}
}

void demo::DemoApplication::RenderDefaultScene(){
	m_sceneSystem->renderScene();
}

bool demo::DemoApplication::inputGamepadL1Hold(){
	if( m_gamepad ){
		return m_gamepad->m_buttons[ gtConst4U ];
	}
	return false;
}

bool demo::DemoApplication::inputGamepadR1Hold(){
	if( m_gamepad ){
		return m_gamepad->m_buttons[ gtConst5U ];
	}
	return false;
}

bool demo::DemoApplication::inputGamepadUpHold(){
	if( m_gamepad ){
		return m_gamepad->m_POV1 == 0 || m_gamepad->m_lY == -1000;
	}
	return false;
}

bool demo::DemoApplication::inputGamepadDownHold(){
	if( m_gamepad ){
		return m_gamepad->m_POV1 == 18000 || m_gamepad->m_lY == 1000;
	}
	return false;
}

bool demo::DemoApplication::inputGamepadLeftHold(){
	if( m_gamepad ){
		return m_gamepad->m_POV1 == 27000 || m_gamepad->m_lX == -1000;
	}
	return false;
}

bool demo::DemoApplication::inputGamepadRightHold(){
	if( m_gamepad ){
		return m_gamepad->m_POV1 == 9000 || m_gamepad->m_lX == 1000;
	}
	return false;
}

bool demo::DemoApplication::update(){
	if( m_gamepad ){
		m_gamepad->poll();
	}
	return m_mainSystem->update();
}

void demo::DemoApplication::UpdateGamepad(){
	
	timer_input += m_delta;

	if( m_gamepad ){
		m_gamepad->poll();
		m_DPad[ gtConst0U ] = m_gamepad->m_POV1 == 0 || m_gamepad->m_lY == -1000;
		m_DPad[ gtConst1U ] = m_gamepad->m_POV1 == 9000 || m_gamepad->m_lX == 1000;
		m_DPad[ gtConst2U ] = m_gamepad->m_POV1 == 18000 || m_gamepad->m_lY == 1000;
		m_DPad[ gtConst3U ] = m_gamepad->m_POV1 == 27000 || m_gamepad->m_lX == -1000;

		if( m_gamepad->m_POV1 == -1 ){
			if(m_gamepad->m_lY > -50 && m_gamepad->m_lY < 50){
				if(m_gamepad->m_lX > -50 && m_gamepad->m_lX < 50){
					timer_input = 0.f;
					timer_input_limit = timer_input_limit_first;
					m_DPadOnce = false;
				}
			}
		}

		for( u32 i = gtConst0U; i < 32u; ++i ){
			if( !m_gamepad->m_buttons[ i ] ){
				m_gamepadButtons[ i ] = false;
			}
		}
	}

	if( timer_input > timer_input_limit ){
		m_DPadOnce = false;
		timer_input = 0.f;
		timer_input_limit = timer_input_limit_second;
	}
}

void demo::DemoApplication::updateSettings(){
	if( m_settingsTypeID < 0 ){
		m_settingsTypeID = 1;
	}else if( m_settingsTypeID > 1 ){
		m_settingsTypeID = 0;
	}

	if( m_settingsTypeID == 0 ){
		m_settingsTextLanguage->setBackgroundColor( gtColorBlack );
		m_settingsTextLanguage->setTextColor( gtColorLightGray );
	}else{
		m_settingsTextLanguage->setBackgroundColor( gtColorLightGray );
		m_settingsTextLanguage->setTextColor( gtColorBlack );
	}

	if( m_settingsTypeID == 1 ){
		m_settingsTextSound->setBackgroundColor( gtColorBlack );
		m_settingsTextSound->setTextColor( gtColorLightGray );
	}else{
		m_settingsTextSound->setBackgroundColor( gtColorLightGray );
		m_settingsTextSound->setTextColor( gtColorBlack );
	}

}

void demo::DemoApplication::updateSettingsText(){

	m_pauseTextContinueShape->setText( getString( u"17" ) );
	m_pauseTextSettingsShape->setText( getString( u"19" ) );
	m_pauseTextExitShape->setText( getString( u"18" ) );

	m_settingsTextLanguageName->setText( m_stringArray[ m_languageID ].m_langName );
	m_settingsTextLanguage->setText( getString( u"25" ) );
	m_settingsTextSound->setText( getString( u"26" ) );

	if( m_useSound )
		m_settingsTextSoundUse->setText( getString( u"27" ) );
	else
		m_settingsTextSoundUse->setText( getString( u"28" ) );

	m_pauseTextMainMenuShape->setText( getString( u"29" ) );
}

void demo::DemoApplication::xmlLoadSettings(){
	if( !gtFileSystem::existFile( m_xmlPath ) ){
		xmlCreateDrefaultSettingsFile();
	}

	m_xml = gtFileSystem::XMLRead( m_xmlPath );
	if( m_xml ){
		{
			auto arr = m_xml->selectNodes( u"/GOST_DEMO/SOUND" );
			if( arr.size() ){
				if( arr[ gtConst0U ]->attributeList.size() ){
					if( arr[ gtConst0U ]->attributeList[ gtConst0U ]->name == u"a" ){
						if( arr[ gtConst0U ]->attributeList[ gtConst0U ]->value == u"1" ){
							m_useSound = true;
						}else{
							m_useSound = false;
						}
					}
				}
			}
		}
		{
			auto arr = m_xml->selectNodes( u"/GOST_DEMO/LANG" );
			if( arr.size() ){
				if( arr[ gtConst0U ]->attributeList.size() ){
					if( arr[ gtConst0U ]->attributeList[ gtConst0U ]->name == u"a" ){
						auto sz = m_stringArray.size();
						for( u32 i = gtConst0U; i < sz; ++i ){
							if( m_stringArray[ i ].m_langName == arr[ gtConst0U ]->attributeList[ gtConst0U ]->value ){
								m_languageID = i;
								break;
							}
						}
					}
				}
			}
		}
	}
}

void demo::DemoApplication::xmlSaveSettings(){

	gtString sound( u"SOUND" );
	gtString lang( u"LANG" );

	auto root = m_xml->getRootNode();
	auto sz = root->nodeList.size();
	for( u32 i = gtConst0U; i < sz; ++i ){
		auto atrsz = root->nodeList[ i ]->attributeList.size();

		if( root->nodeList[ i ]->name == sound ){
			for( u32 o = gtConst0U; o < atrsz; ++o ){
				if( root->nodeList[ i ]->attributeList[ o ]->name == u"a" ){
					if( m_useSound ){
						root->nodeList[ i ]->attributeList[ o ]->value = u"1";
					}else{
						root->nodeList[ i ]->attributeList[ o ]->value = u"0";
					}
				}
			}
		}else if( root->nodeList[ i ]->name == lang ){
			for( u32 o = gtConst0U; o < atrsz; ++o ){
				if( root->nodeList[ i ]->attributeList[ o ]->name == u"a" ){
					root->nodeList[ i ]->attributeList[ o ]->value = m_stringArray[ m_languageID ].m_langName;
				}
			}
		}
	}

	gtFileSystem::XMLWrite( m_xmlPath, root );

}

void demo::DemoApplication::xmlCreateDrefaultSettingsFile(){
	gtFile_t file = util::createFileForWriteText( m_xmlPath );
	
	gtTextFileInfo info;
	info.m_endian = gtTextFileEndian::Little;
	info.m_format = gtTextFileFormat::UTF_16;
	info.m_hasBOM = true;

	file->setTextFileInfo( info );

	file->write( gtString(
u"<?xml version=\"1.0\"?>\n\
<GOST_DEMO>\n\
\t<SOUND a=\"1\" />\n\
\t<LANG a=\"English\" />\n\
</GOST_DEMO>"
) 
);

}

void demo::DemoApplication::Pause(){
	m_state = DemoState::DemoMenu;
	m_isPause = true;
	ShowMenu();
	playAudio(DemoAudioType::Accept);
}

void demo::DemoApplication::StopDemo(){
	m_state = DemoState::MainMenu;
	m_isPause = false;
	hideDemoHUD();
	HideMenu();
	m_demoPauseMenuID = 0;
	updateDemoPause();
	m_demoArrays[m_activeDemoTypeSelected][m_activeDemoSelected].Shutdown();
	ShutdownDefaultScene();
}

void demo::DemoApplication::inputDemoMenuPause(){

	if( m_eventConsumer->keyDown( gtKey::K_ESCAPE ) || inputGamepadMainMenuStart() || inputGamepadMainMenuEscape() ){
		playAudio(DemoAudioType::Cancel);
		m_isPause = false;
		HideMenu();
	}

	if( m_eventConsumer->keyDown( gtKey::K_UP ) || inputGamepadMainMenuUp() ){
		playAudio(DemoAudioType::Select);
		--m_demoPauseMenuID;
		updateDemoPause();
	}

	if( m_eventConsumer->keyDown( gtKey::K_DOWN ) || inputGamepadMainMenuDown() ){
		playAudio(DemoAudioType::Select);
		++m_demoPauseMenuID;
		updateDemoPause();
	}

	if( m_eventConsumer->keyDown( gtKey::K_ENTER ) || inputGamepadMainMenuEnter() ){
		playAudio(DemoAudioType::Accept);

		if( m_demoPauseMenuID == 0 ){
			m_isPause = false;
			HideMenu();
		}else if( m_demoPauseMenuID == 1 ){
			StopDemo();
		}else if( m_demoPauseMenuID == 2 ){
			m_mainSystem->shutdown();
		}
	}
}

void demo::DemoApplication::runMainMenu(){
	if( m_isPause ){
		pauseBackgroundFadeOut();
		inputMainMenuPause();
	}else{
		pauseBackgroundFadeIn();
		inputMainMenu();
	}
}

void demo::DemoApplication::runDemo(){
	if( m_input->isKeyDown( gtKey::K_F1 ) || inputGamepadSelectHold() ){
		m_showDescription = true;
		pauseBackgroundFadeOut();
	}else{
		pauseBackgroundFadeIn();
	}

	m_demoArrays[m_activeDemoTypeSelected][m_activeDemoSelected].Input( m_delta );
	m_demoArrays[m_activeDemoTypeSelected][m_activeDemoSelected].Update();
}

void demo::DemoApplication::runDemoMenu(){
	if( m_isPause ){
		pauseBackgroundFadeOut();
		inputDemoMenuPause();
	}else{
		pauseBackgroundFadeIn();
	}
}

void demo::DemoApplication::updateDemoPause(){
	if( m_demoPauseMenuID < 0 ){
		m_demoPauseMenuID = 2;
	}else if( m_demoPauseMenuID > 2 ){
		m_demoPauseMenuID = 0;
	}

	m_pauseTextContinueShape->setBackgroundColor( gtColorLightGray );
	m_pauseTextMainMenuShape->setBackgroundColor( gtColorLightGray );
	m_pauseTextExitShape->setBackgroundColor( gtColorLightGray );
	m_pauseTextContinueShape->setTextColor( gtColorBlack );
	m_pauseTextMainMenuShape->setTextColor( gtColorBlack );
	m_pauseTextExitShape->setTextColor( gtColorBlack );

	if( m_demoPauseMenuID == 0 ){
		m_pauseTextContinueShape->setBackgroundColor( gtColorBlack );
		m_pauseTextContinueShape->setTextColor( gtColorLightGray );
	}else if( m_demoPauseMenuID == 1 ){
		m_pauseTextMainMenuShape->setBackgroundColor( gtColorBlack );
		m_pauseTextMainMenuShape->setTextColor( gtColorLightGray );
	}else if( m_demoPauseMenuID == 2 ){
		m_pauseTextExitShape->setBackgroundColor( gtColorBlack );
		m_pauseTextExitShape->setTextColor( gtColorLightGray );
	}
}

void demo::DemoApplication::Render(){
	switch( m_state ){
	case DemoState::DemoMenu:{
		renderDemoMenu();
		}break;
	case DemoState::DemoRun:{
		renderDemo();
		}break;
	case DemoState::MainMenu:{
		renderMainMenu();
		}break;
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