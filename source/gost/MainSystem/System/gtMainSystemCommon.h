#pragma once
#ifndef __GT_MAIN_SYSTEM_COMMON_H__
#define __GT_MAIN_SYSTEM_COMMON_H__

namespace gost{

	
	class gtLogImpl;

	class gtMainSystemCommon : public gtMainSystem{
		
		gtArray<gtGraphicsSystem*>	m_drivers;
		bool				m_useTimer;
		u32					m_timer;
		u32					m_time;
		u32					m_tick;

	protected:

		void	updateTimer();
		void	updateEvents();

		gtPtr<gtOutputWindow>		m_output_window;
		gtList< gtPtr< gtWindow > > m_windowCache;

		gtDeviceCreationParameters	m_params;
		bool						m_isRun;
		
		gtStackTracer*				m_stackTracer;
		
		u32							m_systemWindowCount;

		gtPtr<gtInputSystemImpl>	m_inputSystem;
		gtPtr<gtEventSystem>		m_events;
		gtPtr<gtPluginSystemImpl>	m_pluginSystem;
		gtPtr<gtModelSystemImpl>	m_modelSystem;
		gtPtr<gtSceneSystemImpl>	m_sceneSystem;
		gtPtr<gtGUISystemImpl>		m_GUISystem;


		static gtMainSystemCommon*	s_instance;

		gtMatrix4					m_WVP[ gtConst3U ];

		gtGraphicsSystem *					m_gs;

	public:

		static gtFileSystemCommon*	s_fileSystem;
		//static gtMemorySystemCommon*s_memorySystem;

		gtMainSystemCommon();
		virtual ~gtMainSystemCommon();

		static gtPtr<gtLogImpl> s_log;
		
		bool		isRun();
		gtLog*		getLog();
		void		initStackTracer();
		void		initEventSystem();

		static gtMainSystemCommon * getInstance();
		gtStackTracer*				getStackTracer();
		gtPtr<gtAudioSystem>		createAudioSystem( const GT_GUID& uid );
		gtPtr<gtGraphicsSystem>		createGraphicsSystem( /*gtPlugin* videoDriverPlugin,*/ const gtGraphicsSystemInfo&, const GT_GUID& uid );

		gtPtr<gtImage>	loadImage( const gtString& fileName );
		gtPtr<gtImage>	loadImage( const gtString& fileName, const GT_GUID& pluginGUID );
		void			addEvent( const gtEvent&, u8 prior = gtConst0U );
		gtInputSystem*  getInputSystem();
		gtModelSystem*	getModelSystem();
		gtPluginSystem*	getPluginSystem();
		gtSceneSystem*	getSceneSystem( gtGraphicsSystem * currentRenderDriver );
		gtGUISystem*	getGUISystem( gtGraphicsSystem * currentRenderDriver );

		const gtMatrix4& getMatrixWorld();
		const gtMatrix4& getMatrixView();
		const gtMatrix4& getMatrixProjection();
		void setMatrixWorld( const gtMatrix4& );
		void setMatrixView( const gtMatrix4& );
		void setMatrixProjection( const gtMatrix4& );

		bool pollEvent( gtEvent& event );
		const gtDeviceCreationParameters&	getDeviceCreationParameters();

		u32						getLoadedVideoDriverCount();
		gtGraphicsSystem*		getLoadedVideoDriver( u32 id );
		gtGraphicsSystem*		getMainVideoDriver();
		void					setMainVideoDriver( gtGraphicsSystem* d );
		void					setTimer( u32 milliseconds );
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