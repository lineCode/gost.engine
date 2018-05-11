#pragma once
#ifndef __GT_STACK_TRACE_H__
#define __GT_STACK_TRACE_H__

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
	
	constexpr u32 gtMaxStackFrames = 1024u;

	class gtStackTrace{
		
		gtPtr<gtLog> m_log;

		//gtMutex * m_mutex;

		gtStackTrace(){}

		void initialize();	bool m_is_initialized;
		void shutdown();

	public:

		gtStackTrace( gtMainSystem* s ):m_is_initialized( false ){
			m_log = gtPtrNew<gtLog>( s->getLog() );
			m_log->setOutputWindow( s->getOutputWindow() );
			m_log->addRef();
		}

		virtual ~gtStackTrace(){
			m_log->release();
		}

		void printStackTrace( u32 skip_begin = 1u, u32 skip_end = 7u );

		GT_API static	void dumpStackTrace();

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