#pragma once
#ifndef __GT_LOGER_H__
#define __GT_LOGER_H__

namespace gost{

	class gtOutputWindow;

	class gtLoger : public gtRefObject{
	public:

		enum class msgType{
			error,		
			warning,	
			info		
		};

			//	\%f - float
			//	\%i - int
			//	\%u - unsigned
			//	\%s - char16_t*
			//	\%c - char16_t
		virtual void print( msgType type, const char16_t* str, ... ) = 0;

		virtual void setOutputWindow( gtOutputWindow* ) = 0;

		virtual void setInfoType( msgType type = msgType::info ) = 0;


	};

	class gtLogWriter{
	public:
		GT_API static	void printError( const char16_t* str, ... );

		GT_API static	void printWarning( const char16_t* str, ... );

		GT_API static	void printInfo( const char16_t* str, ... );
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
