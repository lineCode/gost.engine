#pragma once
#ifndef __GT_EVENT_SYSTEM_H__
#define __GT_EVENT_SYSTEM_H__

namespace gost{

	constexpr u32 EventMax = 128u;

	class gtEventSystem : public gtRefObject{


		gtEventConsumer* m_userConsumer;

		gtEvent			m_events[ EventMax ];
		u32				m_numOfEvents;
		u32				m_currentEvent;
		/*
		bool			m_keysDown[ 256u ];
		gtVector2<u16>	m_cursorPosition;
		gtByte			m_mouseState;
		*/

		gtInputSystemImpl * m_input;

	public:

		gtEventSystem( gtInputSystemImpl * input, gtEventConsumer* uc = nullptr );
		virtual ~gtEventSystem();

		void				getKeyboardAndMouseStates();

		gtEventConsumer*	getConsumer();

		void				runEventLoop();

		bool				pollEvent( gtEvent& event );
	//	bool				isKeyDown( gtKey key );
		void				resetEvents();

		void				addEvent( const gtEvent&, u8 prior );

	//	bool				isLMBDown();
	//	bool				isRMBDown();
	//	bool				isMMBDown();
	//	const gtVector2<u16>& getCursorPosition();
	};

	//	=============================================================

	class gtEngineEventConsumer : public gtEventConsumer{
	public:

		gtEngineEventConsumer();
		virtual ~gtEngineEventConsumer();
		
		void processEvent( const gtEvent& ev );

	};

}


#endif

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