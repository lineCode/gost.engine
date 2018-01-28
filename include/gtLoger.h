/**	GOST
	\file gtLoger.h
	\brief Helper class for logging
*/

#pragma once
#ifndef __GT_LOGER_H__
#define __GT_LOGER_H__ ///< include guard

namespace gost{

	class gtOutputWindow;

		///	выводит сообщения в окно вывода
	class gtLoger : public gtRefObject{
	public:

			///	можно будет отключить показ общей информации
			///	предупреждений или ошибок
		enum class msgType{
			error,		///< только ошибки
			warning,	///< только ошибки и предупреждения
			info		///< все сообщения
		};

			///	напечатает форматированную строку
			///	\param type: какого типа сообщение
			///	\param str: форматированный текст
			///	\param ...: аргументы \n
			/// Обозначения для аргументов: \anchor ox_anchor_gtLoger_print
			///	\code {.cpp}
			///	\%f - float			
			///	\%i - int			
			///	\%u - unsigned		
			///	\%s - char16_t*		
			///	\%c - char16_t		
			///	\endcode
		virtual void print( msgType type, char16_t* str, ... ) = 0;

			///	установка окна, в которое будет выводится текст
		virtual void setOutputWindow( gtOutputWindow* ) = 0;

			///	\param type: тип сообщения
			///	info - будут все сообщения \n
			///	warning - warning и error \n
			///	error - только error \n
		virtual void setInfoType( msgType type = msgType::info ) = 0;


	};

		///	Для более простого вывода сообщений
	class gtLogWriter{
	public:
			///	Вывести сообщение об ошибке
			///	\param str: форматированный текст
			///	\param ...: аргументы
			///	\n \ref ox_anchor_gtLoger_print "Обозначения для аргументов"
		GT_API static	void printError( char16_t* str, ... );

			///	Вывести предупреждение 
			///	\param str: форматированный текст
			///	\param ...: аргументы
			///	\n \ref ox_anchor_gtLoger_print "Обозначения для аргументов"
		GT_API static	void printWarning( char16_t* str, ... );

			///	Вывести информационное сообщение 
			///	\param str: форматированный текст
			///	\param ...: аргументы
			///	\n \ref ox_anchor_gtLoger_print "Обозначения для аргументов"
		GT_API static	void printInfo( char16_t* str, ... );
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