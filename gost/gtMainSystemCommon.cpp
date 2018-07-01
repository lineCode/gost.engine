//	GOST 

#include "common.h"

gtPtr<gtLogImpl> gtMainSystemCommon::s_log;
gtFileSystemCommon* gtMainSystemCommon::s_fileSystem;
gtMainSystemCommon* gtMainSystemCommon::s_instance;
gtMainSystemCommon* gtMainSystemCommon::getInstance(){return s_instance;}

gtMainSystemCommon::gtMainSystemCommon() : m_isRun( true ),
	m_stackTracer( nullptr ), 
	m_systemWindowCount( 0u ),
	m_driver( nullptr ),
	m_useTimer( false ),
	m_timer( 0u ),
	m_time( 0u ),
	m_tick( 0u )
{

	s_log = gtPtrNew<gtLogImpl>( new gtLogImpl );
	s_instance = this;

	m_pluginSystem	= gtPtrNew<gtPluginSystemImpl>( new gtPluginSystemImpl );
	m_modelSystem = gtPtrNew<gtModelSystemImpl>( new gtModelSystemImpl );
	m_sceneSystem = gtPtrNew<gtSceneSystemImpl>( new gtSceneSystemImpl );
	m_GUISystem = gtPtrNew<gtGUISystemImpl>( new gtGUISystemImpl );

}

gtMainSystemCommon::~gtMainSystemCommon(){
	if( m_stackTracer ){
		delete m_stackTracer;
		m_stackTracer = nullptr;
	}
}

gtLog* gtMainSystemCommon::getLog(){
	return s_log.data();
}


void gtMainSystemCommon::initStackTracer(){
	if( !m_stackTracer ){
		m_stackTracer = new gtStackTracer( this );
	}
}

void gtMainSystemCommon::initEventSystem(){
	m_events		= gtPtrNew<gtEventSystem>( new gtEventSystem( m_params.m_consumer ) );
}



gtMainSystem*	gtMainSystem::getInstance(){
	return gtMainSystemCommon::getInstance();
}
			
gtStackTracer*	gtMainSystemCommon::getStackTracer(){
	return m_stackTracer;
}

void gtStackTracer::dumpStackTrace(){
	gtMainSystemCommon::getInstance()->getStackTracer()->printStackTrace(2u,3u);
}

gtPtr<gtDriver> gtMainSystemCommon::createVideoDriver( /*gtPlugin* videoDriverPlugin,*/ const gtDriverInfo& params, const GT_GUID& uid ){

	auto plugin = this->getPluginSystem()->getPlugin( uid );

	if( !plugin ){
		gtLogWriter::printError( u"Can not find video driver in `plugins` folder" );
		return nullptr;
	}

	if( plugin->getInfo().m_info.m_type != gtPluginType::Render ){
		gtLogWriter::printError( u"Can not create video driver" );
		return nullptr;
	}

	gtDriver * d = ((gtPluginRender*)plugin)->loadDriver( params );

	if( d )
		m_drivers.push_back( d );

	m_driver = d;

	return gtPtrNew<gtDriver>(d);
}

u32 gtMainSystemCommon::getLoadedVideoDriverCount(){
	return m_drivers.size();
}

gtDriver* gtMainSystemCommon::getLoadedVideoDriver( u32 id ){
	if( m_drivers.size() ){
		GT_ASSERT2( id < m_drivers.size(), "id < m_drivers.size()" );
		return m_drivers[ id ];
	}
	return nullptr;
}

gtDriver* gtMainSystemCommon::getMainVideoDriver(){
	return m_driver;
}

void gtMainSystemCommon::setMainVideoDriver( gtDriver* d ){
	m_driver = d;
}

bool gtMainSystemCommon::allocateMemory( void** data, u32 size ){
	GT_ASSERT1( !(*data), "Memory block is not free or pointer not set nullptr", "*data==nullptr" );
	*data = std::malloc( size );

	if( (*data) ){
		return true;
	}

	return false;
}

void gtMainSystemCommon::freeMemory( void** data ){
	GT_ASSERT1( *data, "Memory block is not allocated or set nullptr", "*data!=nullptr" );
	std::free( *data );
	*data = nullptr;
}

gtPtr<gtImage>	gtMainSystemCommon::loadImage( const gtString& fileName ){
	return gtPtrNew<gtImage>( this->m_pluginSystem->importImage( fileName ) );
}

gtPtr<gtImage>	gtMainSystemCommon::loadImage( const gtString& fileName, const GT_GUID& pluginGUID ){
	return gtPtrNew<gtImage>( this->m_pluginSystem->importImage( fileName, pluginGUID, true ) );
}

void		gtMainSystemCommon::addEvent( const gtEvent& ev, u8 prior ){
	if( m_events.data() )
		m_events->addEvent( ev, prior );
}

gtModelSystem*	gtMainSystemCommon::getModelSystem(){
	return m_modelSystem.data();
}

gtPluginSystem*	gtMainSystemCommon::getPluginSystem(){
	return m_pluginSystem.data();
}

gtSceneSystem*	gtMainSystemCommon::getSceneSystem( gtDriver * currentRenderDriver ){
	if( currentRenderDriver )
		m_sceneSystem->setCurrentRenderDriver( currentRenderDriver );
	return m_sceneSystem.data();
}

gtGUISystem*	gtMainSystemCommon::getGUISystem( gtDriver * currentRenderDriver ){
	if( currentRenderDriver )
		m_GUISystem->setCurrentRenderDriver( currentRenderDriver );
		m_GUISystem->init();
	return m_GUISystem.data();
}

const gtMatrix4& gtMainSystemCommon::getMatrixWorld(){
	return m_WVP[ 0u ];
}

const gtMatrix4& gtMainSystemCommon::getMatrixView(){
	return m_WVP[ 1u ];
}

const gtMatrix4& gtMainSystemCommon::getMatrixProjection(){
	return m_WVP[ 2u ];
}

void gtMainSystemCommon::setMatrixWorld( const gtMatrix4& m ){
	m_WVP[ 0u ] = m;
}

void gtMainSystemCommon::setMatrixView( const gtMatrix4& m ){
	m_WVP[ 1u ] = m;
}

void gtMainSystemCommon::setMatrixProjection( const gtMatrix4& m ){
	m_WVP[ 2u ] = m;
}

bool gtMainSystemCommon::pollEvent( gtEvent& event ){
	if( !m_isRun ) return false;
	return m_events->pollEvent( event );
}

bool gtMainSystemCommon::isKeyPressed( gtKey key ){
	return m_events->isKeyDown( key );
}

bool gtMainSystemCommon::isLMBDown(){
	return m_events->isLMBDown();
}

bool gtMainSystemCommon::isRMBDown(){
	return m_events->isRMBDown();
}

bool gtMainSystemCommon::isMMBDown(){
	return m_events->isMMBDown();
}

const gtVector2<u16>& gtMainSystemCommon::getCursorPosition(){
	return m_events->getCursorPosition();
}

const gtDeviceCreationParameters& gtMainSystemCommon::getDeviceCreationParameters(){
	return m_params;
}

bool gtMainSystemCommon::isRun(){
	if( !m_isRun ){
	}
	return m_isRun;
}

gtPtr<gtGameController> gtMainSystemCommon::createGameContoller( const GT_GUID& uid ){
	gtPlugin * plugin = nullptr;
	gtPluginInput * pluginInput = nullptr;
	
	auto * ps = getPluginSystem();
	plugin = ps->getPlugin( uid );

	if( !plugin ){
		u32 np = ps->getNumOfPlugins();

		for( u32 i = 0u; i < np; ++i ){

			auto * pl = ps->getPlugin( i );

			if( pl->getInfo().m_info.m_type == gtPluginType::Input ){
			
				pluginInput = ps->getAsPluginInput( pl );

				auto ret = pluginInput->loadInputDriver();

				if( ret ){

					return gtPtr<gtGameController>( ret );
				}
			}
		}

		return nullptr;
	}
	pluginInput = ps->getAsPluginInput( plugin );
	return gtPtrNew<gtGameController>( pluginInput->loadInputDriver() );
}

gtPtr<gtAudioSystem> gtMainSystemCommon::createAudioSystem( const GT_GUID& uid ){
	gtPlugin * plugin = nullptr;
	gtPluginAudio * pluginAudio = nullptr;
	
	auto * ps = getPluginSystem();
	plugin = ps->getPlugin( uid );

	if( !plugin ){
		u32 np = ps->getNumOfPlugins();

		for( u32 i = 0u; i < np; ++i ){

			auto * pl = ps->getPlugin( i );

			if( pl->getInfo().m_info.m_type == gtPluginType::Audio ){
			
				pluginAudio = ps->getAsPluginAudio( pl );

				auto ret = pluginAudio->loadAudioDriver();

				if( ret ){

					return gtPtr<gtAudioSystem>( ret );
				}
			}
		}

		return nullptr;
	}
	pluginAudio = ps->getAsPluginAudio( plugin );
	return gtPtrNew<gtAudioSystem>( pluginAudio->loadAudioDriver() );
}

void gtMainSystemCommon::setTimer( u32 milliseconds ){
	m_useTimer = true;
	m_timer = milliseconds;
}

void gtMainSystemCommon::updateTimer(){
	static u32 t1 = 0u;
	u32 t2 = getTime();
	m_tick = t2 - t1;
	t1 = t2;
	if( m_useTimer ){
		m_time += m_tick;

		if( m_time > m_timer ){ // end
			m_useTimer = false;

			gtEvent e;
			e.type = gtEventType::System;
			e.systemEvent.eventID = GT_EVENT_SYSTEM_TIMER;
			addEvent( e, 1 );

			m_time = 0u;
		}
	}
}

/*
Copyright (c) 2017-2018 532235

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
