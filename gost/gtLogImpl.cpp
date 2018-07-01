//	GOST

#include "common.h"


gtLogImpl::gtLogImpl()
: m_msgType(msgType::Info),
m_out( nullptr ){}

gtLogImpl::~gtLogImpl(){
}

	//	%f - float
	//	%i - int
	//	%u - unsigned
	//	%s - char16_t*
	//	%c - char16_t
void gtLogImpl::print( msgType type, const char16_t* str, ... ){
	if( m_msgType >= type ){
		gtString message;
		switch( type ){
		case gost::gtLog::msgType::Error:
			message.assign(u"Error: ");
			break;
		case gost::gtLog::msgType::Warning:
			message.assign(u"Warning: ");
			break;
		case gost::gtLog::msgType::Info:
			break;
		}
		
		gt_va_list args;
		va_start( args, str );
//	print( type, str, args );
		deformat( str, args, message );
		va_end( args );
		
		gtFile_t file = util::openFileForWriteText(u"log.txt");
		file->write( message );
		file->write( gtString(u"\r\n") );

		if( m_out )
			m_out->print( message );
	}
}


void gtLogImpl::print( msgType /*type*/, const char16_t* /*str*/, void * /*p*/ ){
/*
	if( m_msgType >= type ){
		gtString message;
		switch( type ){
		case gost::gtLog::msgType::error:
			message.assign(u"Error: ");
			break;
		case gost::gtLog::msgType::warning:
			message.assign(u"Warning: ");
			break;
		case gost::gtLog::msgType::info:
			break;
		}

		gt_va_list args;// = static_cast<gt_va_list>(p);
		deformat( str, args, message );

		gtFile_t file = util::openFileForWriteText(u"log.txt");
		file->write( message );
		file->write( gtString(u"\r\n") );

		if( m_out )
			m_out->print( message );
	}
*/
}

void gtLogImpl::deformat( const char16_t* fmt,
	gt_va_list& args, gtString& message ){

	u32 len = 0U;
	const char16_t* p = fmt;
	do	{		++len;	} while(*p++);
	--len;

	va_list list = (va_list)args;

	bool S = false;
	for( u32 i(0u); i < len; ++i ){

		std::wostringstream ss;


		if( S ){
			if( fmt[ i ] == u'f' ){
				ss << va_arg( list, f64 );
				message += (char16_t*)ss.str().c_str();
				continue;
			}else if( fmt[ i ] == u'i' ){
				ss << va_arg( list, s32 );
				message += (char16_t*)ss.str().c_str();
				continue;
			}else if( fmt[ i ] == u'u' ){
				ss << va_arg( list, u32 );
				message += (char16_t*)ss.str().c_str();
				continue;
			}else if( fmt[ i ] == u'c' ){
				message += va_arg( list, /*char16_t*/int );
				continue;
			}else if( fmt[ i ] == u's' ){
				char16_t * p2 = va_arg( list, char16_t* );
				u32 len2( 0U );
				do{ ++len2; } while(*p2++);
				p2 -= len2;
				for( u32 o(0U); o < len2-1u; ++o )
					message += p2[ o ];
				continue;
			}
		}

		if( fmt[ i ] == u'%' ){
			if( !S ) S = true;
			else S = false;
		}else S = false;

		if( !S )
			message += fmt[ i ];
	}

}

void gtLogImpl::setOutputWindow( gtOutputWindow* out ){
	m_out = out;
}


void gtLogImpl::setInfoType( msgType type ){
	m_msgType = type;
}


void gtLogWriter::printError( const char16_t* str, ... ){
	gt_va_list args;
	va_start( args, str );
	gtMainSystemCommon::s_log->print( gost::gtLog::msgType::Error, str, args );
	va_end( args );
}


void gtLogWriter::printWarning( const char16_t* str, ... ){
	gt_va_list args;
	va_start( args, str );
	gtMainSystemCommon::s_log->print( gost::gtLog::msgType::Warning, str, args );
	va_end( args );
}


void gtLogWriter::printInfo( const char16_t* str, ... ){
	gt_va_list args;
	va_start( args, str );
	gtMainSystemCommon::s_log->print( gost::gtLog::msgType::Info, str, args );
	va_end( args );
}

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
