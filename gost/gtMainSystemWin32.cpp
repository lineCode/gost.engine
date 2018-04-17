//	GOST

#include "common.h"

#ifdef GT_PLATFORM_WIN32

namespace gost{

		//	конструктор
	gtMainSystemWin32::gtMainSystemWin32( const gtDeviceCreationParameters& params ){
		m_params = params;

	}

		//	деструктор
	gtMainSystemWin32::~gtMainSystemWin32( void ){
		this->m_isRun = false;
	}

		//	возвратит укозатель на окно вывода
	gtOutputWindow* gtMainSystemWin32::getOutputWindow( void ){
		return this->m_output_window.data();
	}

	bool	gtMainSystemWin32::init( void ){
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
				this->m_output_window = gtPtrNew<gtOutputWindow>( m_params.m_outputWindow );
			}

			gtLog->setOutputWindow( m_output_window.data() );
			//m_output_window->release();

			this->initStackTracer();
			this->initEventSystem();

			this->s_fileSystem = new gtFileSystemWin32;
			//this->m_GUISystem->init();

			if( this->s_fileSystem->existFile( u"log.txt" ) )
				this->s_fileSystem->deleteFile( u"log.txt" );

			gtLog->print(gtLoger::msgType::info, u"Starting GoST version %i.%i", 0, 1 );

			if( !this->m_pluginSystem->init() ){
				return false;
			}

		}

		return true;
	}

		//	получает и обрабатывает оконные сообщения
	void gtMainSystemWin32::updateWindowEvents( void ){
		MSG msg;
		while( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) ){
			if( !GetMessage( &msg, NULL, 0, 0 )){
				this->quit();
			}
			TranslateMessage (&msg);
		  	DispatchMessage (&msg);
		}
	}


	bool	gtMainSystemWin32::update( void ){

		if( m_isRun ){

			this->m_events->resetEvents();

			this->updateWindowEvents();

			if( this->m_events->getConsumer() ){
				this->m_events->runEventLoop();
			}else{

			}
		}

		return m_isRun;
	}

		//	завершает работу
	void gtMainSystemWin32::quit( void ){
		if( m_isRun ){
			m_isRun = false;
			PostQuitMessage( 0 );
		}
	}

	gtPtr<gtWindow>	gtMainSystemWin32::createSystemWindow( gtWindowInfo* wi ){
		gtPtr< gtWindow > window( gtPtrNew< gtWindow >( new gtWindowWin32( wi ) ) );

		if( !((gtWindowWin32*)window.data())->init( ++m_systemWindowCount ) ){
			gtLogWriter::printError( u"Can not create system window" );
			gtStackTrace::dumpStackTrace();
			return nullptr;
		}

			//	при завершении, так как вставляются gtPtr
			//	удалится всё (должно)
	//	this->m_windowCache.add( gtPtr< gtWindow >(window) );
	//	window->addRef();

#ifdef GT_DEBUG
		window->setDebugName( u"SystemWindow" );
#endif
		wi->m_owner = window.data();

		return window;
	}

	u32			gtMainSystemWin32::getTime( void ){
		static bool isInit = false;
		static u32 baseTime;
		if( !isInit ){
			baseTime = timeGetTime();
			isInit = true;
		}
		return timeGetTime() - baseTime;
	}

		//	получит указатель на таймер
	gtTimer*	gtMainSystemWin32::getTimer( void ){
		return m_timer.data();
	}

	void gtMainSystemWin32::shutdown( void ){

		m_sceneSystem->clearScene();

		quit();
	}
}

gtPtr<gtThread>	gtMainSystemWin32::createThread( void ){
	return gtPtr<gtThread>( gtPtrNew<gtThread>( new gtThreadWin32 ) );
}

gtPtr<gtMutex>	gtMainSystemWin32::createMutex( void ){
	return gtPtr<gtMutex>( gtPtrNew<gtMutex>( new gtMutexWin32 ) );
}

#endif

/*
Copyright (c) 2017, 2018 532235

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
