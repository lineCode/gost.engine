#include "creator.h"

demo::DemoApplicationEventConsumer::DemoApplicationEventConsumer( void ){
}
demo::DemoApplicationEventConsumer::~DemoApplicationEventConsumer( void ){
}

void demo::DemoApplicationEventConsumer::processEvent( const gtEvent& ev ){
	switch( ev.type ){
		case gtEventType::GUI:{
			processEventGUI( ev );
		}break;
		case gtEventType::Joystick:{
			processEventJoystick( ev );
		}break;
		case gtEventType::Keyboard:{
			processEventKeyboard( ev );
		}break;
		case gtEventType::Mouse:{
			processEventMouse( ev );
		}break;
		case gtEventType::System:{
			processEventSystem( ev );
		}break;
		case gtEventType::Window:{
			processEventWindow( ev );
		}break;
	}
}

void demo::DemoApplicationEventConsumer::processEventGUI( const gtEvent& ev ){
}

void demo::DemoApplicationEventConsumer::processEventJoystick( const gtEvent& ev ){
	switch( ev.joystickEvent.joystickEventID ){
	case GT_EVENT_JOYSTICK_ADD:{
	}break;
	case GT_EVENT_JOYSTICK_REMOVE:{
	}break;
	}
}

void demo::DemoApplicationEventConsumer::processEventKeyboard( const gtEvent& ev ){
}

void demo::DemoApplicationEventConsumer::processEventMouse( const gtEvent& ev ){
}

void demo::DemoApplicationEventConsumer::processEventSystem( const gtEvent& ev ){
	switch( ev.systemEvent.eventID ){
	case GT_EVENT_SYSTEM_TIMER:{
	}break;
	}
}

void demo::DemoApplicationEventConsumer::processEventWindow( const gtEvent& ev ){
	switch( ev.windowEvent.eventID ){
	case GT_EVENT_WINDOW_MAXIMIZE:{
	}break;
	case GT_EVENT_WINDOW_MINIMIZE:{
	}break;
	case GT_EVENT_WINDOW_MOVE:{
	}break;
	case GT_EVENT_WINDOW_PAINT:{
	}break;
	case GT_EVENT_WINDOW_RESTORE:{
	}break;
	case GT_EVENT_WINDOW_SIZING:{
	}break;
	}
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