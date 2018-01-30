/*!	GOST
	\file gtStackTrace.h
	\brief stack tracer
*/
#pragma once
#ifndef __GT_STACK_TRACE_H__
#define __GT_STACK_TRACE_H__ ///< include guard

#if defined( GT_USE_STACK_TRACE )

#if defined( GT_PLATFORM_WIN32 )
//#include <Windows.h>
#include <dbghelp.h>
#pragma comment(lib,"Dbghelp.lib")
#else
#error Класс gtStackTrace поддерживается только для Win32
#endif

#endif

namespace gost{
	
		/// размер стека
	constexpr u32 gtMaxStackFrames = 1024u;

		///	Выводит цепочку вызовов функций от текущей до самого начала
		///	По логике должен работать только в Debug версии 
	class gtStackTrace{
		
			///	для вывода текста
		gtPtr<gtLoger> m_log;

			///	нельзя создать объект с таким конструктором
		gtStackTrace( void ){}

			///	инициализация при первом вызове printStackTrace
		void initialize( void );	bool m_is_initialized;
			///	производит зависимое от ОС операции по освобождению ранее взятой памяти
		void shutdown( void );

	public:
			///	конструирует объект и инициализирует логер
		gtStackTrace( gtMainSystem* s ) : m_is_initialized( false ){
			m_log = gtPtrNew<gtLoger>( s->getLoger() );
			m_log->setOutputWindow( s->getOutputWindow() );
			m_log->addRef();
		}
			///	dtor
		virtual ~gtStackTrace( void ){
			m_log->release();
		}

			///	skip_begin	- пропуск с начала\n
			///	skip_end	- пропуск с конца\n
			///	если skip_begin = 0 то будет показан gtStackTrace::printStackTrace()\n
			///	что очевидно, по этому по умолчанию стоит 1\n
			///	skip_end = 7 значит не напечатает последние 7
		void printStackTrace( u32 skip_begin = 1u, u32 skip_end = 7u );

			///	движок имеет свой объект gtStackTrace\n
			///	данной функцией можно вызывать printStackTrace()\n
			///	всего лишь 1м предложением gtStackTrace::dumpStackTrace()\n
			///	создан специально чтобы не плодить объекты gtStackTrace в разных модулях (.exe, .dll)
		GT_API static	void dumpStackTrace( void );

	};

}

#if defined( GT_USE_STACK_TRACE )

#if defined( GT_PLATFORM_WIN32 )
#include <gtStackTraceWin32.inl>
#else
#error Класс gtStackTrace поддерживается только для Win32
#endif

#else
namespace gost{
GT_FORCE_INLINE void gtStackTrace::printStackTrace( u32 /*skip_begin*/, u32 /*skip_end*/ ){}
}
#endif


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