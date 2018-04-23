//	GOST

#pragma once
#ifndef __GT_MAIN_SYSTEM_WIN32_H__
#define __GT_MAIN_SYSTEM_WIN32_H__

#if defined(GT_PLATFORM_WIN32)

/*
	Реализация главной системы для Windows  
*/

namespace gost{
	 
		//	Реализация главной системы для Windows
	class gtMainSystemWin32 GT_FINAL: public gtMainSystemCommon{

		gtPtr<gtTimerWin32> m_timer;

			//	получает и обрабатывает оконные сообщения
		void updateWindowEvents( void );

			//	завершает работу
		void quit( void );

	public:

			//	конструктор
		gtMainSystemWin32( const gtDeviceCreationParameters& );

			//	деструктор
		virtual ~gtMainSystemWin32( void );

		void shutdown( void );

			//	инициализация в отдельном методе чтобы не занимались проблемами внутри конструктора
		bool	init( void );

			//	получить окно вывода
		gtOutputWindow* getOutputWindow( void ) GT_FINAL;

		bool	update( void ) GT_FINAL;

		gtPtr<gtWindow>	createSystemWindow( gtWindowInfo* );

			//	получит время прошедшее с момента запуска движка
		u32			getTime( void );
			//	получит указатель на таймер
		gtTimer*	getTimer( void );

		gtPtr<gtThread>	createThread( void );

		gtPtr<gtMutex>	createMutex( void );

		v2i getScreenSize( void );
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