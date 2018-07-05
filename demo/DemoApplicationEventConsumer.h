#pragma once
#ifndef __GT_DEMO_EVENTS_H__
#define __GT_DEMO_EVENTS_H__

namespace demo{

	class DemoApplication;
	enum class DemoState : u32;

	struct DemoApplicationContext{
		DemoApplication *	app;
		s32 * activeDemoTypeSelected;
		s32 * activeDemoType;
		s32 * activeDemoSelected;
		s32 * currentDemoColonIndex;
		s32 * pauseMainMenuSelectedId;
		s16 * demoPauseMenuID;
		s32   demoArraySize;
		DemoState * demoState;
		v4i   oldWindowSize;
	};

	class DemoApplicationEventConsumer : public gtEventConsumer {
		bool	m_keys[ 256u ];
	public:
		DemoApplicationEventConsumer( DemoApplicationContext c );
		~DemoApplicationEventConsumer();

		DemoApplicationContext  m_context;

		void processEvent( const gtEvent& ev );

		void processEventGUI( const gtEvent& ev );
		void processEventJoystick( const gtEvent& ev );
		void processEventKeyboard( const gtEvent& ev );
		void processEventMouse( const gtEvent& ev );
		void processEventSystem( const gtEvent& ev );
		void processEventWindow( const gtEvent& ev );

		bool keyDown( gtKey key );

	};

}

#endif


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