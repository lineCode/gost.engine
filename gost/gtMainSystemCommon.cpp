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
gtDriver* gtMainSystemCommon::createVideoDriver( /*gtPlugin* videoDriverPlugin,*/ const gtDriverInfo& params, const gtString& uid ){

	//GT_ASSERT2( videoDriverPlugin, "videoDriverPlugin was nullptr" );

	//if( !videoDriverPlugin ) return nullptr;

	/*if( videoDriverPlugin->getInfo().m_info.m_type != gtPluginType::render ){

		gtLogWriter::printError( u"Can not create video driver" );

#ifdef GT_EDBUG
		gtStackTrace::dumpStackTrace();
#endif

		return nullptr;
	}

	gtPluginRender* plugin = (gtPluginRender*)videoDriverPlugin;*/

	auto plugin = this->getPluginSystem()->getPlugin( uid );

	if( plugin->getInfo().m_info.m_type != gtPluginType::render ){
		gtLogWriter::printError( u"Can not create video driver" );
		return nullptr;
	}

	return ((gtPluginRender*)plugin)->loadDriver( params );
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

gtSceneSystem*	gtMainSystemCommon::getSceneSystem( gtDriver * currentRenderDriver ){
	if( currentRenderDriver )
		m_sceneSystem->setCurrentRenderDriver( currentRenderDriver );
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

const gtVector2<u16>& gtMainSystemCommon::getCursorPosition( void ){
	return m_events->getCursorPosition();
}

const gtDeviceCreationParameters& gtMainSystemCommon::getDeviceCreationParameters( void ){
	return m_params;
}

bool gtMainSystemCommon::isRun( void ){
	return m_isRun;
}

gtAudioSystem* gtMainSystemCommon::createAudioSystem( const gtString& uid ){
	/*
	auto plugin = this->getPluginSystem()->getPlugin( uid );

	if( plugin->getInfo().m_info.m_type != gtPluginType::render ){
		gtLogWriter::printError( u"Can not create video driver" );
		return nullptr;
	}

	return ((gtPluginRender*)plugin)->loadDriver( params );
	*/
	
	gtPlugin * plugin = nullptr;
	gtPluginAudio * pluginAudio = nullptr;

	auto * ps = getPluginSystem();

	if( uid.size() )
		plugin = ps->getPlugin( uid );

	if( !plugin || !uid.size() ){
		u32 np = ps->getNumOfPlugins();

		for( u32 i = 0u; i < np; ++i ){

			auto * pl = ps->getPlugin( i );

			if( pl->getInfo().m_info.m_type == gtPluginType::audio ){
			
				pluginAudio = ps->getAsPluginAudio( pl );

				auto * ret = pluginAudio->loadAudioDriver();

				if( ret ) return ret;

			}
		}
	}

	pluginAudio = ps->getAsPluginAudio( plugin );
	return pluginAudio->loadAudioDriver();
}

gtXMLDocument* gtMainSystemCommon::XMLRead( const gtString& file ){

	if( !gtFileSystem::existFile( file ) ){
		gtLogWriter::printWarning( u"Can not read XML document. File not exist. [%s]", file.data() );
		return nullptr;
	}

	gtPtr_t( gtXMLDocumentImpl, xml, new gtXMLDocumentImpl(file) );

	if( !xml.data() ){
		gtLogWriter::printWarning( u"Can not create XML document." );
		return nullptr;
	}


	xml->addRef();
	return xml.data();
}

void writeText( gtString& outText, const gtString& inText ){
	u32 sz = inText.size();
	for( u32 i = 0u; i < sz; ++i ){
		if( inText[ i ] == u'\'' ){
			outText += u"&apos;";
		}else if( inText[ i ] == u'\"' ){
			outText += u"&quot;";
		}else if( inText[ i ] == u'<' ){
			outText += u"&lt;";
		}else if( inText[ i ] == u'>' ){
			outText += u"&gt;";
		}else if( inText[ i ] == u'&' ){
			outText += u"&amp;";
		}else{
			outText += inText[ i ];
		}
	}
}

void writeName( gtString& outText, const gtString& inText ){
	outText += "<";
	outText += inText;
}

bool writeNodes( gtString& outText, const gtXMLNode& node, u32 tabCount ){
	for( u32 i = 0u; i < tabCount; ++i ){
		outText += u"\t";
	}

	++tabCount;

	writeName( outText, node.name );
	
	u32 sz = node.attributeList.size();
	if( sz ){
		for( u32 i = 0u; i < sz; ++i ){
			outText += u" ";
			outText += node.attributeList[ i ].name;
			outText += u"=";
			outText += u"\"";
			writeText( outText, node.attributeList[ i ].value );
			outText += u"\"";
		}
	}

	if( !node.nodeList.size() && !node.text.size() ){
		outText += u"/>\r\n";
		return true;
	}else if( node.text.size() ){
		outText += u">";
		writeText( outText, node.text );
		outText += u"</";
		outText += node.name;
		outText += u">\r\n";
		return true;

	}else{
		outText += u">\r\n";
		sz = node.nodeList.size();
		for( u32 i = 0u; i < sz; ++i ){
			if( !writeNodes( outText, node.nodeList[ i ], tabCount ) ){
				for( u32 o = 0u; o < tabCount; ++o ){
					outText += u"\t";
				}
				outText += u"</";
				outText += node.nodeList[ i ].name;
				outText += u">\r\n";
			}
		}
	}

	--tabCount;

	return false;
}

void gtMainSystemCommon::XMLWrite( const gtString& file, const gtXMLNode& rootNode, bool utf8 ){

	gtString outText( u"<?xml version=\"1.0\"" );
	if( utf8 ) outText += " encoding=\"UTF-8\"";
	outText += " ?>\r\n";

	writeNodes( outText, rootNode, 0 );
	outText += u"</";
	outText += rootNode.name;
	outText += u">\r\n";

	gtFile_t out = util::createFileForWriteText( file );

	gtTextFileInfo ti;
	ti.m_hasBOM = true;
	if( utf8 ){
		ti.m_format = ti.utf_8;
		out->setTextFileInfo( ti );
		
		gtStringA mbstr;
		util::utf16_to_utf8( outText, mbstr );

		out->write( mbstr );

	}else{
		ti.m_endian = ti.little;
		ti.m_format = ti.utf_16;
		out->setTextFileInfo( ti );
		out->write( outText );
	}

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