#include "common.h"

gtInputSystemImpl::gtInputSystemImpl(){
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
	return m_mouseState.isLeftButtonDown();
}

bool gtInputSystemImpl::isRMBDown(){
	return m_mouseState.isRightButtonDown();
}

bool gtInputSystemImpl::isMMBDown(){
	return m_mouseState.isMiddleButtonDown();
}

void gtInputSystemImpl::reset(){
	m_mouseState.state = 0u;
}

void gtInputSystemImpl::setKeyboardState( gtEventKeyboard * e ){
	m_keysDown[ (u32)e->key ] = e->state.bits.b0;
}

void gtInputSystemImpl::setMouseState( gtEventMouse * e ){
	m_mouseState = *e;
	m_cursorPosition.x = e->x;
	m_cursorPosition.y = e->y;
}

gtPtr<gtInputController> gtInputSystemImpl::createInputContoller( const GT_GUID& uid ){
	gtPluginCommon * plugin = nullptr;
	gtPluginInput * pluginInput = nullptr;
	
	auto * ps = gtMainSystem::getInstance()->getPluginSystem();
	plugin = (gtPluginCommon*)ps->getPlugin( uid );

	if( !plugin ){
		u32 np = ps->getNumOfPlugins();

		for( u32 i = 0u; i < np; ++i ){

			auto * pl = (gtPluginCommon*)ps->getPlugin( i );

			if( pl->getInfo().m_info.m_type == gtPluginType::Input ){
			
				pluginInput = (gtPluginInput *)pl->getImplementation();

				auto ret = pluginInput->loadInputDriver();

				if( ret ){

					return gtPtr<gtInputController>( ret );
				}
			}
		}

		return nullptr;
	}
	pluginInput = (gtPluginInput *)plugin;
	return gtPtrNew<gtInputController>( pluginInput->loadInputDriver() );
}