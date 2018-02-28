/*!	GOST
	\file gtOutputWindow.h
	\brief output window. Окно для вывода текстовой информации
*/
#pragma once
#ifndef __GT_OUTPUT_WINDOW_H__
#define __GT_OUTPUT_WINDOW_H__ ///< include guard

namespace gost{

		///	Окно для вывода текстовой информации
	class gtOutputWindow : public gtRefObject {
	public:

			///	Прежде чем использовать нужно вызвать Init
		virtual	void	init( void ) = 0;

			///	При завершении работы нужно вызвать этот метод
		virtual void	shutdown( void ) = 0;

			///	Инициализировано ли
			/// \return Вернёт \b true если инициализировано
		virtual bool	isInit( void ) = 0;

			///	Если окно невидимо, покажет его
		virtual void	show( void ) = 0;

			///	Сделать окно невидимым (спрячет, или сделает прозрачным, зависит от реализации)
		virtual void	hide( void ) = 0;

			///	Напечатать текст
			/// \param text: text
		virtual void	print( const gtString& text ) = 0;

			///	Установить заголовок
			/// \param title: title
		virtual void	setWindowText( const gtString& title ) = 0;
		
			///
		virtual bool	isShow( void ) = 0;
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