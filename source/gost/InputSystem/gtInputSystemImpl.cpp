#include "common.h"

gtInputSystemImpl::gtInputSystemImpl():m_LMB(gtConst0),m_RMB(gtConst0),m_MMB(gtConst0){
	memset( m_keysDown, 0, 256u );
}
gtInputSystemImpl::~gtInputSystemImpl(){}

const gtVector2<s16>& gtInputSystemImpl::getCursorPosition(){
	return m_cursorPosition;
}

bool gtInputSystemImpl::isKeyDown( gtKey key ){
	return m_keysDown[ (u32)key ];
}

bool gtInputSystemImpl::isLMBDown(){
	return m_LMB == gtConst_1;
}

bool gtInputSystemImpl::isRMBDown(){
	return m_RMB == gtConst_1;
}

bool gtInputSystemImpl::isMMBDown(){
	return m_MMB == gtConst_1;
}

void gtInputSystemImpl::reset(){
	m_LMB = gtConst0;
	m_RMB = gtConst0;
	m_MMB = gtConst0;
}

void gtInputSystemImpl::setKeyboardState( gtEventKeyboard * e ){
	m_keysDown[ (u32)e->key ] = e->state.bits.b0;
}

void gtInputSystemImpl::setMouseState( gtEventMouse * e ){
	m_LMB = e->LMB;
	m_RMB = e->RMB;
	m_MMB = e->MMB;
	m_cursorPosition.x = e->x;
	m_cursorPosition.y = e->y;
}

gtPtr<gtInputController> gtInputSystemImpl::createInputContoller( const GT_GUID& uid ){
	gtPlugin * plugin = nullptr;
	gtPluginInput * pluginInput = nullptr;
	
	auto * ps = gtMainSystem::getInstance()->getPluginSystem();
	plugin = ps->getPlugin( uid );

	if( !plugin ){
		u32 np = ps->getNumOfPlugins();

		for( u32 i = gtConst0U; i < np; ++i ){

			auto * pl = ps->getPlugin( i );

			if( pl->getInfo().m_info.m_type == gtPluginType::Input ){
			
				pluginInput = ps->getAsPluginInput( pl );

				auto ret = pluginInput->loadInputDriver();

				if( ret ){

					return gtPtr<gtInputController>( ret );
				}
			}
		}

		return nullptr;
	}
	pluginInput = ps->getAsPluginInput( plugin );
	return gtPtrNew<gtInputController>( pluginInput->loadInputDriver() );
}