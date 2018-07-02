#include "common.h"

#ifdef GT_PLATFORM_WIN32

namespace gost{

	gtMainSystemWin32::gtMainSystemWin32( const gtDeviceCreationParameters& params ){
		m_params = params;
	}

	gtMainSystemWin32::~gtMainSystemWin32(){
		this->m_isRun = false;

		// ???
		//delete this->s_fileSystem;
		//delete this->s_memorySystem;
	}

	gtOutputWindow* gtMainSystemWin32::getOutputWindow(){
		return this->m_output_window.data();
	}

	bool	gtMainSystemWin32::init(){
		// Почему я выделяю память но не освобождаю?
		//this->s_memorySystem = new gtMemorySystemImplWin32;
		

        timeBeginPeriod(1);

		getTime();
		m_timer = gtPtrNew<gtTimerWin32>( new gtTimerWin32 );

		{
			if( !m_params.m_outputWindow ){
				this->m_output_window = gtPtrNew<gtOutputWindow>( new gtOutputWindowWin32 );
				this->m_output_window->init();
				this->m_output_window->show();
				m_params.m_outputWindow = this->m_output_window.data();
			}else{
				this->m_output_window = m_params.m_outputWindow;
			}

			gtMainSystemCommon::s_log->setOutputWindow( m_output_window.data() );

			this->initStackTracer();
			this->initEventSystem();

			// Почему я выделяю память но не освобождаю?
			this->s_fileSystem = new gtFileSystemWin32;

			if( this->s_fileSystem->existFile( u"log.txt" ) )
				this->s_fileSystem->deleteFile( u"log.txt" );
			
			gtMainSystemCommon::s_log->print( gtLog::msgType::Info, u"Starting GoST version %i.%i", 0, 1 );

			if( !this->m_pluginSystem->init() ){
				return false;
			}

		}

		return true;
	}

	void gtMainSystemWin32::updateWindowEvents(){
		MSG msg;
		while( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) ){
			if( !GetMessage( &msg, NULL, 0, 0 )){
				this->quit();
			}
			TranslateMessage (&msg);
		  	DispatchMessage (&msg);
		}
	}


	bool	gtMainSystemWin32::update(){
		if( m_isRun ){
			
			this->updateWindowEvents();

			this->m_events->getKeyboardAndMouseStates();

			if( this->m_events->getConsumer() ){
				this->m_events->runEventLoop();
				this->m_events->resetEvents();
			}else{

			}

			updateTimer();
		}


		return m_isRun;
	}

	void gtMainSystemWin32::quit(){
		if( m_isRun ){
			m_isRun = false;
			PostQuitMessage( 0 );
		}
	}

	gtPtr<gtWindow>	gtMainSystemWin32::createSystemWindow( gtWindowInfo* wi ){
		gtPtr< gtWindow > window( gtPtrNew< gtWindow >( new gtWindowWin32( wi ) ) );

		if( !((gtWindowWin32*)window.data())->init( ++m_systemWindowCount ) ){
			gtLogWriter::printError( u"Can not create system window" );
			gtStackTracer::dumpStackTrace();
			return nullptr;
		}

		wi->m_owner = window.data();

		return window;
	}

	u32			gtMainSystemWin32::getTime(){
		static bool isInit = false;
		static u32 baseTime;
		if( !isInit ){
			baseTime = timeGetTime();
			isInit = true;
		}
		return timeGetTime() - baseTime;
	}

	gtTimer*	gtMainSystemWin32::getTimer(){
		return m_timer.data();
	}

	void gtMainSystemWin32::shutdown(){

		m_sceneSystem->clearScene();

		quit();
	}
}

gtPtr<gtThread>	gtMainSystemWin32::createThread(){
	return gtPtr<gtThread>( gtPtrNew<gtThread>( new gtThreadWin32 ) );
}

gtPtr<gtMutex>	gtMainSystemWin32::createMutex(){
	return gtPtr<gtMutex>( gtPtrNew<gtMutex>( new gtMutexWin32 ) );
}

v2i gtMainSystemWin32::getScreenSize(){
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	return v2i( desktop.right, desktop.bottom );
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
