//	GOST

#include"common.h"

gtEventSystem::gtEventSystem( gtEventConsumer* uc ) :
	m_userConsumer( uc ),
	m_numOfEvents( 0u ),
	m_currentEvent( 0u )
{
	memset( m_events, 0, EventMax * sizeof( gtEvent ) );
	memset( m_keysDown, 0, 256u );
	m_mouseState.byte = 0u;
}

gtEventSystem::~gtEventSystem( void ){
}

gtEventConsumer* gtEventSystem::getConsumer( void ){
	return m_userConsumer;
}

void gtEventSystem::resetEvents( void ){
	m_numOfEvents = 0u;
}

bool gtEventSystem::isKeyDown( gtKey key ){
	return m_keysDown[ (u32)key ];
}

void gtEventSystem::getKeyboardAndMouseStates( void ){
	u32 cur = m_currentEvent;
	if( m_numOfEvents ){
		while( true ){
			if( m_events[ cur ].type == gtEventType::None ){
				break;
			}

			m_mouseState.bits.b0 = false;
			m_mouseState.bits.b1 = false;
			m_mouseState.bits.b2 = false;

			switch( m_events[ cur ].type ){
				case gtEventType::Keyboard:{
					m_keysDown[ (u32)m_events[ cur ].keyboardEvent.key ] = m_events[ cur ].keyboardEvent.state.bits.b0;
				}break;
				case gtEventType::Mouse:{
					m_mouseState.bits.b0 = m_events[ cur ].mouseEvent.state.bits.b0;
					m_mouseState.bits.b1 = m_events[ cur ].mouseEvent.state.bits.b2;
					m_mouseState.bits.b2 = m_events[ cur ].mouseEvent.state.bits.b4;
					m_cursorPosition.x = m_events[ cur ].mouseEvent.x;
					m_cursorPosition.y = m_events[ cur ].mouseEvent.y;
				}break;
				case gtEventType::None:
				case gtEventType::Joystick:
				case gtEventType::GUI:
				case gtEventType::Window:
				case gtEventType::System:
				break;
			}
			++cur;
		}
	}
}

bool gtEventSystem::pollEvent( gtEvent& event ){
	if( m_events[ m_currentEvent ].type == gtEventType::None )
		return false;

	event = m_events[ m_currentEvent ];
	m_events[ m_currentEvent ].type = gtEventType::None;
	++m_currentEvent;

	if( m_currentEvent == m_numOfEvents )
			m_currentEvent = 0u;

	return true;
}

void gtEventSystem::runEventLoop( void ){

	while( true ){

		if( m_events[ m_currentEvent ].type == gtEventType::None )
			break;

		if( m_userConsumer )
			m_userConsumer->processEvent( m_events[ m_currentEvent ] );

		m_events[ m_currentEvent ].type = gtEventType::None;


		++m_currentEvent;

		if( m_currentEvent == m_numOfEvents )
			m_currentEvent = 0u;
	}

}

void gtEventSystem::addEvent( const gtEvent& ev, u8 prior ){
	if( m_numOfEvents < EventMax ){

		if( prior ){
			//m_events[ m_currentEvent + prior-1u ] = ev;
		}else{
		}

		m_events[ m_numOfEvents ] = ev;
		m_numOfEvents++;
	}
}

bool gtEventSystem::isLMBDown( void ){
	return m_mouseState.bits.b0;
}

bool gtEventSystem::isRMBDown( void ){
	return m_mouseState.bits.b1;
}

bool gtEventSystem::isMMBDown( void ){
	return m_mouseState.bits.b2;
}

const gtVector2<u16>& gtEventSystem::getCursorPosition( void ){
	return m_cursorPosition;
}

//	=============================================================

gtEngineEventConsumer::gtEngineEventConsumer(){
}

gtEngineEventConsumer::~gtEngineEventConsumer(){
}

void gtEngineEventConsumer::processEvent( const gtEvent& /*ev*/ ){

}

/*
Copyright (c) 2017 532235

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
