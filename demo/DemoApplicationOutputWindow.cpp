#include "creator.h"

#ifdef GT_PLATFORM_WIN32

demo::DemoApplicationOutputWindow::DemoApplicationOutputWindow( void ):
m_hWnd( nullptr ),
m_isInit( false ){
#ifdef GT_DEBUG
	this->setDebugName( u"OutputWindow" );
#endif
	init();
}

demo::DemoApplicationOutputWindow::~DemoApplicationOutputWindow( void ){
	shutdown();
}

void	demo::DemoApplicationOutputWindow::init( void ){
	if( !AllocConsole() ){
		return;
	}
	freopen("CONOUT$", "w", stdout);
	m_hWnd = GetConsoleWindow();
	m_isInit = true;
}

void	demo::DemoApplicationOutputWindow::shutdown( void ){
	if( m_isInit )
		if( FreeConsole() )
			m_isInit = false;
}

bool	demo::DemoApplicationOutputWindow::isInit( void ){
	return m_isInit;
}

void	demo::DemoApplicationOutputWindow::show( void ){}
void	demo::DemoApplicationOutputWindow::hide( void ){}

void	demo::DemoApplicationOutputWindow::print( const gtString& s ){
	if( m_isInit )
		wprintf( L"%s\n", (wchar_t*)s.data() );
}

void	demo::DemoApplicationOutputWindow::setWindowText( const gtString& s ){
	if( m_isInit )
		SetWindowText( m_hWnd, (wchar_t*)s.data() );
}

bool	demo::DemoApplicationOutputWindow::isShow( void ){
	return true;
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