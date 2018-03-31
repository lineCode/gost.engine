//	GOST

namespace gost{

	
	GT_FORCE_INLINE void gtStackTrace::initialize( void ){
		if( !m_is_initialized ){
			//m_mutex = gtMainSystem::getInstance()->createMutex();
			//m_mutex->lock();
			DWORD symOpt = SymGetOptions();
			symOpt |= SYMOPT_LOAD_LINES | SYMOPT_DEBUG | SYMOPT_DEFERRED_LOADS;
			SymSetOptions( symOpt );

			UINT prevErrMode = GetErrorMode();
			SetErrorMode( SEM_FAILCRITICALERRORS );
			
			if( !SymInitialize( GetCurrentProcess(), NULL, TRUE ) ){
				m_log->print(gtLoger::msgType::warning, u"%s", u"can not initialize stack tracer" );
				return;
			}

			SetErrorMode( prevErrMode );
			m_is_initialized = true;

			//m_mutex->unlock();
		}
	}

	GT_FORCE_INLINE void gtStackTrace::shutdown( void ){
		if( m_is_initialized ){
			if( !SymCleanup( GetCurrentProcess() ) ){
				m_log->print(gtLoger::msgType::warning, u"%s", u"Can not free memory. Error code [%u]", GetLastError() );
			}
			//m_mutex->release();
		}
	}

	GT_FORCE_INLINE void gtStackTrace::printStackTrace( u32 /*skip_begin*/, u32 /*skip_end*/ ){
		DWORD symOpt = SymGetOptions();
		symOpt |= SYMOPT_LOAD_LINES | SYMOPT_DEBUG | SYMOPT_DEFERRED_LOADS;
		SymSetOptions( symOpt );

		UINT prevErrMode = GetErrorMode();
		SetErrorMode( SEM_FAILCRITICALERRORS );
			
		if( !SymInitialize( GetCurrentProcess(), NULL, TRUE ) ){
			gtLogWriter::printWarning( u"can not initialize stack tracer" );
			return;
		}

		SetErrorMode( prevErrMode );



		HANDLE proc = GetCurrentProcess();

		unsigned long * stack[ gtMaxStackFrames ];

		unsigned int frames = CaptureStackBackTrace( 0u, gtMaxStackFrames, (PVOID*)stack, NULL );

		SYMBOL_INFO_PACKAGE sip;
		sip.si.SizeOfStruct = sizeof(sip.si);
		sip.si.MaxNameLen = sizeof(sip.name);

		gtLogWriter::printInfo( u"Dump stack trace:" );

		for( unsigned int i(0u); i < frames; ++i ){
				
			DWORD64 symbol = (DWORD64)stack[i];

			DWORD64 displacement = 0;
			BOOL ok_symbol = SymFromAddr( 
				proc,
				symbol, 
				&displacement, 
				&sip.si);

			if( !ok_symbol ){
				gtLogWriter::printError( u"Can not get symbol from address. Error code ", int(GetLastError()) );
				continue;
			}

			IMAGEHLP_LINE64 line;
			memset( &line, 0, sizeof(line) );
				
			line.SizeOfStruct = sizeof(line);

			DWORD displacement2 = 0;
			ok_symbol = SymGetLineFromAddr64( proc, symbol, &displacement2, &line);
			if( !ok_symbol ){
				gtLogWriter::printError( u"Can not locate source line. Error code ", int(GetLastError()) );
				continue;
			}

			char trace[2048];
			sprintf_s( trace, 2048, "%s(%i):'%s' - %llu", line.FileName, line.LineNumber, sip.si.Name, sip.si.Address );
			
			//	char â chart16_t
			std::string message;
			char * p = &trace[0];
			while( *p ){
				message += static_cast<char16_t>(*p);
				++p;
			}
			
			gtLogWriter::printInfo( u"%s", message.data() );
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