#pragma once
#ifndef __GT_INPUT_SYSTEM_IMPL_H__
#define __GT_INPUT_SYSTEM_IMPL_H__

namespace gost{

	class gtInputSystemImpl : public gtInputSystem{
		bool			m_keysDown[ gtConst256U ];
		gtVector2<s16>	m_cursorPosition;
		gtByte			m_mouseState;
	public:
		gtInputSystemImpl();
		virtual ~gtInputSystemImpl();

		gtPtr<gtInputController> createInputContoller( const GT_GUID& plugin );
		bool isKeyDown( gtKey key );
		const gtVector2<s16>& getCursorPosition();
		bool isLMBDown();
		bool isRMBDown();
		bool isMMBDown();

		void reset();
		void setKeyboardState( gtEventKeyboard * e );
		void setMouseState( gtEventMouse * e );
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