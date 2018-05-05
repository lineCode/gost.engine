#pragma once
#ifndef __GT_OUTPUT_WINDOW_WIN32_H__
#define __GT_OUTPUT_WINDOW_WIN32_H__

#if defined(GT_PLATFORM_WIN32)

namespace gost{

	class gtOutputWindowWin32 GT_FINAL : public gtOutputWindow {

		bool		m_isInit;
		bool		m_isShown;
		HWND		m_hWnd;
		WNDCLASS	m_wc;
		RECT		m_windowRect;
		
		static LRESULT CALLBACK
					OutWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
		void		clear_buffer( void );
		void		save( void );

	public:

		gtOutputWindowWin32( void );
		virtual ~gtOutputWindowWin32( void );

		void	init( void ) GT_FINAL;
		void	shutdown( void ) GT_FINAL;
		bool	isInit( void ) GT_FINAL;
		void	show( void ) GT_FINAL;
		void	hide( void ) GT_FINAL;
		void	print( const gtString& ) GT_FINAL;
		void	setWindowText( const gtString& ) GT_FINAL;
		bool	isShow( void );

		HWND	m_hWndBuffer;
		HBRUSH	m_hbrEditBackground;
	};

}
#endif
#endif

/*
Copyright (c) 2017-2018

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
