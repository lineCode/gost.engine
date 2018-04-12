﻿
/*!	GOST
	\file gtWindow.h
	\brief Всё что связано с системным окном
*/
#pragma once
#ifndef __GT_WINDOW_H__
#define __GT_WINDOW_H__ //< include guard

namespace gost{
	
	
	
		//	System window
	class gtWindow : public gtRefObject{
	public:

			//	Set title. Установить заголовок окна
			// \param title: заголовок
		virtual void	setWindowTitle( const gtString& title ) = 0;

			//	Вернуть handle (HWND) окна
			// \return вернёт handle окна
		virtual void*	getHandle( void ) = 0;

			//	Получить координаты левого верхнего и правого нижнего углов
			// \return Вернёт координаты левого верхнего и правого нижнего углов
		virtual const v4i&		getRect( void ) = 0;

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