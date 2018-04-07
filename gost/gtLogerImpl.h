//	GOST

#pragma once
#ifndef __GT_LOGER_IMPL_H__
#define __GT_LOGER_IMPL_H__

namespace gost{

	class gtLogerImpl GT_FINAL : public gtLoger{
			 
			///	окно вывода
		gtOutputWindow*	m_out;

			///	текущий уровень вывода информации
		msgType m_msgType;

			///	расшифровывает сообщение
		void	deformat( const char16_t* fmt, gt_va_list& args, gtString& );
			

	public:
			///	конструктор
		gtLogerImpl( void );

			///	деструктор
		virtual ~gtLogerImpl( void );

			///	напечатает форматированную строку
			///	%f - float
			///	%i - int
			///	%u - unsigned
			///	%s - char16_t*
			///	%c - char16_t
		void print( msgType, char16_t* str, ... ) GT_FINAL;
			
			///	через указатель void передаются все аргументы
			///	удобное решение. другого не придумал
		void print( msgType, char16_t* str, void * );

			///	установка окна, в которое будет выводится текст
		void setOutputWindow( gtOutputWindow* ) GT_FINAL;

			///	info - будут все сообщения
			///	warning - warning и error
			///	error - только error
		void setInfoType( msgType = msgType::info ) GT_FINAL;
	};

}

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