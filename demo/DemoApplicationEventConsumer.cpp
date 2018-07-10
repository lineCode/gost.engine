#include "creator.h"

demo::DemoApplicationEventConsumer::DemoApplicationEventConsumer( DemoApplicationContext c ):m_context( c ){
	memset( m_keys, 0, 256u );
}
demo::DemoApplicationEventConsumer::~DemoApplicationEventConsumer(){
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
	switch (ev.GUIEvent.action){
	case gtEventGUIAction::MouseMove:{
		if( *m_context.demoState == DemoState::MainMenu ){
			if( ev.GUIEvent.id > 99u && ev.GUIEvent.id < 199u ){
				*m_context.activeDemoTypeSelected = ev.GUIEvent.id - 100u;
				*m_context.currentDemoColonIndex = *m_context.activeDemoSelected = *m_context.activeDemoType = 0;
				m_context.app->RefreshGUI();

			}else if( ev.GUIEvent.id > 199u && ev.GUIEvent.id < 299u ){
				*m_context.activeDemoSelected = ev.GUIEvent.id - 200u;
				*m_context.currentDemoColonIndex = *m_context.activeDemoSelected;
				*m_context.activeDemoType = 1;
				m_context.app->RefreshGUI();

			}else if( ev.GUIEvent.id == DEMO_GUI_ID_PAUSE_CONTINUE ){
				auto text = (gtGUITextField*)ev.GUIEvent.object;
				if( text ){
					*m_context.pauseMainMenuSelectedId = 0;
					m_context.app->RefreshGUI();
				}

			}else if( ev.GUIEvent.id == DEMO_GUI_ID_PAUSE_SETTINGS ){
				auto text = (gtGUITextField*)ev.GUIEvent.object;
				if( text ){
					*m_context.pauseMainMenuSelectedId = 1;
					m_context.app->RefreshGUI();
				}
			}else if( ev.GUIEvent.id == DEMO_GUI_ID_PAUSE_EXIT ){
				auto text = (gtGUITextField*)ev.GUIEvent.object;
				if( text ){
					*m_context.pauseMainMenuSelectedId = 2;
					m_context.app->RefreshGUI();
				}
			}
		}
	}break;
	case gtEventGUIAction::MouseEnter:{
		if( *m_context.demoState == DemoState::MainMenu ){
			if( ev.GUIEvent.id == DEMO_GUI_ID_WELCOME_TEXT ){
				auto text = (gtGUITextField*)ev.GUIEvent.object;
				if( text ){
					text->getBackgroundShape()->setTransparent( 0.f );
					text->getBackgroundShape()->setColor( gtColorBlack );
				}
			}
		}else if( *m_context.demoState == DemoState::DemoMenu ){
			if( ev.GUIEvent.id == DEMO_GUI_ID_PAUSE_CONTINUE ){
				auto text = (gtGUITextField*)ev.GUIEvent.object;
				if( text ){
					*m_context.demoPauseMenuID = 0;
					m_context.app->RefreshGUI();
				}

			}else if( ev.GUIEvent.id == DEMO_GUI_ID_PAUSE_EXIT ){
				auto text = (gtGUITextField*)ev.GUIEvent.object;
				if( text ){
					*m_context.demoPauseMenuID = 2;
					m_context.app->RefreshGUI();
				}
			}
		}
	}break;
	case gtEventGUIAction::MouseLeave:{
		if( *m_context.demoState == DemoState::MainMenu ){
			if( ev.GUIEvent.id == DEMO_GUI_ID_WELCOME_TEXT ){
				auto text = (gtGUITextField*)ev.GUIEvent.object;
				if( text ){
					text->getBackgroundShape()->setTransparent( 1.f );
				}
			}
		}
	}break;
	case gtEventGUIAction::MouseLeftButtonDown:{
		if( *m_context.demoState == DemoState::MainMenu ){
			if( ev.GUIEvent.id == DEMO_GUI_ID_WELCOME_TEXT ){
				auto text = (gtGUITextField*)ev.GUIEvent.object;
				if( text ){
					text->getBackgroundShape()->setColor( gtColorGreen );
				}
			}
		}
	}break;
	case gtEventGUIAction::MouseLeftButtonUp:{
		if( *m_context.demoState == DemoState::MainMenu
			|| *m_context.demoState == DemoState::DemoMenu ){
			if( ev.GUIEvent.id == DEMO_GUI_ID_WELCOME_TEXT ){
				auto text = (gtGUITextField*)ev.GUIEvent.object;
				if( text ){
					text->getBackgroundShape()->setColor( gtColorBlack );
				}
			}else if( ev.GUIEvent.id == DEMO_GUI_ID_PAUSE_CONTINUE ){
				m_context.app->ReturnToMainMenu();
			}else if( ev.GUIEvent.id == DEMO_GUI_ID_PAUSE_EXIT ){
				m_context.app->TerminateProgram();
			}
		}
	}break;
	}
}

void demo::DemoApplicationEventConsumer::processEventJoystick( const gtEvent& ev ){
	switch( ev.joystickEvent.joystickEventID ){
	case GT_EVENT_JOYSTICK_ADD:{
		gtLogWriter::printInfo( u"Add gamepad: %s %u\n", ev.joystickEvent.joystick->name.data(), ev.joystickEvent.joystickID );
		m_context.app->ActivateGamepad( true, ev.joystickEvent.joystick );
	}break;
	case GT_EVENT_JOYSTICK_REMOVE:{
		m_context.app->ActivateGamepad( false );
	}break;
	}
}

void demo::DemoApplicationEventConsumer::processEventKeyboard( const gtEvent& ev ){
	m_keys[ (u32)ev.keyboardEvent.key ] = ev.keyboardEvent.state.bits.b0;
}

void demo::DemoApplicationEventConsumer::processEventMouse( const gtEvent& ev ){
}

void demo::DemoApplicationEventConsumer::processEventSystem( const gtEvent& ev ){
	switch( ev.systemEvent.eventID ){
	case GT_EVENT_SYSTEM_TIMER:{
		m_context.app->ScanGamepads();
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
	case GT_EVENT_WINDOW_SIZE:{
		m_context.app->RebuildGUI();
	}break;
	}
}

bool demo::DemoApplicationEventConsumer::keyDownOnce( gtKey key ){
	bool v = m_keys[ (u32)key ];
	m_keys[ (u32)key ] = false;
	return v;
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