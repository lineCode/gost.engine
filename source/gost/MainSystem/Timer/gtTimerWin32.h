﻿//	GOST

#pragma once
#ifndef __GT_TIMER_WIN32_H__
#define __GT_TIMER_WIN32_H__

#if defined(GT_PLATFORM_WIN32)

namespace gost{

	class gtTimerWin32 GT_FINAL : public gtTimer{
		gtMainSystem * m_ms;
		f32 m_delta;
		u32 m_last;
		u32 m_now;
	public:

		gtTimerWin32();
		virtual ~gtTimerWin32();

		gtRealTime	getRealTime() GT_FINAL;
		f32*        getDelta()    GT_FINAL;

		void updateDelta();
	};

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