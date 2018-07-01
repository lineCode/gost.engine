#include "common.h"

gtInputSystemImpl::gtInputSystemImpl(){
	memset( m_keysDown, 0, 256u );
	m_mouseState.byte = 0u;
}
gtInputSystemImpl::~gtInputSystemImpl(){}

const gtVector2<u16>& gtInputSystemImpl::getCursorPosition(){
	return m_cursorPosition;
}

bool gtInputSystemImpl::isKeyDown( gtKey key ){
	return m_keysDown[ (u32)key ];
}

bool gtInputSystemImpl::isLMBDown(){
	return m_mouseState.bits.b0;
}

bool gtInputSystemImpl::isRMBDown(){
	return m_mouseState.bits.b1;
}

bool gtInputSystemImpl::isMMBDown(){
	return m_mouseState.bits.b2;
}

void gtInputSystemImpl::reset(){
	m_mouseState.bits.b0 = false;
	m_mouseState.bits.b1 = false;
	m_mouseState.bits.b2 = false;
}

void gtInputSystemImpl::setKeyboardState( gtEventKeyboard * e ){
	m_keysDown[ (u32)e->key ] = e->state.bits.b0;
}

void gtInputSystemImpl::setMouseState( gtEventMouse * e ){
	m_mouseState.bits.b0 = e->state.bits.b0;
	m_mouseState.bits.b1 = e->state.bits.b2;
	m_mouseState.bits.b2 = e->state.bits.b4;
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

		for( u32 i = 0u; i < np; ++i ){

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