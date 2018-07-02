//	GOST 

#include "common.h"

gtPtr<gtLogImpl> gtMainSystemCommon::s_log;
gtFileSystemCommon* gtMainSystemCommon::s_fileSystem;
//gtMemorySystemCommon* gtMainSystemCommon::s_memorySystem;
gtMainSystemCommon* gtMainSystemCommon::s_instance;
gtMainSystemCommon* gtMainSystemCommon::getInstance(){return s_instance;}

gtMainSystemCommon::gtMainSystemCommon() : m_isRun( true ),
	m_stackTracer( nullptr ), 
	m_systemWindowCount( gtConst0U ),
	m_gs( nullptr ),
	m_useTimer( false ),
	m_timer( gtConst0U ),
	m_time( gtConst0U ),
	m_tick( gtConst0U )
{

	s_log = gtPtrNew<gtLogImpl>( new gtLogImpl );
	s_instance = this;

	m_inputSystem = gtPtrNew<gtInputSystemImpl>( new gtInputSystemImpl );
	m_pluginSystem = gtPtrNew<gtPluginSystemImpl>( new gtPluginSystemImpl );
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
	m_events		= gtPtrNew<gtEventSystem>( new gtEventSystem( m_inputSystem.data(), m_params.m_consumer ) );
}



gtMainSystem*	gtMainSystem::getInstance(){
	return gtMainSystemCommon::getInstance();
}
			
gtStackTracer*	gtMainSystemCommon::getStackTracer(){
	return m_stackTracer;
}

void gtStackTracer::dumpStackTrace(){
	gtMainSystemCommon::getInstance()->getStackTracer()->printStackTrace(gtConst2U,gtConst3U);
}

gtPtr<gtGraphicsSystem> gtMainSystemCommon::createGraphicsSystem( /*gtPlugin* videoDriverPlugin,*/ const gtGraphicsSystemInfo& params, const GT_GUID& uid ){

	auto plugin = this->getPluginSystem()->getPlugin( uid );

	if( !plugin ){
		gtLogWriter::printError( u"Can not find video driver in `plugins` folder" );
		return nullptr;
	}

	if( plugin->getInfo().m_info.m_type != gtPluginType::Render ){
		gtLogWriter::printError( u"Can not create video driver" );
		return nullptr;
	}

	gtGraphicsSystem * d = ((gtPluginRender*)plugin)->loadDriver( params );

	if( d )
		m_drivers.push_back( d );

	m_gs = d;

	return gtPtrNew<gtGraphicsSystem>(d);
}

u32 gtMainSystemCommon::getLoadedVideoDriverCount(){
	return m_drivers.size();
}

gtGraphicsSystem* gtMainSystemCommon::getLoadedVideoDriver( u32 id ){
	if( m_drivers.size() ){
		GT_ASSERT2( id < m_drivers.size(), "id < m_drivers.size()" );
		return m_drivers[ id ];
	}
	return nullptr;
}

gtGraphicsSystem* gtMainSystemCommon::getMainVideoDriver(){
	return m_gs;
}

void gtMainSystemCommon::setMainVideoDriver( gtGraphicsSystem* d ){
	m_gs = d;
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

gtInputSystem*  gtMainSystemCommon::getInputSystem(){
	return m_inputSystem.data();
}

gtModelSystem*	gtMainSystemCommon::getModelSystem(){
	return m_modelSystem.data();
}

gtPluginSystem*	gtMainSystemCommon::getPluginSystem(){
	return m_pluginSystem.data();
}

gtSceneSystem*	gtMainSystemCommon::getSceneSystem( gtGraphicsSystem * currentRenderDriver ){
	if( currentRenderDriver )
		m_sceneSystem->setCurrentRenderDriver( currentRenderDriver );
	return m_sceneSystem.data();
}

gtGUISystem*	gtMainSystemCommon::getGUISystem( gtGraphicsSystem * currentRenderDriver ){
	if( currentRenderDriver )
		m_GUISystem->setCurrentRenderDriver( currentRenderDriver );
		m_GUISystem->init();
	return m_GUISystem.data();
}

const gtMatrix4& gtMainSystemCommon::getMatrixWorld(){
	return m_WVP[ gtConst0U ];
}

const gtMatrix4& gtMainSystemCommon::getMatrixView(){
	return m_WVP[ gtConst1U ];
}

const gtMatrix4& gtMainSystemCommon::getMatrixProjection(){
	return m_WVP[ gtConst2U ];
}

void gtMainSystemCommon::setMatrixWorld( const gtMatrix4& m ){
	m_WVP[ gtConst0U ] = m;
}

void gtMainSystemCommon::setMatrixView( const gtMatrix4& m ){
	m_WVP[ gtConst1U ] = m;
}

void gtMainSystemCommon::setMatrixProjection( const gtMatrix4& m ){
	m_WVP[ gtConst2U ] = m;
}

bool gtMainSystemCommon::pollEvent( gtEvent& event ){
	if( !m_isRun ) return false;
	return m_events->pollEvent( event );
}



const gtDeviceCreationParameters& gtMainSystemCommon::getDeviceCreationParameters(){
	return m_params;
}

bool gtMainSystemCommon::isRun(){
	if( !m_isRun ){
	}
	return m_isRun;
}

gtPtr<gtAudioSystem> gtMainSystemCommon::createAudioSystem( const GT_GUID& uid ){
	gtPlugin * plugin = nullptr;
	gtPluginAudio * pluginAudio = nullptr;
	
	auto * ps = getPluginSystem();
	plugin = ps->getPlugin( uid );

	if( !plugin ){
		u32 np = ps->getNumOfPlugins();

		for( u32 i = gtConst0U; i < np; ++i ){

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
	static u32 t1 = gtConst0U;
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

			m_time = gtConst0U;
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
