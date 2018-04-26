#pragma once
#ifndef __GT_DEMO_APP_H__
#define __GT_DEMO_APP_H__

namespace demo{

	enum class DemoState : u32 {
		MainMenu,
		DemoMenu,
		DemoRun
	};

	struct DemoLang{
		gtString m_langName;
		gtArray<gtString> m_stringArray;
	};

	class DemoApplication{

		gtPtr<gtMainSystem>			m_mainSystem;
		gtPtr<gtWindow>				m_mainWindow;
#ifdef GT_PLATFORM_WIN32
		gtPtr<gtOutputWindow>		m_outputWindow;
#endif
		gtGUISystem*				m_guiSystem;
		gtPtr<gtDriver>				m_driver;
		gtPtr<gtGameController>		m_gamepadSystem;
		gtGameControllerDevice *	m_gamepad;

		gtTexture*			m_backgroundTexture;
		gtTexture*			m_gamepadTexture;
		gtPtr<gtGUIShape>	m_backgroundShape;
		gtPtr<gtGUIShape>	m_gamepadiconShape;
		gtPtr<gtGUITextField> m_welcomeText;
		gtPtr<gtGUIFont>	m_mainFont;

		demo::DemoApplicationEventConsumer* m_eventConsumer;
		gtDeviceCreationParameters  m_params;
		gtWindowInfo				m_windowInfo;
		gtDriverInfo				m_driverInfo;

		DemoState					m_state;

		gtArray<DemoLang>			m_stringArray;

		bool initEngine( void );
		bool initWindow( void );
		bool initVideoDriver( void );
		bool initMainMenu( void );
		bool initStrings( void );

		bool rebuildMainMenu( void );

		void renderMainMenu( void );

	public:
		DemoApplication( void );
		~DemoApplication( void );

		bool Init( void );
		void Run( void );

		void RebuildGUI( void );
		void ScanGamepads( void );
		void ActivateGamepad( bool, gtGameControllerDevice* d = nullptr );

		gtMainSystem	*	GetMainSystem( void );
	};

}

#endif


/*
Copyright (c) 2018 532235

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