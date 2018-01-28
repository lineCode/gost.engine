//	GOST

namespace gost{

	GT_FORCE_INLINE void gtStackTrace::initialize( void ){
		if( !m_is_initialized ){
			if( !SymInitialize( GetCurrentProcess(), NULL, TRUE ) ){
				m_log->print(gtLoger::msgType::warning, u"%s", u"can not initialize stack tracer" );
				return;
			}
			m_is_initialized = true;
		}
	}

	GT_FORCE_INLINE void gtStackTrace::shutdown( void ){
		if( m_is_initialized ){
			if( !SymCleanup( GetCurrentProcess() ) ){
				m_log->print(gtLoger::msgType::warning, u"%s", u"Can not free memory. Error code [%u]", GetLastError() );
			}
		}
	}

	GT_FORCE_INLINE void gtStackTrace::printStackTrace( u32 skip_begin, u32 skip_end ){

		this->initialize();

		HANDLE proc = GetCurrentProcess();

		DWORD symOpt = SymGetOptions();
		symOpt |= SYMOPT_LOAD_LINES | SYMOPT_DEBUG | SYMOPT_DEFERRED_LOADS;
		SymSetOptions( symOpt );

		unsigned long * stack[ gtMaxStackFrames ];

		u32 frames = CaptureStackBackTrace( 0u, gtMaxStackFrames, (PVOID*)stack, NULL );

		SYMBOL_INFO_PACKAGE sip;
		sip.si.SizeOfStruct = sizeof(sip.si);
		sip.si.MaxNameLen = sizeof(sip.name);

		m_log->print(gtLoger::msgType::info, u"%s", u"Dump stack trace:" );

		if( skip_begin >= frames-skip_end ) skip_begin = 0u;
		if( skip_end > frames ) skip_end = 0u;

		for( u32 i(skip_begin); i < frames-skip_end; ++i ){
				
			DWORD64 symbol = (DWORD64)stack[i];

			DWORD64 displacement = 0;
			BOOL ok_symbol = SymFromAddr( proc, symbol, &displacement, &sip.si);
			if( !ok_symbol ){
				m_log->print(gtLoger::msgType::warning, u"%s[%i]", u"Can not get symbol from address. Error code ", s32(GetLastError()) );
				continue;
			}

			IMAGEHLP_LINE64 line;
			memset( &line, 0, sizeof(line) );
				
			line.SizeOfStruct = sizeof(line);

			DWORD displacement2 = 0;
			ok_symbol = SymGetLineFromAddr64( proc, symbol, &displacement2, &line);
			if( !ok_symbol ){
				m_log->print(gtLoger::msgType::warning, u"%s[%i]", u"Can not locate source line. Error code ", s32(GetLastError()) );
				continue;
			}

			s8 trace[2048];
			GT_SNPRINTF(trace, 2048, "%s(%i):'%s' - %#010x", line.FileName, line.LineNumber, sip.si.Name, sip.si.Address );
			
			//	char â chart16_t
			gtString message;
			s8 * p = &trace[0];
			while( *p ){
				message += static_cast<char16_t>(*p);
				++p;
			}
			
			m_log->print(gtLoger::msgType::info, u"%s", message.data() );
		}
				
	}

}
/*
Copyright (c) 2017 532235

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