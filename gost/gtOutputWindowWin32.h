//	GOST

#pragma once
#ifndef __GT_OUTPUT_WINDOW_WIN32_H__
#define __GT_OUTPUT_WINDOW_WIN32_H__

/*
	Реализация для Windows
*/

#if defined(GT_PLATFORM_WINDOWS)

namespace gost{

	//	Реализация для Windows
	class gtOutputWindowWin32 GT_FINAL : public gtOutputWindow {

		//	true если был вызван init()
		bool m_isInit;

		bool m_isShown;

		//	Window handle
		HWND m_hWnd;
		WNDCLASS m_wc;

		RECT m_windowRect;

		//	Оконная процедура
		static LRESULT CALLBACK
			OutWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

			//	очистить окно
		void clear_buffer( void );

			//	сохранить всё что есть в окне в файл
		void save( void );

	public:

		//	Конструктор
		gtOutputWindowWin32( void );

		//	Деструктор
		virtual ~gtOutputWindowWin32( void );

		//	Инициализация
		void	init( void ) GT_FINAL;	//	по сути если класс объявлен как final то нет смысла писать его для методов. ну да ладно, буду говнокодить

		//	Завершение работы
		void	shutdown( void ) GT_FINAL;

		//	Возвратит true если инициализирован
		bool	isInit( void ) GT_FINAL;

		//	Если окно невидимо, покажет его
		void	show( void ) GT_FINAL;

		//	Сделает окно невидимым (спрячет, или сделает прозрачным, зависит от реализации)
		void	hide( void ) GT_FINAL;

		//	Напечатает текст
		void	print( const gtString& ) GT_FINAL;

		//	Установит заголовок
		void	setWindowText( const gtString& ) GT_FINAL;

		bool	isShow( void );

		HWND m_hWndBuffer;
		HBRUSH m_hbrEditBackground;
	};

}
#endif
#endif

/*
Copyright (c) 2017

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
