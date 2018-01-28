//	GOST 

#include "common.h"

gtPtr<gtLogerImpl> gtMainSystemCommon::s_loger;
gtFileSystemCommon* gtMainSystemCommon::s_fileSystem;

	//	возвратит указатель на gtMainSystem
	//	альтернатива this так как this не работает в статических методах
gtMainSystemCommon* gtMainSystemCommon::s_instance;
gtMainSystemCommon* gtMainSystemCommon::getInstance( void ){return s_instance;}

gtMainSystemCommon::gtMainSystemCommon( void ) : m_isRun( true ),
	m_stackTracer( nullptr ), 
	m_systemWindowCount( 0u )
{
//	s_fileSystem = nullptr;

	s_loger = gtPtrNew<gtLogerImpl>( new gtLogerImpl );
	s_instance = this;

	m_pluginSystem	= gtPtrNew<gtPluginSystemImpl>( new gtPluginSystemImpl );
	m_modelSystem = gtPtrNew<gtModelSystemImpl>( new gtModelSystemImpl );
	m_sceneSystem = gtPtrNew<gtSceneSystemImpl>( new gtSceneSystemImpl );

#ifdef GT_DEBUG
	m_debugName.assign(u"gtMainSystem");
#endif


}

gtMainSystemCommon::~gtMainSystemCommon(){
	if( m_stackTracer ){
		delete m_stackTracer;
		m_stackTracer = nullptr;
	}
}

gtLoger* gtMainSystemCommon::getLoger( void ){
	return s_loger.data();
}


void gtMainSystemCommon::initStackTracer( void ){
	if( !m_stackTracer ){
		m_stackTracer = new gtStackTrace( this );
	}
}

void gtMainSystemCommon::initEventSystem( void ){
	m_events		= gtPtrNew<gtEventSystem>( new gtEventSystem( m_params.m_consumer ) );
}



gtMainSystem*	gtMainSystem::getInstance( void ){
	return gtMainSystemCommon::getInstance();
}
			
	//	возвратит StackTracer
gtStackTrace*	gtMainSystemCommon::getStackTracer( void ){
	return m_stackTracer;
}

	//	gtStackTrace::dumpStackTrace
void gtStackTrace::dumpStackTrace( void ){
	gtMainSystemCommon::getInstance()->getStackTracer()->printStackTrace(2u,3u);
}

	//	Инициализирует видео драйвер
gtDriver* gtMainSystemCommon::createVideoDriver( gtPlugin* videoDriverPlugin, const gtDriverInfo& params ){

	GT_ASSERT2( videoDriverPlugin, "videoDriverPlugin was nullptr" );

	if( !videoDriverPlugin ) return nullptr;

	if( videoDriverPlugin->getInfo().m_info.m_type != gtPluginType::render ){

		gtLogWriter::printError( u"Can not create video driver" );

#ifdef GT_EDBUG
		gtStackTrace::dumpStackTrace();
#endif

		return nullptr;
	}

	gtPluginRender* plugin = (gtPluginRender*)videoDriverPlugin;

	return plugin->loadDriver( params );
}

	//	Выделяет память размером size. Для освобождения нужно вызвать freeMemory
	//	(void**)&i->data
bool gtMainSystemCommon::allocateMemory( void** data, u32 size ){
	GT_ASSERT1( !(*data), "Memory block is not free or pointer not set nullptr", "*data==nullptr" );
	*data = std::malloc( size );
	return (*data)?true:false;
}

	//	Освобождает память, выделенную с помощью allocateMemory
void gtMainSystemCommon::freeMemory( void** data ){
	GT_ASSERT1( *data, "Memory block is not allocated or set nullptr", "*data!=nullptr" );
	std::free( *data );
	*data = nullptr;
}

	//	Загрузит gtImage, если расширение поддерживается хоть каким-то плагином
gtImage*	gtMainSystemCommon::loadImage( const gtString& fileName ){
	gtPtr<gtImage> image = gtPtrNew<gtImage>( this->m_pluginSystem->importImage( fileName ) );
	if( !image.data() ) return nullptr;
	image->addRef();
	return image.data();
}

	//	Загрузит gtImage плагином имеющим указанный код
gtImage*	gtMainSystemCommon::loadImage( const gtString& fileName, const gtString& pluginGUID ){
	gtPtr<gtImage> image = gtPtrNew<gtImage>( this->m_pluginSystem->importImage( fileName, pluginGUID, true ) );
	if( !image.data() ) return nullptr;
	image->addRef();
	return image.data();
}

	//	Удаляет картинку из памяти
void		gtMainSystemCommon::removeImage( gtImage* image ){
	if( image ){
		image->release();
		image = nullptr;
	}
}

	//	добавить событие. inFront если вперёд.
void		gtMainSystemCommon::addEvent( const gtEvent& ev, u8 prior ){
	if( m_events.data() )
		m_events->addEvent( ev, prior );
}

gtModelSystem*	gtMainSystemCommon::getModelSystem( void ){
	return m_modelSystem.data();
}

gtPluginSystem*	gtMainSystemCommon::getPluginSystem( void ){
	return m_pluginSystem.data();
}

gtSceneSystem*	gtMainSystemCommon::getSceneSystem( void ){
	return m_sceneSystem.data();
}

const gtMatrix4& gtMainSystemCommon::getMatrixWorld( void ){
	return m_WVP[ 0u ];
}

const gtMatrix4& gtMainSystemCommon::getMatrixView( void ){
	return m_WVP[ 1u ];
}

const gtMatrix4& gtMainSystemCommon::getMatrixProjection( void ){
	return m_WVP[ 2u ];
}

void gtMainSystemCommon::setMatrixWorld( const gtMatrix4& m ){
	m_WVP[ 0u ] = m;
}

void gtMainSystemCommon::setMatrixView( const gtMatrix4& m ){
	m_WVP[ 1u ] = m;
}

void gtMainSystemCommon::setMatrixProjection( const gtMatrix4& m ){
	m_WVP[ 2u ] = m;
}

bool gtMainSystemCommon::pollEvent( gtEvent& event ){
	if( !m_isRun ) return false;
	return m_events->pollEvent( event );
}

bool gtMainSystemCommon::isKeyPressed( gtKey key ){
	return m_events->isKeyDown( key );
}

bool gtMainSystemCommon::isLMBDown( void ){
	return m_events->isLMBDown();
}

bool gtMainSystemCommon::isRMBDown( void ){
	return m_events->isRMBDown();
}

bool gtMainSystemCommon::isMMBDown( void ){
	return m_events->isMMBDown();
}

const gtVector<u16,2u>& gtMainSystemCommon::getCursorPosition( void ){
	return m_events->getCursorPosition();
}

const gtDeviceCreationParameters& gtMainSystemCommon::getDeviceCreationParameters( void ){
	return m_params;
}

bool gtMainSystemCommon::isRun( void ){
	return m_isRun;
}

gtPtrNew<gtAudioSystem> gtMainSystemCommon::createAudioSystem( const gtString& guid ){
	if( guid.size() ){
	}else{
	}

	return gtPtrNew<gtAudioSystem>( new gtAudioSystem );
}

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