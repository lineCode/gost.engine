#pragma once
#ifndef __GT_DEMO_APP_H__
#define __GT_DEMO_APP_H__

#define DEMO_TYPE_NUM 13u

#define DEMO_COMMON gtConst0U
#define DEMO_EVENTS gtConst1U
#define DEMO_AUDIO gtConst2U
#define DEMO_VIDEO gtConst3U
#define DEMO_PICTURES gtConst4U
#define DEMO_MODELS gtConst5U
#define DEMO_INPUT gtConst6U
#define DEMO_GUI gtConst7U
#define DEMO_GAME_OBJECTS gtConst8U
#define DEMO_FILE_SYSTEM gtConst9U
#define DEMO_PHYSICS 1gtConst0U
#define DEMO_OTHER 11u
#define DEMO_DEMONSTRATION 12u

namespace demo{

	enum class DemoState : u32 {
		MainMenu,
		DemoMenu,
		DemoRun
	};

	struct DemoLang{
		gtString m_langName;
		gtArray<gtPair<gtString,gtString>/*gtString*/> m_stringArray;
	};

	enum class DemoAudioType {
		Select,
		Accept,
		Cancel
	};

	class DemoApplication{
		

		gtPtr<gtMainSystem>			m_mainSystem;
		gtPtr<gtWindow>				m_mainWindow;

		gtGUISystem*				m_guiSystem;
		gtPtr<gtGraphicsSystem>		m_gs;
		gtPtr<gtInputController>	m_gamepadSystem;
		gtInputSystem *				m_input;
		gtInputDevice *				m_gamepad;
		gtSceneSystem*				m_sceneSystem;

		gtPtr<gtOutputWindow>		m_outputWindow;

		gtPtr<gtAudioSystem>		m_audioSystem;
		gtPtr<gtAudioObject>		m_audioSelect;
		gtPtr<gtAudioObject>		m_audioAccept;
		gtPtr<gtAudioObject>		m_audioCancel;
		void						playAudio( DemoAudioType );

		gtPtr<gtXMLDocument>		m_xml;
		void						xmlLoadSettings();
		void						xmlSaveSettings();
		void						xmlCreateDrefaultSettingsFile();
		gtString					m_xmlPath;

		gtTexture*					m_backgroundTexture;
		gtTexture*					m_gamepadTexture;
		gtPtr<gtGUIShape>			m_backgroundShape;
		gtPtr<gtGUIShape>			m_gamepadiconShape;
		gtPtr<gtGUITextField>		m_welcomeText;
		gtPtr<gtGUIFont>			m_mainFont;
		gtPtr<gtGUIFont>			m_infoFont;
		
		gtPtr<gtGUITextField>	m_demoText_camera;

		gtPtr<gtGUITextField>	m_leftColonEntity[DEMO_TYPE_NUM];
		gtPtr<gtGUITextField>	m_rightColonEntity[24u];
		gtPtr<gtGUIShape>		m_leftColonShape;
		gtPtr<gtGUIShape>		m_rightColonShape;
		gtPtr<gtGUITextField>	m_rightColonDefaultText;
		v4i						m_rightColonDefaultRect;

		gtPtr<gtGUIShape>		m_descriptionBackgroundShape;
		gtPtr<gtGUITextField>	m_description;
		v4i						m_descriptionRect;

		gtPtr<gtGUIShape>		m_pauseBackgroundShape;
		gtPtr<gtGUIShape>		m_pauseShape;
		gtPtr<gtGUITextField>	m_pauseTextContinueShape;
		gtPtr<gtGUITextField>	m_pauseTextSettingsShape;
		gtPtr<gtGUITextField>	m_pauseTextMainMenuShape;
		gtPtr<gtGUITextField>	m_pauseTextExitShape;
		s32						m_pauseMainMenuSelectedId;
		void					updatePauseMainMenu();


		bool					m_isPause;
		bool					m_isSettings;
		bool					m_useSound;
		bool					m_showDescription;
		bool					m_sceneInitialized;

		gtPtr<gtGUIShape>		m_settingsBackgroundShape;
		gtPtr<gtGUITextField>	m_settingsTextLanguage;
		gtPtr<gtGUITextField>	m_settingsTextLanguageName;
		gtPtr<gtGUITextField>	m_settingsTextSound;
		gtPtr<gtGUITextField>	m_settingsTextSoundUse;
		void					updateSettings();
		s16						m_settingsTypeID;
		s16						m_demoPauseMenuID;
		void					updateDemoPause();


		s32		m_languageID;
		s32		m_activeDemoType; //left colon
		s32		m_activeDemoTypeSelected;
		s32		m_activeDemoSelected;
		s32		m_rightColonFirstID; // first textfield have demo id
		s32		m_currentDemoColonIndex;

		demo::DemoApplicationEventConsumer* m_eventConsumer;
		gtDeviceCreationParameters  m_params;
		gtWindowInfo				m_windowInfo;
		gtGraphicsSystemInfo		m_gsInfo;

		DemoState					m_state;

		gtArray<DemoLang>			m_stringArray;
		gtArray<DemoElement>		m_demoArrays[ DEMO_TYPE_NUM ];

		bool		m_DPad[ gtConst4U ];
		bool		m_DPadOnce;
		bool		m_gamepadButtons[32];
		f32			m_delta;

		bool initEngine();
		bool initWindow();
		bool initVideoDriver();
		bool initMainMenu();
		bool initStrings();
		void initAudio();


		void addDemo( u32 index, const demo::DemoElement& element );
		void updateDemoText();
		void updateSettingsText();

		const gtString& getString( const gtString& a );

		void rebuildMainMenuColons();
		bool rebuildMainMenu();

		void updateColons();

		void inputMainMenu();
		void inputMainMenuPause();
		void inputDemoMenuPause();

		void runMainMenu();
		void runDemo();
		void runDemoMenu();


		void renderMainMenu();
		void renderDemoMenu();
		void renderDemo();

		void showDemoHUD();
		void hideDemoHUD();
		void updateHUD();

		f32 timer_input;
		f32 timer_input_limit_first;
		f32 timer_input_limit_second;
		f32 timer_input_limit;

		void pauseBackgroundFadeIn();
		void pauseBackgroundFadeOut();

		static DemoApplication * s_this;

	public:


		DemoApplication();
		~DemoApplication();

		bool Init();
		void Run();

		void RebuildGUI();
		void RefreshGUI();
		void ScanGamepads();
		void ActivateGamepad( bool, gtInputDevice* d = nullptr );
		void UpdateGamepad();

		gtMainSystem	*	GetMainSystem();
		demo::DemoApplicationEventConsumer	*	GetEventConsumer();
		
		bool InitDefaultScene();
		void RenderDefaultScene();
		void ShutdownDefaultScene();
		
		void Render();
		void HideMenu();
		void ShowMenu();
		void Pause();

		bool inputGamepadMainMenuUp();
		bool inputGamepadMainMenuDown();
		bool inputGamepadMainMenuLeft();
		bool inputGamepadMainMenuRight();
		bool inputGamepadMainMenuEnter();
		bool inputGamepadMainMenuEscape();
		bool inputGamepadMainMenuStart();
		bool inputGamepadMainMenuSelect();

		bool inputGamepadSelectHold();
		bool inputGamepadUpHold();
		bool inputGamepadDownHold();
		bool inputGamepadLeftHold();
		bool inputGamepadRightHold();
		bool inputGamepadL1Hold();
		bool inputGamepadR1Hold();

		bool update();

		void StopDemo();
		void ReturnToMainMenu();
		void TerminateProgram();

		static DemoApplication*	GetThis();

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