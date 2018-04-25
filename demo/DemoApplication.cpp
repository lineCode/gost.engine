#include "creator.h"

demo::DemoApplication::DemoApplication( void ):
m_guiSystem( nullptr ){
#ifdef GT_PLATFORM_WIN32
	m_params.m_device_type = gtDeviceType::Windows;

	m_outputWindow	=	gtPtrNew<gtOutputWindow>( new DemoApplicationOutputWindow );
	m_params.m_outputWindow = m_outputWindow.data();
#elif GT_PLATFORM_LINUX
	m_params.m_device_type = gtDeviceType::Linux;
#endif

	m_params.m_consumer		= &m_eventConsumer;
}

demo::DemoApplication::~DemoApplication( void ){}

bool demo::DemoApplication::Init( void ){

	if( !initEngine() )
		return false;

	if( !initWindow() )
		return false;

	if( !initVideoDriver() )
		return false;

	if( !initBackground() )
		return false;
	
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

bool demo::DemoApplication::initBackground( void ){
	gtString logoPath(u"../demo/media/logo.png");
	if( gtFileSystem::existFile( logoPath ) )
		m_backgroundTexture = m_driver->getTexture( logoPath );
	else{
		gtLogWriter::printWarning( u"Can not load background texture. File %s not exist.", logoPath.data() );
	}

	m_backgroundShape	= m_guiSystem->createShapeRectangle( m_windowInfo.m_rect, gtColor( gtColorWhite ) );
	if( !m_backgroundShape.data() ){
		gtLogWriter::printError( u"Can not background shape." );
		return false;
	}

	if( m_backgroundTexture.data() ){
		m_backgroundShape->setTexture( m_backgroundTexture.data() );
	}else{
		m_backgroundShape->setColor( gtColor( gtColorBlack ) );
	}

	return true;
}

void demo::DemoApplication::Run( void ){
	while( m_mainSystem->update() ){
		if( m_mainSystem->isKeyPressed( gtKey::K_ESCAPE ) ){
			m_mainSystem->shutdown();
		}

		m_driver->beginRender();

		m_driver->setDepthState( false );
		m_backgroundShape->render();
		m_driver->setDepthState();

		m_driver->endRender();

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