	/**	GOST
	\file gtError.h
	\brief Some macros for error handling
*/
#pragma once
#ifndef __GT_ERROR_H__
#define __GT_ERROR_H__ //< include guard


namespace gost{

#ifdef GT_DEBUG


	//	Утверждение с указанием двух величин
	//	\param expr: expression(логическое выражение)
	//	\param str: по какой причине сработало
	//	\param exprstr: выражение в виде строки
	//	\param v1: первая величина
	//	\param v2: вторая величина
#	define GT_ASSERT(expr,str,exprstr,v1,v2) if(!(expr)){\
				gtLogWriter::printError(u"Assertion failed: %s", u##str );\
				gtLogWriter::printError(u"Expected: %s [%u][%u]", u##exprstr, v1, v2 );\
				gtLogWriter::printError(u"Source: %s, line %i", GT_FILE, GT_LINE );\
				gtStackTrace::dumpStackTrace();\
								GT_BREAKPOINT(0) };

	//	Утверждение
	//	\param expr: expression(логическое выражение)
	//	\param str: по какой причине сработало
	//	\param exprstr: выражение в виде строки
#	define GT_ASSERT1(expr,str,exprstr) if(!(expr)){\
				gtLogWriter::printError(u"Assertion failed: %s", u##str );\
				gtLogWriter::printError(u"Expected: %s", u##exprstr );\
				gtLogWriter::printError(u"Source: %s, line %i", GT_FILE, GT_LINE );\
				gtStackTrace::dumpStackTrace();\
								GT_BREAKPOINT(0) };

	//	Утверждение
	//	\param expr: expression(логическое выражение)
	//	\param exprstr: выражение в виде строки
#	define GT_ASSERT2(expr,exprstr) if(!(expr)){\
				gtLogWriter::printError(u"Assertion failed" );\
				gtLogWriter::printError(u"Expected: %s", u##exprstr );\
				gtLogWriter::printError(u"Source: %s, line %i", GT_FILE, GT_LINE );\
				gtStackTrace::dumpStackTrace();\
								GT_BREAKPOINT(0) };

	//	Утверждение
	//	\param expr: expression(логическое выражение)
#	define GT_ASSERT3(expr) if(!(expr)){\
				gtLogWriter::printError(u"Assertion failed" );\
				gtLogWriter::printError(u"Source: %s, line %i", GT_FILE, GT_LINE );\
				gtStackTrace::dumpStackTrace();\
								GT_BREAKPOINT(0) };


#if (__cplusplus > 199711L || _MSC_VER > 1800 )

#define GT_ERROR_NEW_STYLE
#endif

//	Here errors for old style static assert
namespace gtError{

		//	используется для определения ошибок на этапе компиляции
	template< s32 x > struct StaticAssert{};

		//	ошибки
	template< bool b > struct GT_VECTOR_BAD_SIZE;
	template<> struct GT_VECTOR_BAD_SIZE<true>{ };

}

#if defined(GT_ERROR_NEW_STYLE)
#	define GT_STATIC_ASSERT(expr,msg)\
	static_assert(expr, QUOTE(msg) )
#else

	//	Определение ошибки на этапе компиляции
	//	\param expr: логическое выражение
	//	\param msg: сообщение
	//	\warning Для C++11 и выше допустимо использовать в качестве сообщения любую строку. Для старой версии C++ есть ошибки в виде имён структур. Лучше создать структуру с именем в виде сообщения.
#	define GT_STATIC_ASSERT(expr,msg)\
	typedef gtError::StaticAssert< sizeof( msg< static_cast<bool>(expr) > ) >

#endif

#else
#	define GT_ASSERT(expr,str,exprstr,v1,v2)
#	define GT_ASSERT1(expr,str,exprstr)
#	define GT_ASSERT2(expr,exprstr)
#	define GT_ASSERT3(expr)
#	define GT_STATIC_ASSERT(expr,msg)
#	define GT_WARNING1(expr,msg1,exprstr)
#	define GT_WARNING2(expr,msg1,code)
#	define GT_WARNING1R(expr,msg1,exprstr)
#	define GT_WARNING2R(expr,msg1,code)
#endif

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
