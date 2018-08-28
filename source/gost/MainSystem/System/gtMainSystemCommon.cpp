//	GOST

#include "common.h"

gtPtr<gtLogImpl> gtMainSystemCommon::s_log;
//gtFileSystemCommon* gtMainSystemCommon::s_fileSystem;
//gtMemorySystemCommon* gtMainSystemCommon::s_memorySystem;
gtMainSystemCommon* gtMainSystemCommon::s_instance;
gtMainSystemCommon* gtMainSystemCommon::getInstance(){return s_instance;}

gtMainSystemCommon::gtMainSystemCommon() :
	m_useTimer( false ),
	m_timer( 0u ),
	m_time( 0u ),
	m_tick( 0u ),
    m_isRun( true ),
	m_stackTracer( nullptr ),
	m_systemWindowCount( 0u ),
	m_gs( nullptr )
{
	s_instance     = this;
	s_log          = gtPtrNew<gtLogImpl>         ( new gtLogImpl );
	m_inputSystem  = gtPtrNew<gtInputSystemImpl> ( new gtInputSystemImpl );
	m_pluginSystem = gtPtrNew<gtPluginSystemImpl>( new gtPluginSystemImpl );
	m_modelSystem  = gtPtrNew<gtModelSystemImpl> ( new gtModelSystemImpl );
	m_sceneSystem  = gtPtrNew<gtSceneSystemImpl> ( new gtSceneSystemImpl );
	m_GUISystem    = gtPtrNew<gtGUISystemImpl>   ( new gtGUISystemImpl );
	m_DebugRenderer= gtPtrNew<gtDebugRendererImpl>   ( new gtDebugRendererImpl );

	initCVarSystem();
}

gtMainSystemCommon::~gtMainSystemCommon(){
	if( m_stackTracer ){
		delete m_stackTracer;
		m_stackTracer = nullptr;
	}
}

gtLog* gtMainSystemCommon::getLog()       { return s_log.data(); }
gtMainSystem* gtMainSystem::getInstance() { return gtMainSystemCommon::getInstance(); }

void gtMainSystemCommon::initStackTracer(){
	if( !m_stackTracer ){
		m_stackTracer = new gtStackTracer( this );
	}
}

void gtMainSystemCommon::initEventSystem(){
	m_events		= gtPtrNew<gtEventSystem>( new gtEventSystem( m_inputSystem.data(), m_params.m_consumer ) );
}

gtString gtCVarSystem_sys_quit( const gtCVarSystemNode& node ){
	gtMainSystem::getInstance()->shutdown();
	return gtString();
}

gtString gtCVarSystem_con_clear( const gtCVarSystemNode& node ){gtMainSystem::getInstance()->getOutputWindow()->clear(); return gtString(); }
gtString gtCVarSystem_con_hide( const gtCVarSystemNode& node ){gtMainSystem::getInstance()->getOutputWindow()->hide(); return gtString(); }
gtString gtCVarSystem_con_show( const gtCVarSystemNode& node ){gtMainSystem::getInstance()->getOutputWindow()->show(); return gtString(); }


void gtMainSystemCommon::initCVarSystem(){
	m_CVarSystem = gtPtrNew<gtCVarSystemImpl>( new gtCVarSystemImpl );
	m_CVarSystem->addCommand( gtCVarType::System, u"help", u"Display information about the command. Use `help command`.\r\n\
help - show this message\r\n\
sys_quit - Shutdown engine\r\n\
con_clear - Clear output window\r\n\
con_hide - Hide output window\r\n\
con_show - Show output window\r\n", u"", gtCVarSystem_printDescription );
	m_CVarSystem->addCommand( gtCVarType::System, u"sys_quit", u"Shutdown engine", u"", gtCVarSystem_sys_quit );
	m_CVarSystem->addCommand( gtCVarType::System, u"con_clear", u"Clear output window", u"", gtCVarSystem_con_clear );
	m_CVarSystem->addCommand( gtCVarType::System, u"con_hide", u"Hide output window", u"", gtCVarSystem_con_hide );
	m_CVarSystem->addCommand( gtCVarType::System, u"con_show", u"Show output window", u"", gtCVarSystem_con_show );
}


void	gtMainSystemCommon::updateEvents(){
	this->m_events->getKeyboardAndMouseStates();
	m_GUISystem->update();
	if( this->m_events->getConsumer() ){
		this->m_events->runEventLoop();
		this->m_events->resetEvents();
	}else{
	}
	updateTimer();
}

gtStackTracer*	gtMainSystemCommon::getStackTracer(){
	return m_stackTracer;
}

void gtStackTracer::dumpStackTrace(){
	gtMainSystemCommon::getInstance()->getStackTracer()->printStackTrace(2u,gtConst3U);
}

gtPtr<gtGraphicsSystem> gtMainSystemCommon::createGraphicsSystem( /*gtPlugin* videoDriverPlugin,*/ const gtGraphicsSystemInfo& params, const GT_GUID& uid ){
	auto plugin = (gtPluginCommon*)this->getPluginSystem()->getPlugin( uid );
	if( !plugin ){
		gtLogWriter::printError( u"Can not find renderer plugin in `plugins` folder" );
		return nullptr;
	}

	if( plugin->getInfo().m_info.m_type != gtPluginType::Render ){
		gtLogWriter::printError( u"Can not create graphics system" );
		return nullptr;
	}

	gtGraphicsSystemInfo p = params;
	plugin->load();
	gtGraphicsSystem * d = ((gtPluginRender*)plugin->getImplementation())->loadDriver( &p );
	if( d )
		m_drivers.push_back( d );

	m_gs = d;
	
	m_DebugRenderer->setGS( d );
	
	return gtPtrNew<gtGraphicsSystem>(d);
}

gtPtr<gtPhysicsSystem> gtMainSystemCommon::createPhysicsSystem( const gtPhysicsSystemInfo& psi, const GT_GUID& uid ){
	auto plugin = (gtPluginCommon*)this->getPluginSystem()->getPlugin( uid );
	if( !plugin ){
		gtLogWriter::printError( u"Can not find physics plugin in `plugins` folder" );
		return nullptr;
	}

	if( plugin->getInfo().m_info.m_type != gtPluginType::Physics ){
		gtLogWriter::printError( u"Can not create physics system" );
		return nullptr;
	}

	gtPhysicsSystemInfo p = psi;
	plugin->load();
	gtPhysicsSystem * d = ((gtPluginPhysics*)plugin->getImplementation())->loadPhysics( &p );

	return gtPtrNew<gtPhysicsSystem>(d);
}

gtGraphicsSystem* gtMainSystemCommon::getMainVideoDriver()         { return m_gs; }
void gtMainSystemCommon::setMainVideoDriver( gtGraphicsSystem* d ) { m_gs = d; }
u32 gtMainSystemCommon::getLoadedVideoDriverCount()                { return m_drivers.size(); }

gtGraphicsSystem* gtMainSystemCommon::getLoadedVideoDriver( u32 id ){
	if( m_drivers.size() ){
		GT_ASSERT2( id < m_drivers.size(), "id < m_drivers.size()" );
		return m_drivers[ id ];
	}
	return nullptr;
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

bool gtMainSystemCommon::checkEventType( const gtEvent& ev ){
	u32 cur = 0u;
	while( true ){

		auto event = m_events->getEvent( cur );

		if( event.type == gtEventType::None )
			break;

		if( event.type == ev.type ){
			return true;
		}

		++cur;

		if( cur == EventMax)
			break;
	}
	return false;
}

bool gtMainSystemCommon::checkEvent( gtEvent& ev, bool(*compare_function)( gtEvent& current_event, gtEvent& user_event ) ){
	u32 cur = 0u;
	while( true ){

		auto event = m_events->getEvent( cur );

		if( event.type == gtEventType::None )
			break;

		if( event.type == ev.type ){
			if( compare_function( event, ev ) ){
				return true;
			}
		}

		++cur;
		if( cur == EventMax)
			break;
	}
	return false;
}

gtCVarSystem* gtMainSystemCommon::getCVarSystem(){
	return m_CVarSystem.data();
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



const gtDeviceCreationParameters& gtMainSystemCommon::getDeviceCreationParameters(){
	return m_params;
}

bool gtMainSystemCommon::isRun(){
	if( !m_isRun ){
	}
	return m_isRun;
}

gtPtr<gtScriptSystem>       gtMainSystemCommon::createScriptSystem( const GT_GUID& uid ){
	gtPluginCommon * plugin = nullptr;
	gtPluginScript * pluginScript = nullptr;
	auto * ps = getPluginSystem();
	plugin = (gtPluginCommon*)ps->getPlugin( uid );
	
	//если указанный плагин не запустился то проба использовать другой
	if( !plugin ){
		u32 np = ps->getNumOfPlugins();

		for( u32 i = 0u; i < np; ++i ){
			auto * pl = (gtPluginCommon*)ps->getPlugin( i );
			if( pl->getInfo().m_info.m_type == gtPluginType::Script ){
				pl->load();
				pluginScript = (gtPluginScript *)pl->getImplementation();
				return gtPtrNew<gtScriptSystem>( pluginScript->loadScriptPlugin() );
			}
		}

		return nullptr;
	}
	
	plugin->load();
	pluginScript = (gtPluginScript *)plugin->getImplementation();
	return gtPtrNew<gtScriptSystem>( pluginScript->loadScriptPlugin() );
}

gtPtr<gtAudioSystem> gtMainSystemCommon::createAudioSystem( const GT_GUID& uid ){
	gtPluginCommon * plugin = nullptr;
	gtPluginAudio * pluginAudio = nullptr;
	auto * ps = getPluginSystem();
	plugin = (gtPluginCommon*)ps->getPlugin( uid );
	if( !plugin ){
		u32 np = ps->getNumOfPlugins();

		for( u32 i = 0u; i < np; ++i ){

			auto * pl = (gtPluginCommon*)ps->getPlugin( i );

			if( pl->getInfo().m_info.m_type == gtPluginType::Audio ){
				pl->load();
				pluginAudio = (gtPluginAudio *)pl->getImplementation();
				return gtPtrNew<gtAudioSystem>( pluginAudio->loadAudioDriver() );
			}
		}

		return nullptr;
	}
	plugin->load();
	pluginAudio = (gtPluginAudio *)plugin->getImplementation();
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

v4f gtMainSystemCommon::screenToWorld( const gtVector2<s16>& coord ){
	v4f v;

	auto camera = m_sceneSystem->getActiveCamera();

	if( camera ){

		auto P = camera->getProjectionMatrix();
		auto V = camera->getViewMatrix();

		auto PV = P * V;
		PV.invert();

		auto rc = this->m_gs->getParams().m_outWindow->getClientRect();

		v4f coords;
		coords[0] = (2.0f*((float)(coord.x)/(rc.z-0)))-1.0f;
		coords[1]=1.0f-(2.0f*((float)(coord.y)/(rc.w-0)));
        coords[2]=2.0* .0f -1.0;
        coords[3]=1.0;

		v = math::mul4( coords, PV );

		v.w = 1.0 / v.w;
		v.x *= v.w;
		v.y *= v.w;
		v.z *= v.w;
	}
	return v;
}

gtRayf32 gtMainSystemCommon::getRayFromScreen( const gtVector2<s16>& coord, f32 len ){
	gtRayf32 ray;

	auto camera = m_sceneSystem->getActiveCamera();

	if( camera ){

		auto P = camera->getProjectionMatrix();

		auto rc = this->m_gs->getParams().m_outWindow->getClientRect();

		f32 v3dx = ((( 2.0f * coord.x) / rc.getWidth() ) - 1 ) / P[0].x;
		f32 v3dy = -((( 2.0f * coord.y) / rc.getHeight() ) - 1 ) / P[1].y;
		f32 v3dz = -1.0f; // f32 v3dz = 1.0f; for LH

		v4f rayPos( v3dx, v3dy, v3dz, 0.f );

		auto V = camera->getViewMatrix();
		V.invert();

		{
			v4f result  = V[3];
			v4f inverse( result.w );
			inverse.x = 1.f / inverse.x;
			inverse.y = 1.f / inverse.y;
			inverse.z = 1.f / inverse.z;
			inverse.w = 1.f / inverse.w;

			ray.m_begin = result * inverse;
		}
		{
			v4f X,Y,Z,result;
			Z.set(rayPos.z);
			Y.set(rayPos.y);
			X.set(rayPos.x);

			result = Z * V[2];
			result += Y * V[1];
			result += X * V[0];

			ray.m_end = result;

			ray.m_end = (ray.m_begin + (ray.m_end*len));
		}

	}

	return ray;
}

u32              gtMainSystemCommon::getMajorVersion(){
	return GOST_MAJOR_VERSION;
}

u32              gtMainSystemCommon::getMinorVersion(){
	return GOST_MINOR_VERSION;
}

gtDebugRenderer* gtMainSystemCommon::getDebugRenderer(){
	return m_DebugRenderer.data();
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
