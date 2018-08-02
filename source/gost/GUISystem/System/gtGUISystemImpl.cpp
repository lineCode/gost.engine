#include "common.h"

#include "GUISystem/BuiltInFont.hpp"

gtGUISystemImpl::gtGUISystemImpl():
	m_mainSystem( nullptr ),
	m_inputSystem( nullptr ),
	m_gs( nullptr )
{
	m_mainSystem = gtMainSystem::getInstance();
	m_inputSystem = m_mainSystem->getInputSystem();
}

gtGUISystemImpl::~gtGUISystemImpl(){
}

void gtGUISystemImpl::addToUserInput( gtGUIObject * o, u32 id ){
	auto sz = m_userInputObjects.size();
	for( auto i = 0u; i < sz; ++i ){
		if(m_userInputObjects[ i ].m_second == id){
			m_userInputObjects[ i ].m_first = o;
			return;
		}
	}

	m_userInputObjects.push_back( gtPair<gtGUIObject*,u32>(o, id) );
}

void gtGUISystemImpl::removeFromUserInput( gtGUIObject * o ){
	auto sz = m_userInputObjects.size();
	for( auto i = 0u; i < sz; ++i ){
		if( m_userInputObjects[ i ].m_first == o ){
			m_userInputObjects.erase( i );
			break;
		}
	}
}

bool gtGUISystemImpl_compareEvent_callback_lmbDown( gtEvent& current_event, gtEvent& /*user_event*/ ){ return current_event.mouseEvent.isLeftButtonDown(); }
bool gtGUISystemImpl_compareEvent_callback_rmbDown( gtEvent& current_event, gtEvent& /*user_event*/ ){ return current_event.mouseEvent.isRightButtonDown(); }
bool gtGUISystemImpl_compareEvent_callback_mmbDown( gtEvent& current_event, gtEvent& /*user_event*/ ){ return current_event.mouseEvent.isMiddleButtonDown(); }
bool gtGUISystemImpl_compareEvent_callback_lmbUp( gtEvent& current_event, gtEvent& /*user_event*/ )  { return current_event.mouseEvent.isLeftButtonUp();   }
bool gtGUISystemImpl_compareEvent_callback_rmbUp( gtEvent& current_event, gtEvent& /*user_event*/ )  { return current_event.mouseEvent.isRightButtonUp();   }
bool gtGUISystemImpl_compareEvent_callback_mmbUp( gtEvent& current_event, gtEvent& /*user_event*/ )  { return current_event.mouseEvent.isMiddleButtonUp();   }
bool gtGUISystemImpl_compareEvent_callback_lmbDouble( gtEvent& current_event, gtEvent& /*user_event*/ ){ return current_event.mouseEvent.isLeftButtonDouble(); }
bool gtGUISystemImpl_compareEvent_callback_rmbDouble( gtEvent& current_event, gtEvent& /*user_event*/ ){ return current_event.mouseEvent.isRightButtonDouble(); }



void gtGUISystemImpl::updateInput(){
	m_coords = m_inputSystem->getCursorPosition();
	static auto oldCursorPosition = m_coords;

	//printf("%i\t\t%i\n",m_coords.x,m_coords.y);
	auto sz = m_userInputObjects.size();
	for( auto i = 0u; i < sz; ++i ){


		auto o = m_userInputObjects[ i ];
		if( o.m_first->isVisible() ){


			if( util::pointInRect(m_coords,o.m_first->getActiveArea()) ){

				bool has = false;
				for( auto k = 0u; k < i; ++k ){
					if( util::pointInRect(m_coords,m_userInputObjects[ k ].m_first->getActiveArea()) ){
						has = true;
						break;
					}
				}
				if( has ) continue;

				if( !o.m_first->isMouseEnter() ){
					gtEvent e;
					e.type = gtEventType::GUI;
					e.GUIEvent.id = o.m_second;
					e.GUIEvent.action = gtEventGUIAction::MouseEnter;
					e.GUIEvent.object = o.m_first;
					m_mainSystem->addEvent( e );
					o.m_first->setMouseEnter();
				}

				gtEvent findEvent;
				findEvent.type = gtEventType::Mouse;

				gtEvent em;
				em.type = gtEventType::GUI;
				em.GUIEvent.id = o.m_second;
				em.GUIEvent.object = o.m_first;

				if( m_mainSystem->checkEvent( findEvent, gtGUISystemImpl_compareEvent_callback_lmbDown ) ){
					em.GUIEvent.action = gtEventGUIAction::MouseLeftButtonDown;
					m_mainSystem->addEvent( em );
				}else if( m_mainSystem->checkEvent( findEvent, gtGUISystemImpl_compareEvent_callback_lmbUp ) ){
					em.GUIEvent.action = gtEventGUIAction::MouseLeftButtonUp;
					m_mainSystem->addEvent( em );
				}else if( m_mainSystem->checkEvent( findEvent, gtGUISystemImpl_compareEvent_callback_lmbDouble ) ){
					em.GUIEvent.action = gtEventGUIAction::MouseLeftButtonDouble;
					m_mainSystem->addEvent( em );
				}else if( m_mainSystem->checkEvent( findEvent, gtGUISystemImpl_compareEvent_callback_rmbDown ) ){
					em.GUIEvent.action = gtEventGUIAction::MouseRightButtonDown;
					m_mainSystem->addEvent( em );
				}else if( m_mainSystem->checkEvent( findEvent, gtGUISystemImpl_compareEvent_callback_rmbUp ) ){
					em.GUIEvent.action = gtEventGUIAction::MouseRightButtonUp;
					m_mainSystem->addEvent( em );
				}else if( m_mainSystem->checkEvent( findEvent, gtGUISystemImpl_compareEvent_callback_rmbDouble ) ){
					em.GUIEvent.action = gtEventGUIAction::MouseRightButtonDouble;
					m_mainSystem->addEvent( em );
				}else if( m_mainSystem->checkEvent( findEvent, gtGUISystemImpl_compareEvent_callback_mmbDown ) ){
					em.GUIEvent.action = gtEventGUIAction::MouseMiddleButtonDown;
					m_mainSystem->addEvent( em );
				}else if( m_mainSystem->checkEvent( findEvent, gtGUISystemImpl_compareEvent_callback_mmbUp ) ){
					em.GUIEvent.action = gtEventGUIAction::MouseMiddleButtonUp;
					m_mainSystem->addEvent( em );
				}

				gtEvent e;
				e.type = gtEventType::GUI;
				e.GUIEvent.id = o.m_second;
				e.GUIEvent.action = oldCursorPosition == m_coords ? gtEventGUIAction::MouseHover : gtEventGUIAction::MouseMove;
				e.GUIEvent.object = o.m_first;

				m_mainSystem->addEvent( e );
			}else{
				if( o.m_first->isMouseEnter() ){
					gtEvent e;
					e.type = gtEventType::GUI;
					e.GUIEvent.id = o.m_second;
					e.GUIEvent.action = gtEventGUIAction::MouseLeave;
					e.GUIEvent.object = o.m_first;
					m_mainSystem->addEvent( e );
					o.m_first->setMouseLeave();
				}
			}

		}
	}

	oldCursorPosition = m_coords;
}

void gtGUISystemImpl::clearUserInput(){
	m_userInputObjects.clear();
}

void gtGUISystemImpl::init(){
}

void gtGUISystemImpl::setCurrentRenderDriver( gtGraphicsSystem * driver ){
	m_gs = driver;
}


gtPtr<gtGUIMenu> gtGUISystemImpl::createMenu( s32 height, gtGUIFont* font ){
	gtPtr_t( gtGUIMenuImpl, st, new gtGUIMenuImpl( m_gs, font ) );

	if( !st.data() )
		return nullptr;

	if( !st->_init( height ) ){
		gtLogWriter::printWarning( u"Can not create gtGUIMenu" );
		return nullptr;
	}

	return gtPtr<gtGUIMenu>( st.data() );
}

gtPtr<gtGUIShape> gtGUISystemImpl::createShapeRectangle( const v4i& rect, const gtColor& color, bool useGradient, 
			const gtColor& first_color, const gtColor& second_color, bool useVerticalGradient ){
	gtPtr_t( gtGUIShapeImpl, st, new gtGUIShapeImpl( m_gs ) );

	if( !st.data() )
		return nullptr;

	if( !st->initRectangle( rect, color, useGradient, first_color, second_color, useVerticalGradient ) ){
		gtLogWriter::printWarning( u"Can not create gtGUIShape" );
		return nullptr;
	}

	return gtPtr<gtGUIShape>( st.data() );
}

gtPtr<gtGUIFont> gtGUISystemImpl::createFont( const gtString& fontName, gtImage * fromImage ){

	gtPtr_t( gtGUIFontImpl, font, new gtGUIFontImpl( m_gs ) );
	if( !font.data() ){
		return nullptr;
	}

	if( !font->init( fontName, fromImage ) ){
		gtLogWriter::printWarning( u"Can not create font \"%s\"", fontName.c_str() );
		return nullptr;
	}

	//font->addRef();

	return gtPtr<gtGUIFont>( font.data() );
}

gtPtr<gtGUIFont> gtGUISystemImpl::createBuiltInFont(){
	gtImage * fontImage = new gtImage;

	fontImage->bits = gtConst1U;
	fontImage->dataSize = 65536;
		
//	util::memoryAllocate( fontImage->data, fontImage->dataSize );
	fontImage->data = (u8*)gtMemAlloc(fontImage->dataSize);

	memcpy( fontImage->data, gtBuiltInFontBytes, fontImage->dataSize);

	fontImage->format = gtImageFormat::One_bit;
	fontImage->height = 512u;
	fontImage->width  = 1024u;
	fontImage->pitch  = 1024u;
	fontImage->convert( gtImageFormat::R8G8B8A8 );
	fontImage->flipVertical();

	const char16_t * xml = reinterpret_cast<const char16_t *>( &gtBuiltInFontXML[ gtConst0U ] );

	auto font = createFont( gtString( xml ), fontImage );
	fontImage->release();

	if( !font.data() ){
		gtLogWriter::printError( u"font == nullptr" );
		return nullptr;
	}

	return font;
}

gtPtr<gtGUITextField>	gtGUISystemImpl::createTextField( const v4i& rect, gtGUIFont* font, bool fh, bool fw ){
	gtPtr_t( gtGUITextFieldImpl, tf, new gtGUITextFieldImpl( m_gs ) );
	if( !tf.data() )
		return nullptr;
	if( !tf->init( rect, font, fh, fw ) ){
		gtLogWriter::printWarning( u"Can not create static text " );
		return nullptr;
	}
	return gtPtr<gtGUITextField>( tf.data() );
}

gtPtr<gtGUIStaticText> gtGUISystemImpl::createStaticText( const gtString& text, s32 positionX, s32 positionY, gtGUIFont* font ){
	gtPtr_t( gtGUIStaticTextImpl, st, new gtGUIStaticTextImpl( m_gs ) );
	if( !st.data() )
		return nullptr;
	if( !st->init( text, positionX, positionY, font ) ){
		gtLogWriter::printWarning( u"Can not create static text " );
		return nullptr;
	}
	return gtPtr<gtGUIStaticText>( st.data() );
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