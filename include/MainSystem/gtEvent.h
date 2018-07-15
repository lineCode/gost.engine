#pragma once
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
	constexpr u32 GT_EVENT_WINDOW_SIZE     = 7u;

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
		MouseHover,
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
		u32 id              = 0u;
		gtEventGUIAction action;
		gtGUIObject* object = nullptr;
	};

	struct gtEventSystemEvent{
		u32 eventID = 0u;
	};
	
	struct gtEventJoystick{
		u32 joystickID           = 0u;
		u32 joystickEventID      = 0u;
		gtInputDevice * joystick = nullptr;
	};

	struct gtEventWindow{
		gtWindow *  window  = nullptr;
		u32			eventID = 0u;
	};


	struct gtEventMouse{
	
		enum gtEventMouseState{
			MS_LMB_DOWN   = BIT(0),
			MS_LMB_UP     = BIT(1),
			MS_LMB_DOUBLE = BIT(2),
			MS_RMB_DOWN   = BIT(3),
			MS_RMB_UP     = BIT(4),
			MS_RMB_DOUBLE = BIT(5),
			MS_MMB_DOWN   = BIT(6),
			MS_MMB_UP     = BIT(7),
			MS_MMB_DOUBLE = BIT(8),
			MS_X1MB_DOWN  = BIT(9),
			MS_X1MB_UP    = BIT(10),
			MS_X1MB_DOUBLE= BIT(11),
			MS_X2MB_DOWN  = BIT(12),
			MS_X2MB_UP    = BIT(13),
			MS_X2MB_DOUBLE= BIT(14)
		};

		gtEventMouse():state(0u){};

		u32 state;

		u16	x;	//< Cursor position on X
		u16 y;	//< Cursor position on Y

		u16 wheel;	//< Wheel

		bool	isLeftButtonDown()    { return (state&MS_LMB_DOWN);  }
		bool	isLeftButtonUp()	  const { return (state&MS_LMB_UP);    }
		bool	isRightButtonDown()   { return (state&MS_RMB_DOWN);  }
		bool	isRightButtonUp()	  { return (state&MS_RMB_UP);	 }
		bool	isMiddleButtonDown()  { return (state&MS_MMB_DOWN);  }
		bool	isMiddleButtonUp()    { return (state&MS_MMB_UP);	 }
		bool	isExtra1ButtonDown()  { return (state&MS_X1MB_DOWN); }
		bool	isExtra1ButtonUp()    { return (state&MS_X1MB_UP);	 }
		bool	isExtra2ButtonDown()  { return (state&MS_X2MB_DOWN); }
		bool	isExtra2ButtonUp()    { return (state&MS_X2MB_UP);	 }
		bool	isLeftButtonDouble()  { return (state&MS_LMB_DOUBLE);}
		bool	isRightButtonDouble() { return (state&MS_RMB_DOUBLE);}
		bool	isMiddleButtonDouble(){ return (state&MS_MMB_DOUBLE);}
		bool	isExtra1ButtonDouble(){ return (state&MS_X1MB_DOUBLE);}
		bool	isExtra2ButtonDouble(){ return (state&MS_X2MB_DOUBLE);}
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