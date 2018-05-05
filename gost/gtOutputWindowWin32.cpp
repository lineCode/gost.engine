//	GOST

#include "common.h"


#if defined(GT_PLATFORM_WIN32)

#define GT_MENU_ID_OUTPUT_FILE_SAVE 1
#define GT_MENU_ID_OUTPUT_EDIT_CLEAR 2
#define GT_MENU_ID_OUTPUT_VIEW_HIDE 3

gtOutputWindowWin32::gtOutputWindowWin32( void ) : m_isInit( false ), m_isShown( true ),
m_hWnd( nullptr ){
	memset( &m_wc, 0, sizeof( m_wc ) );
}

gtOutputWindowWin32::~gtOutputWindowWin32( void ){
	shutdown();
}

void	gtOutputWindowWin32::init( void ){

	if( m_isInit ) return;

	m_wc.style		   = CS_HREDRAW | CS_VREDRAW;
	m_wc.lpfnWndProc   = ( WNDPROC )OutWndProc;
	m_wc.hInstance     = GetModuleHandle( 0 );
	m_wc.hIcon         = NULL;//LoadIcon( wc.hInstance, MAKEINTRESOURCE(IDI_ICON1));
	m_wc.hCursor       = LoadCursor (NULL,IDC_ARROW);
	m_wc.hbrBackground = CreateSolidBrush(RGB(104,104,104));
	m_wc.lpszMenuName  = NULL;
	m_wc.lpszClassName = L"OUTPUTWINDOW";


	if( !RegisterClass (&m_wc) ){
		MessageBox( 0, L"Can not initialize Output Window", L"Error", MB_ICONERROR );
		return;
	}

	u32 style = WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX
		| WS_CLIPCHILDREN;

	m_windowRect.left = 20;
	m_windowRect.top = 20;
	m_windowRect.right = 800;
	m_windowRect.bottom = 600;

	m_hWnd = CreateWindowEx( NULL, m_wc.lpszClassName,
		L"Output", style,
		m_windowRect.left,
		m_windowRect.top,
		m_windowRect.right,
		m_windowRect.bottom,
		NULL, NULL, m_wc.hInstance,
		this );

	if( !m_hWnd ){
		MessageBox( 0, L"Can not create Output Window", L"Error", MB_ICONERROR );
		return;
	}

	HMENU menu = CreateMenu();
	HMENU menuitem = CreateMenu();
	HMENU menusubitem = CreateMenu();


	AppendMenu( menuitem, MF_STRING, GT_MENU_ID_OUTPUT_FILE_SAVE, L"&Save" );
	//AppendMenu( menuitem, MF_STRING, GT_MENU_ID_OUTPUT_FILE_SAVE, L"&Open" );

	AppendMenu( menusubitem, MF_STRING, 1, L"&Open" );
	AppendMenu( menusubitem, MF_SEPARATOR, 0, 0 );
	AppendMenu( menusubitem, MF_STRING, 2, L"file.txt" );
	AppendMenu( menusubitem, MF_STRING, 3, L"file2.txt" );
	AppendMenu( menuitem, MF_POPUP, (UINT_PTR)menusubitem, L"&Open" );

	AppendMenu( menu, MF_POPUP, (UINT_PTR)menuitem, L"&File" );




	menuitem = CreateMenu();
	AppendMenu( menuitem, MF_STRING, GT_MENU_ID_OUTPUT_EDIT_CLEAR, L"&Clear" );
	AppendMenu( menu, MF_POPUP, (UINT_PTR)menuitem, L"&Edit" );

	menuitem = CreateMenu();
	AppendMenu( menuitem, MF_STRING, GT_MENU_ID_OUTPUT_VIEW_HIDE, L"&Hide" );
	AppendMenu( menu, MF_POPUP, (UINT_PTR)menuitem, L"&View" );

	SetMenu( m_hWnd, menu );

	HDC hDC = GetDC( m_hWnd );
	int nHeight = -MulDiv( 10, GetDeviceCaps( hDC, LOGPIXELSY ), 72 );

	const auto * params = &gtMainSystem::getInstance()->getDeviceCreationParameters();


	std::wstring wstr(L"Consolas");
	if( !AddFontResource( (wchar_t*)params->m_fontName.c_str() ) ){
		wstr.assign(L"Courier New");
	}
	HFONT hfBufferFont = CreateFont( nHeight, 0, 0, 0, FW_LIGHT, 0, 0, 0,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, FF_MODERN | FIXED_PITCH, wstr.data() );
	if( !hfBufferFont ){
		hfBufferFont = CreateFont( nHeight, 0, 0, 0, FW_LIGHT, 0, 0, 0,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY, FF_MODERN | FIXED_PITCH, L"Courier New" );
	}


	ReleaseDC( m_hWnd, hDC );
	m_hWndBuffer = CreateWindow( L"edit", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER |
									ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
									1, 1, m_windowRect.right-20,
									m_windowRect.bottom-100,
									m_hWnd, 0, m_wc.hInstance, NULL );
	SendMessage( m_hWndBuffer, WM_SETFONT, ( WPARAM ) hfBufferFont, 0 );
	m_hbrEditBackground = CreateSolidBrush(RGB(114,114,114));

	m_isInit = true;
}


void	gtOutputWindowWin32::shutdown( void ){
	DeleteObject( m_wc.hbrBackground );
	DeleteObject( m_hbrEditBackground );
	RemoveFontResource( L"consola.TTF" );
	DestroyWindow( m_hWnd );
	m_hWnd = nullptr;
	UnregisterClass( m_wc.lpszClassName, m_wc.hInstance );
	m_isInit = false;
}


bool	gtOutputWindowWin32::isInit( void ){
	return m_isInit;
}


void	gtOutputWindowWin32::show( void ){
	if( m_isInit ){
		ShowWindow( m_hWnd, SW_SHOWNORMAL );
		UpdateWindow( m_hWnd );
		m_isShown = true;
	}
}


void	gtOutputWindowWin32::hide( void ){
	if( m_isInit )
		ShowWindow( m_hWnd, SW_HIDE );
	m_isShown = false;
}


void	gtOutputWindowWin32::print( const gtString& text ){
	if( m_isShown ){

		static unsigned long s_totalChars;

		s_totalChars += text.size();

		if ( s_totalChars > 0x7000 ) {
			SendMessage( m_hWndBuffer, EM_SETSEL, 0, -1 );
			s_totalChars = text.size();
		}

		SendMessage( m_hWndBuffer, EM_LINESCROLL, 0, 0xffff );
		SendMessage( m_hWndBuffer, EM_SCROLLCARET, 0, 0 );
		SendMessage( m_hWndBuffer, EM_REPLACESEL, 0, (LPARAM) text.data() );
		wchar_t f[3]={L'\r',L'\n',0};
		SendMessage( m_hWndBuffer, EM_REPLACESEL, 0, (LPARAM) f );

	}
}

bool	gtOutputWindowWin32::isShow( void ){
	return m_isShown;
}


void	gtOutputWindowWin32::setWindowText( const gtString& text ){
	if( m_isInit )
		SetWindowText( m_hWnd, (wchar_t*)text.data() );
}


LRESULT CALLBACK gtOutputWindowWin32::OutWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam ){


	gtOutputWindowWin32*	output = nullptr;
	if( !output ){
		if( uMsg == WM_NCCREATE ){
			output = static_cast<gtOutputWindowWin32*>( reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams );
			SetLastError(0);
			if (!SetWindowLongPtr(hWnd, -21, reinterpret_cast<LONG_PTR>( output )))
				if (GetLastError() != 0)             return FALSE;
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
		else{
			output = reinterpret_cast<gtOutputWindowWin32*>(GetWindowLongPtr(hWnd, -21));
		}
	}

	switch( uMsg ){
		case WM_CTLCOLORSTATIC:
			if ( ( HWND ) lParam == output->m_hWndBuffer ) {
				SetBkMode( ( HDC ) wParam, TRANSPARENT );
				SetTextColor( ( HDC ) wParam, RGB( 204, 204, 204 ) );
				return ( s64 ) output->m_hbrEditBackground;
			}
			break;
		case WM_VSCROLL:{
				RedrawWindow(output->m_hWndBuffer, NULL, NULL, RDW_ERASE | RDW_INVALIDATE) ;
		}break;
		case WM_COMMAND:{
			if( wParam == GT_MENU_ID_OUTPUT_EDIT_CLEAR ){
				output->clear_buffer();
			}else if( wParam == GT_MENU_ID_OUTPUT_FILE_SAVE ){
				output->save();
			}else if( wParam == GT_MENU_ID_OUTPUT_VIEW_HIDE ){
				output->hide();
			}

			if( HIWORD(wParam) == EN_VSCROLL )
				RedrawWindow(output->m_hWndBuffer, NULL, NULL, RDW_ERASE | RDW_INVALIDATE) ;
		}break;
		case WM_MOUSEMOVE:{
		}break;
		case WM_SYSCOMMAND:
			if( wParam == SC_CLOSE ){
				output->hide();
			}
			break;
		case WM_SIZING:
		case WM_SIZE:{
			RECT rc;
			GetClientRect( output->m_hWnd, &rc );
			MoveWindow( output->m_hWndBuffer, 1, 1, rc.right-1,rc.bottom-40,TRUE );
		}break;
		case WM_CREATE:{

		}break;
    }

    return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

void gtOutputWindowWin32::clear_buffer( void ){
	SetWindowText( m_hWndBuffer, L"" );
}

void gtOutputWindowWin32::save( void ){
#ifdef _MSC_VER
	IFileDialog *pfd;
    if(SUCCEEDED(CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd)))){
		pfd->SetOptions( FOS_OVERWRITEPROMPT | FOS_NOREADONLYRETURN | FOS_PATHMUSTEXIST | FOS_NOCHANGEDIR );
		COMDLG_FILTERSPEC cdfs;
		cdfs.pszName=L"txt";
		cdfs.pszSpec=L"*.txt";
		pfd->SetFileTypes(1,&cdfs);
		pfd->SetDefaultExtension( L"txt" );
        if(SUCCEEDED(pfd->Show(NULL))){
            IShellItem *psi;
            if(SUCCEEDED(pfd->GetResult(&psi))){
				PWSTR pszName;
                psi->GetDisplayName(SIGDN_FILESYSPATH, &pszName);
                psi->Release();

				int len = GetWindowTextLength( m_hWndBuffer );
				std::wstring wstr;
				wstr.append( static_cast<u32>(len),0u);
				auto * ptr = &wstr[0];
				GetWindowText( m_hWndBuffer, ptr, len );

				std::wofstream out(pszName);
				out.write( wstr.data(), len-1 );
            }
        }
        pfd->Release();
    }
#endif
}

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
