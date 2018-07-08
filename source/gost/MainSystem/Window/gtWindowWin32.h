#pragma once
#ifndef __GT_WINDOW_WIN_32_H__
#define __GT_WINDOW_WIN_32_H__

#if defined(GT_PLATFORM_WIN32)

namespace gost{


	class gtWindowWin32 GT_FINAL : public gtWindowCommon{
		HWND		m_hWnd;
		gtString	m_className;
		bool		m_isInit;
		DWORD		m_oldWindowStyle;
		RECT        m_oldWindowPosition;

	public:
		gtWindowWin32( gtWindowInfo* );
		virtual ~gtWindowWin32();

		bool	init( u32 i );
		void	setWindowTitle( const gtString& ) GT_FINAL;
		void*	getHandle() GT_FINAL;

		void    switchToFullscreen();
		void    switchToWindow();

		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	};

}

#endif
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
