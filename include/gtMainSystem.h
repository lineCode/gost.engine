#pragma once
#ifndef __GT_MAIN_SYSTEM_H__
#define __GT_MAIN_SYSTEM_H__

namespace gost{

	class gtOutputWindow;
	class gtLog;
	class gtWindow;
	struct gtWindowInfo;
	class gtDriver;
	struct gtDriverInfo;
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

			// c-tor
		gtDeviceCreationParameters( void ):
			m_outputWindow( nullptr ),
			m_consumer( nullptr )
		{
#if defined(GT_PLATFORM_WIN32)
			m_device_type	=	gtDeviceType::Windows;
#elif defined(GT_PLATFORM_LINUX)
            m_device_type	=	gtDeviceType::Linux;
#else
#error "Эта ОС не поддерживается"
#endif
			m_outputWindow = nullptr;

			m_fontName = u"consola.ttf";
		}

			// d-tor
		~gtDeviceCreationParameters( void ){}

			// Font for output window
		gtString			m_fontName;

		gtDeviceType		m_device_type;

		gtOutputWindow*		m_outputWindow;

			// user event consumer
		gtEventConsumer*	m_consumer;

	};

	class gtMainSystem : public gtRefObject{
	public:

			//	send command for stop engine
		virtual void shutdown( void ) = 0;

			// check is engine run
			//	\return \b true if run
		virtual bool isRun( void ) = 0;

			//	get output window
			//	\return output window
		virtual gtOutputWindow* getOutputWindow( void ) = 0;

			//	\return logger
		virtual gtLog*		getLog( void ) = 0;

			//	\return \b true if engine run
		virtual	bool	update( void ) = 0;

		virtual gtPtr<gtAudioSystem> createAudioSystem( const GT_GUID& uid ) = 0;

		virtual gtPtr<gtWindow>		createSystemWindow( gtWindowInfo* wi ) = 0;

		virtual gtPtr<gtDriver>		createVideoDriver( /*gtPlugin* videoDriverPlugin, */const gtDriverInfo& di, const GT_GUID& uid ) = 0;

		virtual bool		allocateMemory( void** data, u32 size ) = 0;

		virtual void		freeMemory( void** data ) = 0;

		virtual gtPtr<gtImage>	loadImage( const gtString& fileName ) = 0;

		virtual gtPtr<gtImage>	loadImage( const gtString& fileName, const GT_GUID& pluginGUID ) = 0;

		virtual u32			getTime( void ) = 0;

		virtual gtTimer*	getTimer( void ) = 0;

		virtual void		addEvent( const gtEvent& ev, u8 prior = 0u ) = 0;


		virtual gtModelSystem*	getModelSystem( void ) = 0;

		virtual gtPluginSystem*	getPluginSystem( void ) = 0;

		virtual gtSceneSystem*	getSceneSystem( gtDriver * currentRenderDriver ) = 0;

		virtual gtGUISystem*	getGUISystem( gtDriver * currentRenderDriver ) = 0;

		virtual const gtMatrix4& getMatrixWorld( void ) = 0;

		virtual const gtMatrix4& getMatrixView( void ) = 0;

		virtual const gtMatrix4& getMatrixProjection( void ) = 0;

		virtual void setMatrixWorld( const gtMatrix4& m ) = 0;

		virtual void setMatrixView( const gtMatrix4& m ) = 0;

		virtual void setMatrixProjection( const gtMatrix4& m ) = 0;

		virtual bool pollEvent( gtEvent& event ) = 0;

			//	Check pressed key
			//	\param key: key code
			//	\return \b true if pressed
		virtual bool isKeyPressed( gtKey key ) = 0;

			//	Left mouse button down
			//	\return \b true if down
		virtual bool isLMBDown( void ) = 0;

			//	Right mouse button down
			//	\return \b true if down
		virtual bool isRMBDown( void ) = 0;

			//	Middle mouse button down
			//	\return \b true if down
		virtual bool isMMBDown( void ) = 0;

			//	Get cursor position
			//	\return position
		virtual const gtVector2<u16>& getCursorPosition( void ) = 0;

			//	Get params
		virtual const gtDeviceCreationParameters& getDeviceCreationParameters( void ) = 0;

			//	Create thread object
		virtual gtPtr<gtThread> createThread( void ) = 0;

		virtual gtPtr<gtMutex> createMutex( void ) = 0;

		virtual gtPtr<gtXMLDocument> XMLRead( const gtString& file ) = 0;

		virtual void XMLWrite( const gtString& file, gtXMLNode* rootNode, bool utf8 = false ) = 0;

		virtual u32 getLoadedVideoDriverCount( void ) = 0;

		virtual gtDriver* getLoadedVideoDriver( u32 id ) = 0;

		virtual gtDriver* getMainVideoDriver( void ) = 0;

		virtual void setMainVideoDriver( gtDriver* d ) = 0;

		virtual gtPtr<gtGameController> createGameContoller( const GT_GUID& plugin ) = 0;

		virtual v2i getScreenSize( void ) = 0;

			// Engine send `System event` when timer = 0
		virtual void setTimer( u32 milliseconds ) = 0;

		GT_API static gtMainSystem* getInstance( void );


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
