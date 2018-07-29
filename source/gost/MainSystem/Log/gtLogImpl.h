#pragma once
#ifndef __GT_LOG_IMPL_H__
#define __GT_LOG_IMPL_H__

namespace gost{

	class gtLogImpl GT_FINAL : public gtLog{

		gtOutputWindow*	m_out;
		msgType			m_msgType;
		bool            m_useLog;
		void			deformat( const char16_t* fmt, gt_va_list& args, gtString& );


	public:
		gtLogImpl();
		virtual ~gtLogImpl();

		void print( msgType, const char16_t* str, ... ) GT_FINAL;
		void print( msgType, const char16_t* str, void * );
		void setOutputWindow( gtOutputWindow* ) GT_FINAL;
		void setInfoType( msgType = msgType::Info ) GT_FINAL;
		void useLogFile( bool v = true )GT_FINAL;
	};

}

#endif

/*
Copyright (c) 2017-2018

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
