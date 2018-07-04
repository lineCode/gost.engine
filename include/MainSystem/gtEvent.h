﻿#pragma once
#ifndef __GT_EVENT_H__
#define __GT_EVENT_H__

namespace gost{

	class gtGUIObject;
	class gtWindow;
	struct gtInputDevice;

	constexpr u32 GT_EVENT_WINDOW_SIZING   = gtConst1U;
	constexpr u32 GT_EVENT_WINDOW_RESTORE  = gtConst2U;
	constexpr u32 GT_EVENT_WINDOW_MAXIMIZE = gtConst3U;
	constexpr u32 GT_EVENT_WINDOW_MINIMIZE = gtConst4U;
	constexpr u32 GT_EVENT_WINDOW_MOVE     = gtConst5U;
	constexpr u32 GT_EVENT_WINDOW_PAINT    = gtConst6U;

	constexpr u32 GT_EVENT_JOYSTICK_ADD    = gtConst1U;
	constexpr u32 GT_EVENT_JOYSTICK_REMOVE = gtConst2U;

	constexpr u32 GT_EVENT_SYSTEM_TIMER    = gtConst1U;

	constexpr u32 GT_EVENT_MASK_MOUSE_LMB	= gtConst1U;
	constexpr u32 GT_EVENT_MASK_MOUSE_RMB	= gtConst2U;
	constexpr u32 GT_EVENT_MASK_MOUSE_MMB	= gtConst4U;
	constexpr u32 GT_EVENT_MASK_MOUSE_EXTRA1 = gtConst8U;
	constexpr u32 GT_EVENT_MASK_MOUSE_EXTRA2 = gtConst16U;
	constexpr u32 GT_EVENT_MASK_MOUSE_LMB_DBL = gtConst32U;
	constexpr u32 GT_EVENT_MASK_MOUSE_RMB_DBL = gtConst64U;

	
	enum class gtEventType : u32 {
		None,		
		Keyboard,
		Mouse,
		Joystick,
		GUI,
		Window,
		System
	};

	enum class gtEventGUIAction : u32 {
		MouseEnter,
		MouseMove,
		MouseLeave,
		MouseLeftButtonDown,
		MouseLeftButtonUp,
		MouseLeftButtonDouble,
		MouseRightButtonDown,
		MouseRightButtonUp,
		MouseRightButtonDouble,
		MouseMiddleButtonDown,
		MouseMiddleButtonUp
	};

	struct gtEventGUI{
		u32 id;
		gtEventGUIAction action;
		gtGUIObject* object;
	};

	struct gtEventSystemEvent{
		u32 eventID;
	};
	
	struct gtEventJoystick{
		u32 joystickID;
		u32 joystickEventID;
		gtInputDevice * joystick;
	};

	struct gtEventWindow{
		gtWindow *  window;
		u32			eventID;
	};

	struct gtEventMouse{

		gtEventMouse();

		gtByte	state; /*
							b0 - lmb down
							b1 - lmb up
							b2 - rmb down
							b3 - rmb up
							b4 - mmb down
							b5 - mmb up
							b6 - lmb2x
							b7 - rmb2x
					   */

		gtByte	state_extra; /*
							b0 - ex1 down
							b1 - ex1 up
							b2 - ex2 down
							b3 - ex2 up
							b4 - ex1_2x
							b5 - ex2_2x
					   */

		u16	x;	//< Cursor position on X
		u16 y;	//< Cursor position on Y

		u16 wheel;	//< Wheel

		bool	isLeftButtonDown(){	return state.bits.b0;	}
		bool	isLeftButtonUp()	{	return state.bits.b1;	}
		bool	isRightButtonDown(){	return state.bits.b2;	}
		bool	isRightButtonUp()	{	return state.bits.b3;	}
		bool	isMiddleButtonDown(){	return state.bits.b4;	}
		bool	isMiddleButtonUp(){	return state.bits.b5;	}
		bool	isLeftButtonDouble(){	return state.bits.b6;	}
		bool	isRightButtonDouble(){return state.bits.b7;	}


		bool	isExtra1ButtonDown(){	return state_extra.bits.b0;	}
		bool	isExtra1ButtonUp(){	return state_extra.bits.b1;	}
		bool	isExtra2ButtonDown(){	return state_extra.bits.b2;	}
		bool	isExtra2ButtonUp(){	return state_extra.bits.b3;	}
	};

	struct gtEventKeyboard{

		
		gtKey key = gtKey::K_NONE;
		
		char32_t character = U' ';

		gtByte	state;

		bool isPressed( gtKey Key )	{	return ((Key == key) && state.bits.b0);	}
		bool isReleased( gtKey Key ){	return ((Key == key) && state.bits.b1);	}

		bool isCtrl()			{	return state.bits.b2;					}
		bool isShift()		{	return state.bits.b3;					}
		bool isAlt()			{	return state.bits.b4;					}

	private:
		gtEventKeyboard(){}
	};

		
	struct gtEvent{

		gtEvent(){};

		union{
			gtEventGUI		GUIEvent;
			gtEventKeyboard	keyboardEvent;
			gtEventMouse	mouseEvent;
			gtEventWindow	windowEvent;
			gtEventJoystick joystickEvent;
			gtEventSystemEvent systemEvent;
		};

		gtEventType type = gtEventType::None;
		
		u32 dataSize	= gtConst0U;		
		void* data		= nullptr;

	};
	
		// like event receiver in irrlicht
	class gtEventConsumer{
	public:
			
		virtual void processEvent( const gtEvent& ev ) = 0;

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