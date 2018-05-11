#pragma once
#ifndef __GT_MAIN_SYSTEM_WIN32_H__
#define __GT_MAIN_SYSTEM_WIN32_H__

#if defined(GT_PLATFORM_WIN32)

namespace gost{
	 
	class gtMainSystemWin32 GT_FINAL: public gtMainSystemCommon{

		gtPtr<gtTimerWin32> m_timer;
		
		void updateWindowEvents();
		void quit();

	public:

		gtMainSystemWin32( const gtDeviceCreationParameters& );

		virtual ~gtMainSystemWin32();

		void			shutdown();
		bool			init();
		gtOutputWindow* getOutputWindow() GT_FINAL;
		bool			update() GT_FINAL;
		gtPtr<gtWindow>	createSystemWindow( gtWindowInfo* );
		u32				getTime();
		gtTimer*		getTimer();
		gtPtr<gtThread>	createThread();
		gtPtr<gtMutex>	createMutex();
		v2i				getScreenSize();
	};

}

#endif
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