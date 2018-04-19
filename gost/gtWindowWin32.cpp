//	GOST

#include "common.h"

#if defined(GT_PLATFORM_WIN32)

gtWindowWin32::gtWindowWin32( gtWindowInfo* wi )
	:	m_hWnd( 0u ), m_isInit( false )
{
	m_params = *wi;

}

gtWindowWin32::~gtWindowWin32( void ){
	if( m_isInit ){
		if( m_hWnd ) DestroyWindow( m_hWnd );
		UnregisterClass( (wchar_t*)m_className.data(), GetModuleHandle( 0 ) );
	}
}

bool	gtWindowWin32::init( u32 i ){

	GT_ASSERT1( !m_isInit, "Window is initialized", "!m_isInit" );
	if( m_isInit ) return false;

	DWORD style = WS_BORDER | WS_CAPTION | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_SYSMENU | WS_MINIMIZEBOX; // DWORD - это unsigned long

	if( m_params.m_style & gtWindowInfo::style::popup ){
		style = WS_POPUP;
	}else{
		if( m_params.m_style & gtWindowInfo::style::maximize )
			style |= WS_MAXIMIZEBOX;
		if( m_params.m_style & gtWindowInfo::style::resize )
			style |= WS_SIZEBOX;
	}

	s32 windowLeft = m_params.m_rect.x;
	s32 windowTop = m_params.m_rect.y;
	s32 realWidth = m_params.m_rect.z;
	s32 realHeight = m_params.m_rect.w;

	if( m_params.m_style & gtWindowInfo::style::center ){

		RECT clientSize;
		clientSize.top = 0;
		clientSize.left = 0;
		clientSize.right = m_params.m_rect.z - m_params.m_rect.x;
		clientSize.bottom = m_params.m_rect.w - m_params.m_rect.y;

		AdjustWindowRect(&clientSize, style, FALSE);

		realWidth = clientSize.right - clientSize.left;
		realHeight = clientSize.bottom - clientSize.top;

		windowLeft = (GetSystemMetrics(SM_CXSCREEN) - realWidth) / 2;
		windowTop = (GetSystemMetrics(SM_CYSCREEN) - realHeight) / 2;

		if ( windowLeft < 0 )
			windowLeft = 0;
		if ( windowTop < 0 )
			windowTop = 0;

		if( m_params.m_style & gtWindowInfo::style::popup ){
			windowLeft = 0;
			windowTop = 0;
		}
	}

	m_className = u"GTWINDOW_";
	m_className += static_cast<char16_t>(i);

	WNDCLASSEX wc;
	ZeroMemory( &wc, sizeof( wc ) ); // memset Winows style
	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wc.lpfnWndProc		= WndProc;
	wc.hInstance		= GetModuleHandle( 0 );
	wc.hIcon			= nullptr;
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground	= (HBRUSH)(COLOR_MENU+1);
	wc.lpszMenuName		= nullptr;
	wc.lpszClassName	= (wchar_t*)m_className.data();
	wc.hIconSm			= nullptr;

	if( !RegisterClassEx(&wc) ){
		gtLogWriter::printError( u"Can not register window class. Error code[%u]", (u32)GetLastError() );
		return false;
	}

	m_hWnd = CreateWindow( (wchar_t*)m_className.data(),
		(wchar_t*)m_params.m_title.data(),
		style,
		windowLeft,
		windowTop,
		realWidth,
		realHeight,
		nullptr,
		nullptr,
		wc.hInstance,
		this );

	if( !m_hWnd ){
		gtLogWriter::printError( u"Can not create window. Error code[%u]", (u32)GetLastError() );
		return false;
	}

	ShowWindow( m_hWnd, SW_SHOWNORMAL  );
	SetForegroundWindow( m_hWnd );
	SetFocus( m_hWnd );
	UpdateWindow( m_hWnd );


	m_isInit = true;
	return true;
}

void	gtWindowWin32::setWindowTitle( const gtString& title ){
	if( !m_hWnd){
		gtLogWriter::printWarning( u"Can not set window title" );
		gtLogWriter::printWarning( u"HWND == 0" );
		gtStackTrace::dumpStackTrace();
		return;
	}


	if( SetWindowText( m_hWnd, (wchar_t*)title.data() ) == FALSE ){
		gtLogWriter::printWarning( u"Can not set window title. Code [%u]\n", (u32)GetLastError() );
		gtStackTrace::dumpStackTrace();
		return;
	}
}

void*	gtWindowWin32::getHandle( void ){
	return (void*)m_hWnd;
}

// Взято у IrrLicht
// Get the codepage from the locale language id
// Based on the table from http://www.science.co.il/Language/Locale-Codes.asp?s=decimal
static unsigned int LocaleIdToCodepage(unsigned int lcid)
{
	switch ( lcid )
	{
		case 1098:  // Telugu
		case 1095:  // Gujarati
		case 1094:  // Punjabi
		case 1103:  // Sanskrit
		case 1111:  // Konkani
		case 1114:  // Syriac
		case 1099:  // Kannada
		case 1102:  // Marathi
		case 1125:  // Divehi
		case 1067:  // Armenian
		case 1081:  // Hindi
		case 1079:  // Georgian
		case 1097:  // Tamil
			return 0;
		case 1054:  // Thai
			return 874;
		case 1041:  // Japanese
			return 932;
		case 2052:  // Chinese (PRC)
		case 4100:  // Chinese (Singapore)
			return 936;
		case 1042:  // Korean
			return 949;
		case 5124:  // Chinese (Macau S.A.R.)
		case 3076:  // Chinese (Hong Kong S.A.R.)
		case 1028:  // Chinese (Taiwan)
			return 950;
		case 1048:  // Romanian
		case 1060:  // Slovenian
		case 1038:  // Hungarian
		case 1051:  // Slovak
		case 1045:  // Polish
		case 1052:  // Albanian
		case 2074:  // Serbian (Latin)
		case 1050:  // Croatian
		case 1029:  // Czech
			return 1250;
		case 1104:  // Mongolian (Cyrillic)
		case 1071:  // FYRO Macedonian
		case 2115:  // Uzbek (Cyrillic)
		case 1058:  // Ukrainian
		case 2092:  // Azeri (Cyrillic)
		case 1092:  // Tatar
		case 1087:  // Kazakh
		case 1059:  // Belarusian
		case 1088:  // Kyrgyz (Cyrillic)
		case 1026:  // Bulgarian
		case 3098:  // Serbian (Cyrillic)
		case 1049:  // Russian
			return 1251;
		case 8201:  // English (Jamaica)
		case 3084:  // French (Canada)
		case 1036:  // French (France)
		case 5132:  // French (Luxembourg)
		case 5129:  // English (New Zealand)
		case 6153:  // English (Ireland)
		case 1043:  // Dutch (Netherlands)
		case 9225:  // English (Caribbean)
		case 4108:  // French (Switzerland)
		case 4105:  // English (Canada)
		case 1110:  // Galician
		case 10249:  // English (Belize)
		case 3079:  // German (Austria)
		case 6156:  // French (Monaco)
		case 12297:  // English (Zimbabwe)
		case 1069:  // Basque
		case 2067:  // Dutch (Belgium)
		case 2060:  // French (Belgium)
		case 1035:  // Finnish
		case 1080:  // Faroese
		case 1031:  // German (Germany)
		case 3081:  // English (Australia)
		case 1033:  // English (United States)
		case 2057:  // English (United Kingdom)
		case 1027:  // Catalan
		case 11273:  // English (Trinidad)
		case 7177:  // English (South Africa)
		case 1030:  // Danish
		case 13321:  // English (Philippines)
		case 15370:  // Spanish (Paraguay)
		case 9226:  // Spanish (Colombia)
		case 5130:  // Spanish (Costa Rica)
		case 7178:  // Spanish (Dominican Republic)
		case 12298:  // Spanish (Ecuador)
		case 17418:  // Spanish (El Salvador)
		case 4106:  // Spanish (Guatemala)
		case 18442:  // Spanish (Honduras)
		case 3082:  // Spanish (International Sort)
		case 13322:  // Spanish (Chile)
		case 19466:  // Spanish (Nicaragua)
		case 2058:  // Spanish (Mexico)
		case 10250:  // Spanish (Peru)
		case 20490:  // Spanish (Puerto Rico)
		case 1034:  // Spanish (Traditional Sort)
		case 14346:  // Spanish (Uruguay)
		case 8202:  // Spanish (Venezuela)
		case 1089:  // Swahili
		case 1053:  // Swedish
		case 2077:  // Swedish (Finland)
		case 5127:  // German (Liechtenstein)
		case 1078:  // Afrikaans
		case 6154:  // Spanish (Panama)
		case 4103:  // German (Luxembourg)
		case 16394:  // Spanish (Bolivia)
		case 2055:  // German (Switzerland)
		case 1039:  // Icelandic
		case 1057:  // Indonesian
		case 1040:  // Italian (Italy)
		case 2064:  // Italian (Switzerland)
		case 2068:  // Norwegian (Nynorsk)
		case 11274:  // Spanish (Argentina)
		case 1046:  // Portuguese (Brazil)
		case 1044:  // Norwegian (Bokmal)
		case 1086:  // Malay (Malaysia)
		case 2110:  // Malay (Brunei Darussalam)
		case 2070:  // Portuguese (Portugal)
			return 1252;
		case 1032:  // Greek
			return 1253;
		case 1091:  // Uzbek (Latin)
		case 1068:  // Azeri (Latin)
		case 1055:  // Turkish
			return 1254;
		case 1037:  // Hebrew
			return 1255;
		case 5121:  // Arabic (Algeria)
		case 15361:  // Arabic (Bahrain)
		case 9217:  // Arabic (Yemen)
		case 3073:  // Arabic (Egypt)
		case 2049:  // Arabic (Iraq)
		case 11265:  // Arabic (Jordan)
		case 13313:  // Arabic (Kuwait)
		case 12289:  // Arabic (Lebanon)
		case 4097:  // Arabic (Libya)
		case 6145:  // Arabic (Morocco)
		case 8193:  // Arabic (Oman)
		case 16385:  // Arabic (Qatar)
		case 1025:  // Arabic (Saudi Arabia)
		case 10241:  // Arabic (Syria)
		case 14337:  // Arabic (U.A.E.)
		case 1065:  // Farsi
		case 1056:  // Urdu
		case 7169:  // Arabic (Tunisia)
			return 1256;
		case 1061:  // Estonian
		case 1062:  // Latvian
		case 1063:  // Lithuanian
			return 1257;
		case 1066:  // Vietnamese
			return 1258;
	}
	return 65001;   // utf-8
}

LRESULT CALLBACK gtWindowWin32::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
//	PAINTSTRUCT ps;
//	HDC hdc;
	gtWindowWin32* pD = nullptr;

	s32 wmId    = LOWORD(wParam);
//	s32 wmEvent = HIWORD(wParam);

	static HKL KEYBOARD_INPUT_HKL = 0;
	static u32 KEYBOARD_INPUT_CODEPAGE = 1252;

	gtEvent ev;
	ev.type = gtEventType::System;

	if( !pD ){
		if (message == WM_NCCREATE)   {
			pD = static_cast<gtWindowWin32*>(reinterpret_cast<CREATESTRUCT*>(lParam)->lpCreateParams);
			SetLastError(0);
			if (!SetWindowLongPtr(hWnd, -21, reinterpret_cast<LONG_PTR>(pD)))
				if (GetLastError() != 0)             return FALSE;
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		else{
			pD = reinterpret_cast<gtWindowWin32*>(GetWindowLongPtr(hWnd, -21));
		}
	}

	switch( message ){
	//case WM_COMMAND:
	//	// Parse the menu selections:
	//	switch (wmId)
	//	{
	//
	//	default:
	//		return DefWindowProc(hWnd, message, wParam, lParam);
	//	}
	//	break;

	case WM_MOVE:
//		ev.value1 = GT_EVENT_WINDOW_MOVE;
	break;

	case WM_PAINT:
//		ev.value1 = GT_EVENT_WINDOW_PAINT;
		break;

	case WM_SIZE:{
		switch( wmId ){
		case SIZE_MAXIMIZED:
//			ev.value1 = GT_EVENT_WINDOW_MAXIMIZE;
			break;
		case SIZE_MINIMIZED:
//			ev.value1 = GT_EVENT_WINDOW_MINIMIZE;
			break;
		case SIZE_RESTORED:
//			ev.value1 = GT_EVENT_WINDOW_RESTORE;
			break;
		}
	}break;

	case WM_SIZING:
//			ev.value1 = GT_EVENT_WINDOW_SIZING;
	break;

	case WM_QUIT:
	case WM_CLOSE:
	case WM_DESTROY:
		gtMainSystem::getInstance()->getSceneSystem( nullptr )->clearScene();
		PostQuitMessage( 0 );
	break;

	case WM_LBUTTONDBLCLK:
	case WM_RBUTTONDBLCLK:
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEMOVE:
	case WM_MOUSEWHEEL:{
		ev.type = gtEventType::Mouse;

		ev.mouseEvent.x = LOWORD(lParam);
		ev.mouseEvent.y = HIWORD(lParam);

		if( wParam & MK_LBUTTON )		ev.mouseEvent.state.bits.b0 = true;
		if( wParam & MK_RBUTTON )		ev.mouseEvent.state.bits.b2 = true;
		if( wParam & MK_MBUTTON )		ev.mouseEvent.state.bits.b4 = true;
		if( wParam & MK_XBUTTON1 ){
			ev.mouseEvent.state_extra.bits.b0 = true;
			switch( message ){
				case WM_XBUTTONUP: ev.mouseEvent.state_extra.bits.b1 = true; break;
				case WM_XBUTTONDBLCLK: ev.mouseEvent.state_extra.bits.b4 = true; break;
			}
		}
		if( wParam & MK_XBUTTON2 ){
			ev.mouseEvent.state_extra.bits.b2 = true;
			switch( message ){
				case WM_XBUTTONUP: ev.mouseEvent.state_extra.bits.b3 = true; break;
				case WM_XBUTTONDBLCLK: ev.mouseEvent.state_extra.bits.b5 = true; break;
			}
		}

		switch( message ){
			case WM_LBUTTONUP:	ev.mouseEvent.state.bits.b1 = true; break;
			case WM_RBUTTONUP:	ev.mouseEvent.state.bits.b3 = true; break;
			case WM_MBUTTONUP:	ev.mouseEvent.state.bits.b5 = true; break;
			case WM_LBUTTONDBLCLK: ev.mouseEvent.state.bits.b6 = true; break;
			case WM_RBUTTONDBLCLK: ev.mouseEvent.state.bits.b7 = true; break;
		}

		ev.mouseEvent.wheel = static_cast<u16>( s8( (f32)GET_WHEEL_DELTA_WPARAM(wParam) / (f32)WHEEL_DELTA ) );

		gtMainSystem::getInstance()->addEvent( ev );

		return 0;
	}break;

	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_KEYDOWN:
	case WM_KEYUP:{
		ev.type = gtEventType::Keyboard;
		ev.keyboardEvent.key = static_cast<gtKey>( wParam );

		const UINT MY_MAPVK_VSC_TO_VK_EX = 3;
		if( ev.keyboardEvent.key == gtKey::K_SHIFT ){ // shift -> lshift rshift
			ev.keyboardEvent.key = static_cast<gtKey>( MapVirtualKey( ( static_cast<UINT>( lParam >> 16) & 255u ), MY_MAPVK_VSC_TO_VK_EX ) );
			ev.keyboardEvent.state.bits.b2 = true;
		}if( ev.keyboardEvent.key == gtKey::K_CTRL ){ // ctrl -> lctrl rctrl
			ev.keyboardEvent.key = static_cast<gtKey>( MapVirtualKey( ( static_cast<UINT>( lParam >> 16) & 255 ), MY_MAPVK_VSC_TO_VK_EX ) );
			ev.keyboardEvent.state.bits.b3 = true;
			if( lParam & 0x1000000 )
					ev.keyboardEvent.key = static_cast<gtKey>( 163 );
		}

		ev.keyboardEvent.state.bits.b0 = ( message == WM_KEYDOWN );
		ev.keyboardEvent.state.bits.b1 = ( message == WM_KEYUP );

		if( ev.keyboardEvent.key == gtKey::K_ALT ){ // alt -> lalt ralt
			ev.keyboardEvent.key = static_cast<gtKey>( MapVirtualKey( ( static_cast<UINT>( lParam >> 16) & 255 ), MY_MAPVK_VSC_TO_VK_EX ) );
			if( lParam & 0x1000000 )
					ev.keyboardEvent.key = static_cast<gtKey>(165);
			ev.keyboardEvent.state.bits.b4 = true;
		}

		u8 keys[ 256u ];
		GetKeyboardState( keys );
		WORD chars[ 2u ];

		if( ToAsciiEx( (UINT)wParam, HIWORD(lParam), keys, chars, 0, KEYBOARD_INPUT_HKL ) == 1 ){
			WORD uChar;
			MultiByteToWideChar( KEYBOARD_INPUT_CODEPAGE, MB_PRECOMPOSED, (LPCSTR)chars,
				sizeof(chars), (WCHAR*)&uChar, 1 );
			ev.keyboardEvent.character = uChar;
		}else ev.keyboardEvent.character = 0;

		gtMainSystem::getInstance()->addEvent( ev );

		if( message == WM_SYSKEYDOWN || message == WM_SYSKEYUP )
			return DefWindowProc( hWnd, message, wParam, lParam );
		else
			return 0;
	}break;

	case WM_INPUTLANGCHANGE:
		KEYBOARD_INPUT_HKL = GetKeyboardLayout( 0 );
		KEYBOARD_INPUT_CODEPAGE = LocaleIdToCodepage( LOWORD(KEYBOARD_INPUT_HKL) );
	return 0;

		//	запрет на нажатие некоторых клавиш
	case WM_SYSCOMMAND:
		if( ( wParam & 0xFFF0 ) == SC_SCREENSAVE ||
			( wParam & 0xFFF0 ) == SC_MONITORPOWER ||
			( wParam & 0xFFF0 ) == SC_KEYMENU
		)
		return 0;
	}


	//gtMainSystem::getInstance()->addEvent( ev );

	return DefWindowProc( hWnd, message, wParam, lParam );
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
