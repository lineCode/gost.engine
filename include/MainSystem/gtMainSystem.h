#pragma once
#ifndef __GT_MAIN_SYSTEM_H__
#define __GT_MAIN_SYSTEM_H__

namespace gost{

	class gtOutputWindow;
	class gtLog;
	class gtWindow;
	struct gtWindowInfo;
	class gtGraphicsSystem;
	struct gtGraphicsSystemInfo;
	class gtDebugRenderer;
	class gtTimer;

	class gtModel;
	class gtModelSystem;
	class gtPluginSystem;
	class gtFileSystem;

	class gtGameObject;
	class gtStaticObject;
	class gtCamera;
	class gtPlugin;

	enum class gtDeviceType{
		Android,	//< not implemented
		iOS,		//< not implemented
        Linux,		//< not implemented
		OSX,		//< not implemented
		Windows,	
		XBox,		//< not implemented
		PlayStation,//< not implemented
		Wii			//< not implemented
	};

	struct gtDeviceCreationParameters{

		gtDeviceCreationParameters():
			m_outputWindow( nullptr ),
			m_consumer( nullptr )
		{
#if defined(GT_PLATFORM_WIN32)
			m_device_type	=	gtDeviceType::Windows;
#elif defined(GT_PLATFORM_LINUX)
            m_device_type	=	gtDeviceType::Linux;
#elif defined(GT_PLATFORM_ANDROID)
            m_device_type	=	gtDeviceType::Android;
#else
#error "This OS is not supported yet. Visit http://gost.imsoftworks.info/ for additional information."
#endif
			m_outputWindow = nullptr;

			m_fontName = u"consola.ttf";
		}

		~gtDeviceCreationParameters(){}

			// Font for output window
		gtString			m_fontName;

		gtDeviceType		m_device_type;

		gtOutputWindow*		m_outputWindow;

			// user event consumer
		gtEventConsumer*	m_consumer;

	};

		// Этот класс управляет всей системой
	class gtMainSystem : public gtRefObject{
	public:
	
			// Добавить событие в очередь. prior - не используется
		virtual void addEvent( const gtEvent& ev, u8 prior = gtConst0U ) = 0;
		
			// Проверить нахождение события в очереди.
		virtual bool checkEventType( const gtEvent& ev ) = 0;
		
			// Проверить нахождение события в очереди. Можно указать функцию сравнения.
		virtual bool checkEvent( gtEvent& ev, bool(*compare_function)( gtEvent& current_event, gtEvent& user_event ) ) = 0;
		
		virtual gtPtr<gtAudioSystem>    createAudioSystem( const GT_GUID& uid ) = 0;
		virtual gtPtr<gtGraphicsSystem> createGraphicsSystem( const gtGraphicsSystemInfo& gsi, const GT_GUID& uid ) = 0;
		virtual gtPtr<gtMutex>          createMutex() = 0;
		virtual gtPtr<gtPhysicsSystem>  createPhysicsSystem( const gtPhysicsSystemInfo& psi, const GT_GUID& uid ) = 0;
		virtual gtPtr<gtWindow>	        createSystemWindow( gtWindowInfo* wi ) = 0;
		virtual gtPtr<gtThread>         createThread() = 0;
		virtual const gtDeviceCreationParameters& getDeviceCreationParameters() = 0;
		
			// При первом вызове нужно указать currentRenderDriver. Для последующих вызовов можно передавать nullptr
		virtual gtGUISystem*     getGUISystem( gtGraphicsSystem * currentRenderDriver ) = 0;
		virtual gtCVarSystem*    getCVarSystem() = 0;
		GT_API static gtMainSystem* getInstance();
		virtual gtInputSystem*   getInputSystem() = 0;
		
			// Тестовая штука. Думаю не нужна. Если убрать, то не нужно указывать currentRenderDriver в getGUISystem и других.
			// Но из за архитектуры движка следует что VideoDriver-ов может быть множество, по этому можно указывать основной драйвер и т.д.
			// Если использовать 2 окна вывода с разными gtGraphicsSystem то рисовать всё придётся вручную, без использования gtSceneSystem
		virtual gtGraphicsSystem*getLoadedVideoDriver( u32 id ) = 0;
		virtual u32              getLoadedVideoDriverCount() = 0;
		
		virtual gtLog*           getLog() = 0;
		
			//	Используется для быстрого доступа к gtGraphicsSystem 
		virtual gtGraphicsSystem*getMainVideoDriver() = 0;
		
			//  *должен быть создать хоть 1 gtGraphicsSystem
		virtual gtDebugRenderer* getDebugRenderer() = 0;
		
		virtual u32              getMajorVersion() = 0;
		virtual u32              getMinorVersion() = 0;
		
		virtual const gtMatrix4& getMatrixProjection() = 0;
		virtual const gtMatrix4& getMatrixView() = 0;
		virtual const gtMatrix4& getMatrixWorld() = 0;
		virtual gtModelSystem*   getModelSystem() = 0;
		virtual gtOutputWindow*  getOutputWindow() = 0;
		virtual gtPluginSystem*	 getPluginSystem() = 0;
		virtual gtSceneSystem*   getSceneSystem( gtGraphicsSystem * currentRenderDriver ) = 0;
		
		//????
		virtual v2i              getScreenSize() = 0;
		
		
		virtual u32              getTime() = 0;
		virtual gtTimer*         getTimer() = 0;
		virtual bool isRun() = 0;
		
		// Загрузит картинку, используя первый попавшийся плагин который поддерживает расширение указанного файла
		virtual gtPtr<gtImage> loadImage( const gtString& fileName ) = 0;
		
		// Загрузит картинку используя указанный плагин
		virtual gtPtr<gtImage> loadImage( const gtString& fileName, const GT_GUID& pluginGUID ) = 0;
		
		virtual bool pollEvent( gtEvent& event ) = 0;
		
		// Конвертирует координаты курсора в 3D координату в пространстве
		virtual v4f  screenToWorld( const gtVector2<s16>& coord ) = 0;
		
		// Вернёт луч, отрезок от screenToWorld(координаты курсора) в сторону camera->getFar() на расстоянии len
		virtual gtRayf32 getRayFromScreen( const gtVector2<s16>& coord, f32 len = 100.f ) = 0;
		
		
		virtual void setMainVideoDriver( gtGraphicsSystem* d ) = 0;
		virtual void setMatrixProjection( const gtMatrix4& m ) = 0;
		virtual void setMatrixView( const gtMatrix4& m ) = 0;	
		virtual void setMatrixWorld( const gtMatrix4& m ) = 0;
		
		// Установит таймер, и при достижении нуля поступит событие о конце отсчёта
		// ev.systemEvent.eventID == GT_EVENT_SYSTEM_TIMER
		virtual void setTimer( u32 milliseconds ) = 0;
		
		// Завершит работу движка
		virtual void shutdown() = 0;
		
		// Обновит состояние движка, получит все системные сообщения, отправит события и т.д.
		virtual	bool update() = 0;

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
