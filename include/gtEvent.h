/*!	GOST
	\file gtEvent.h
	\brief Events
*/

#pragma once
#ifndef __GT_EVENT_H__
#define __GT_EVENT_H__ ///< include guard

namespace gost{

#define GT_EVENT_WINDOW_SIZING   1u		///< ID сообщения. Окно меняет размер.
#define GT_EVENT_WINDOW_RESTORE  2u		///< ID сообщения. Окно восстановлено.
#define GT_EVENT_WINDOW_MAXIMIZE 3u		///< ID сообщения. Окно развернуто во весь экран.
#define GT_EVENT_WINDOW_MINIMIZE 4u		///< ID сообщения. Окно свёрнуто.
#define GT_EVENT_WINDOW_MOVE     5u		///< ID сообщения. Окно меняет позицию.
#define GT_EVENT_WINDOW_PAINT    6u		///< ID сообщения. Окно рисует содержимое.

//#define GT_EVENT_MASK_KEY_PRESS 0x80000000	///< самый левый бит хранит состояние нажатия клавиши
//#define GT_EVENT_MASK_KEYS		0x7FFFFFFF  ///< этим макросом игнорируется самый левый бит, получая только код клавиши.

#define GT_EVENT_MASK_MOUSE_LMB	1u			///< была ли нажата левая кнопка мыши
#define GT_EVENT_MASK_MOUSE_RMB	2u			///< была ли нажата правая кнопка мыши
#define GT_EVENT_MASK_MOUSE_MMB	4u			///< была ли нажата средняя кнопка мыши
#define GT_EVENT_MASK_MOUSE_EXTRA1 8u		///< была ли нажата дополнительная кнопка мыши
#define GT_EVENT_MASK_MOUSE_EXTRA2 16u		///< была ли нажата дополнительная кнопка мыши
#define GT_EVENT_MASK_MOUSE_LMB_DBL 32u		///< была ли нажата левая кнопка мыши два раза подряд
#define GT_EVENT_MASK_MOUSE_RMB_DBL 64u		///< была ли нажата правая кнопка мыши два раза подряд

	
		//	Тип события
	enum class gtEventType : u32 {
		none,		///< для завершения обработки массива событий
		keyboard,
		mouse,
		joystick,
		GUI,
		window,
		system
	};

	struct gtEventMouse{

		gtEventMouse( void );

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

		u16	x;	///< Cursor position on X
		u16 y;	///< Cursor position on Y

		u16 wheel;	///< Wheel

		bool	isLeftButtonDown( void ){	return state.bits.b0;	}
		bool	isLeftButtonUp( void )	{	return state.bits.b1;	}
		bool	isRightButtonDown( void ){	return state.bits.b2;	}
		bool	isRightButtonUp( void )	{	return state.bits.b3;	}
		bool	isMiddleButtonDown( void ){	return state.bits.b4;	}
		bool	isMiddleButtonUp( void ){	return state.bits.b5;	}
		bool	isLeftButtonDouble( void ){	return state.bits.b6;	}
		bool	isRightButtonDouble( void ){return state.bits.b7;	}


		bool	isExtra1ButtonDown( void ){	return state_extra.bits.b0;	}
		bool	isExtra1ButtonUp( void ){	return state_extra.bits.b1;	}
		bool	isExtra2ButtonDown( void ){	return state_extra.bits.b2;	}
		bool	isExtra2ButtonUp( void ){	return state_extra.bits.b3;	}
	};

	struct gtEventKeyboard{

		
		gtKey key = gtKey::K_NONE;
		
		char32_t character = U' ';

		gtByte	state;

		bool isPressed( gtKey Key )	{	return ((Key == key) && state.bits.b0);	}
		bool isReleased( gtKey Key ){	return ((Key == key) && state.bits.b1);	}

		/* нужно ли */
		bool isCtrl( void )			{	return state.bits.b2;					}
		bool isShift( void )		{	return state.bits.b3;					}
		bool isAlt( void )			{	return state.bits.b4;					}

	private:
		gtEventKeyboard( void ){}
	};

		///	Структура описывающая событие
	struct gtEvent{

		gtEvent( void ){};

		union{
			gtEventKeyboard	keyboardEvent;
			gtEventMouse	mouseEvent;
		};

		gtEventType type = gtEventType::none;
		
		u32 dataSize	= 0u;		///< размер пользовательских данных
		void* data		= nullptr;	///< пользовательские данные

	};
	
		///	Класс-обработчик событий
	class gtEventConsumer{
	public:
			
			///	Вызывается внутри движка, если есть события
		virtual void processEvent( const gtEvent& ev ) = 0;

	};

}


#endif

/*
Copyright (c) 2017, 2018 532235

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