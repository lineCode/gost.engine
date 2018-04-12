//	GOST

#pragma once
#ifndef __GT_MAIN_SYSTEM_COMMON_H__
#define __GT_MAIN_SYSTEM_COMMON_H__

/*
	Общий класс 
*/

namespace gost{

	
		//	Общий класс для конкретных реализаций gtMainSystem
	class gtLogerImpl;

	class gtMainSystemCommon : public gtMainSystem{

		struct gtMemoryTableNode{
			gtMemoryTableNode():m_address(nullptr), m_size( 0u ){}
			gtMemoryTableNode(gtAddressType*A,gtAddressType S):m_address(A), m_size(S){}
			gtAddressType*m_address;
			gtAddressType m_size;
		};

		struct gtMemoryTable{
			gtMemoryTable():m_used(0u){}

			gtList<gtMemoryTableNode> m_table;
			gtAddressType m_used;

			void add( void ** address, gtAddressType size ){
				m_table.push_back( gtMemoryTableNode( (gtAddressType*)*address, size ) );
				m_used += size;
				gtLogWriter::printInfo( u"Mem: %u", m_used );
			}

			void remove( void ** address ){
				gtAddressType* ad = (gtAddressType*)*address;

				auto it = m_table.begin();
				for(; it != m_table.end(); ++it){
					if( (*it).m_address == ad ){
						break;
					}
				}

				m_used -= (*it).m_size;
				m_table.erase( it );
			}

		}m_memTable;


		gtArray<gtDriver*>	m_drivers;

	protected:

		gtPtr<gtOutputWindow>	m_output_window;
		gtList< gtPtr< gtWindow > > m_windowCache;


			//	параметры главной системы
		gtDeviceCreationParameters	m_params;

			//	она возвращается в методе update
		bool	m_isRun;
		
			//	объект для вывода стека вызова функций
		gtStackTrace* m_stackTracer;
		
		u32 m_systemWindowCount;

		gtPtr<gtEventSystem> m_events;
		gtPtr<gtPluginSystemImpl>	m_pluginSystem;
		gtPtr<gtModelSystemImpl>	m_modelSystem;
		gtPtr<gtSceneSystemImpl>	m_sceneSystem;
		gtPtr<gtGUISystemImpl>		m_GUISystem;


	//	gtEngineEventConsumer m_engineConsumer;

		static gtMainSystemCommon*			s_instance;

		gtMatrix4 m_WVP[ 3u ];

		gtDriver * m_driver;

	public:

		static gtFileSystemCommon*	s_fileSystem;

			//	конструктор
		gtMainSystemCommon( void );

			//	деструктор
		virtual ~gtMainSystemCommon( void );

		static gtPtr<gtLogerImpl> s_loger;

	//	static gtFileSystemCommon* s_fileSystem;
		
		bool isRun( void );

		gtLoger*		getLoger( void );

			//	так как трейсеру нужно окно вывода, а это окно создаётся чуть позже выделения памяти для него
			//	то, после создания окна, вызывается эта функция, чтобы можно было дать трейсеру
			//	инициализированный объект окна
		void initStackTracer( void );

		void initEventSystem( void );

			//	возвратит указатель на gtMainSystem
			//	альтернатива this так как this не работает в статических методах
		static gtMainSystemCommon * getInstance( void );
			
			//	возвратит StackTracer
		gtStackTrace*	getStackTracer( void );

		
		gtPtr<gtAudioSystem> createAudioSystem( const gtString& uid = gtString() );


		gtPtr<gtDriver> createVideoDriver( /*gtPlugin* videoDriverPlugin,*/ const gtDriverInfo&, const gtString& uid );

		bool	allocateMemory( void** data, u32 size );

		void	freeMemory( void** data );

		
		gtPtr<gtImage>	loadImage( const gtString& fileName );

		gtPtr<gtImage>	loadImage( const gtString& fileName, const gtString& pluginGUID );

	//	void		removeImage( gtImage* );

		void		addEvent( const gtEvent&, u8 prior = 0u );


		gtModelSystem*	getModelSystem( void );
		gtPluginSystem*	getPluginSystem( void );
		gtSceneSystem*	getSceneSystem( gtDriver * currentRenderDriver );
		gtGUISystem*	getGUISystem( gtDriver * currentRenderDriver );

		const gtMatrix4& getMatrixWorld( void );
		const gtMatrix4& getMatrixView( void );
		const gtMatrix4& getMatrixProjection( void );
		void setMatrixWorld( const gtMatrix4& );
		void setMatrixView( const gtMatrix4& );
		void setMatrixProjection( const gtMatrix4& );

		bool pollEvent( gtEvent& event );
		bool isKeyPressed( gtKey key );
		bool isLMBDown( void );
		bool isRMBDown( void );
		bool isMMBDown( void );
		const gtVector2<u16>& getCursorPosition( void );
		const gtDeviceCreationParameters& getDeviceCreationParameters( void );

		gtPtr<gtXMLDocument> XMLRead( const gtString& file );
		void XMLWrite( const gtString& file, gtXMLNode* rootNode, bool utf8 = false );
		u32 getLoadedVideoDriverCount( void );
		gtDriver* getLoadedVideoDriver( u32 id );
		gtDriver* getMainVideoDriver();
		void setMainVideoDriver( gtDriver* d );
	};

#define gtLog gtMainSystemCommon::s_loger


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