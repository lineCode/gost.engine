//	GOST

#include "common.h"

gtPluginSystemImpl::gtPluginSystemImpl( void ):
	m_numOfPlugins( 0u )
{
#ifdef GT_DEBUG
	m_debugName.assign(u"gtPluginSystemImpl");
#endif
}


gtPluginSystemImpl::~gtPluginSystemImpl( void ){

}


void gtPluginSystemImpl::scanFolder( const gtString& dir ){
	gtArray<gtFileSystem::DirObject> objs;

	gtFileSystem::scanDirBegin( dir );
	gtFileSystem::DirObject dob;
	while( gtFileSystem::getDirObject( &dob ) ){
		objs.push_back( dob );
	}
	gtFileSystem::scanDirEnd();

	u32 sz = objs.size();

	for( u32 i = 0u; i < sz; ++i ){
		auto * o = &objs[ i ];
		if( o->type == gtFileSystem::DirObjectType::file ){
			gtString extension = util::stringGetExtension( gtString((char16_t*)o->path) );
			if( extension == u"gpl" || extension == u"gpld" ){
				 
#ifdef GT_DEBUG
				if( extension.size() == 3 ) continue;
#else
				if( extension.size() == 4 ) continue;
#endif


				gtPluginInfoDL pi_dl;

				GT_LIBRARY_HANDLE lib = GT_LOAD_LIBRARY(o->path);
				if( !lib ){
					gtLogWriter::printWarning( u"Can not load plugin [%s]", o->path );
					continue;
				}
				gtGetPluginInfo f_GetPluginInfo = GT_LOAD_FUNCTION_SAFE_CAST(gtGetPluginInfo,lib,"GetPluginInfo");
					/*reinterpret_cast<gtGetPluginInfo>(
						reinterpret_cast<void*>(
							GT_LOAD_FUNCTION( lib, "GetPluginInfo" )
						)
					);*/

				if( !f_GetPluginInfo ){
					gtLogWriter::printWarning( u"Can not get procedure address [%s] from plugin [%s]", u"GetPluginInfo", o->path );
					GT_FREE_LIBRARY( lib );
					continue;
				}

				gtPluginInfo pi;
				f_GetPluginInfo( pi );

				if( pi.m_type == gtPluginType::unknown ){
					gtLogWriter::printWarning( u"Unsupported plugin [%s]", o->path );
					GT_FREE_LIBRARY( lib );
					continue;
				}
				
				//	пока добавляется только общая информация
				pi_dl.m_path.assign((char16_t*)o->path);
				pi_dl.m_info = pi;
				pi_dl.m_handle = lib;
				
				
				if( pi.m_type == gtPluginType::render ){
					gtPtr_t(gtPluginRender,plugin, new gtPluginRender( &pi_dl ) );

					if( !plugin->checkLibraryFunctions() ) continue;

					m_renderPluginCache.push_back( plugin.data() );
					m_plugins.push_back( plugin.data() );

				}else if( pi.m_type == gtPluginType::import_image ){

					gtPtr_t(gtPluginImportImage,plugin,new gtPluginImportImage( &pi_dl ) );

					if( !plugin->checkLibraryFunctions())	continue;

					m_importImagePluginCache.push_back( plugin.data() );
					m_plugins.push_back( plugin.data() );

				}else if( pi.m_type == gtPluginType::import_model ){

					gtPtr_t(gtPluginImportModel,plugin,new gtPluginImportModel( &pi_dl ) );

					if( !plugin->checkLibraryFunctions())	continue;

					this->m_importModelPluginCache.push_back( plugin.data() );
				}else if( pi.m_type == gtPluginType::audio ){
					
					gtPtr_t(gtPluginAudio,plugin,new gtPluginAudio( &pi_dl ) );

					if( !plugin->checkLibraryFunctions() ) continue;

					m_audioPluginCache.push_back( plugin.data() );
					m_plugins.push_back( plugin.data() );

				}

				gtLogWriter::printInfo( u"Add plugin: %s", pi.m_name.data() );
				gtLogWriter::printInfo( u"Description: %s", pi.m_description.data() );

				m_numOfPlugins++;
				GT_FREE_LIBRARY( lib );

			}
		}
	}

	for( u32 i = 0u; i < sz; ++i ){
		auto * o = &objs[ i ];

		if( o->type == gtFileSystem::DirObjectType::folder ){
			scanFolder( (char16_t*)o->path );
		}
	}
}

bool gtPluginSystemImpl::init( void ){

	gtString pluginsDir( gtFileSystem::getProgramPath() + u"plugins/" );

	if( !gtFileSystem::existDir( pluginsDir ) ){
		gtLogWriter::printError( u"Plugins folder not found" );
		return false;
	}

	scanFolder( pluginsDir );

	return true;
}

	//	получить количество плагинов в папке plugins
u32	gtPluginSystemImpl::getNumOfPlugins( void ){
	return m_numOfPlugins;
}

gtPlugin*	gtPluginSystemImpl::getPlugin( const gtString& uid ){
	u32 sz = m_plugins.size();

	if( !sz ) return nullptr;

	for( u32 i = 0u; i < sz; ++i ){
		auto * o = m_plugins[ i ];
		if( o->getInfo().m_info.m_GUID == uid ){
			return o;
		}
	}
	return nullptr;
}

gtPlugin*	gtPluginSystemImpl::getPlugin( u32 id ){
	u32 sz = m_plugins.size();

	GT_ASSERT( id < sz, "Bad id(too hight)", "id < sz", id, sz );

	if( !sz ) return nullptr;
	if( id >= sz ) return nullptr;

	return m_plugins[ id ];
}

	//	загружает картинку
gtImage * gtPluginSystemImpl::importImage( const gtString& fileName, const gtString& guid, bool useguid ){

	if( !gtFileSystem::existFile( fileName ) ){
		gtLogWriter::printWarning( u"Can not load image [%s]. File not exist.", fileName.data() );
		return nullptr;
	}

	gtString file = fileName;
	util::stringFlipSlash( file );
	
	gtString ext = util::stringGetExtension( file );
	util::stringToLower( ext );
	
	gtImage * image = new gtImage;

	u32 sz = this->m_importImagePluginCache.size();
	for( u32 i = 0u; i < sz; ++i ){
		auto * o = &this->m_importImagePluginCache[ i ];

		if( useguid ){
			if( o->data()->getInfo().m_info.m_GUID == guid ){
				o->data()->load();
				o->data()->loadImage( &file, &image );
				break;
			}
		}else{
			u32 esz = o->data()->m_extensions.size();
			for( u32 j = 0u; j < esz; ++j ){
				if( o->data()->m_extensions[ j ] == ext ){
					o->data()->load();
					o->data()->loadImage( &file, &image );
					if( image->data ) break;
				}
			}
		}

	}

	if( image->data ){
		gtLogWriter::printInfo(u"Loading image: %s", fileName.data());
		return image;
	}

	delete image;
	return nullptr;
}

	//	загружает модель
gtModel * gtPluginSystemImpl::importModel( const gtString& fileName, const gtString& guid, bool useguid){

	if( !gtFileSystem::existFile( fileName ) ){
		gtLogWriter::printWarning( u"Can not load model [%s]. File not exist.", fileName.data() );
		return nullptr;
	}

	gtString file = fileName;
	util::stringFlipSlash( file );
	
	gtString ext = util::stringGetExtension( file );
	util::stringToLower( ext );
	
	gtModel * model = nullptr;//= gtMainSystem::getInstance()->getModelSystem()->createEmpty(;

	u32 sz = this->m_importModelPluginCache.size();
	for( u32 i = 0u; i < sz; ++i ){
		auto * o = &this->m_importModelPluginCache[ i ];

		if( useguid ){
			if( o->data()->getInfo().m_info.m_GUID == guid ){
				o->data()->load();
				model = o->data()->loadModel( &file );
				break;
			}
		}else{
			u32 esz = o->data()->m_extensions.size();
			for( u32 j = 0u; j < esz; ++j ){
				if( o->data()->m_extensions[ j ] == ext ){
					o->data()->load();
					model = o->data()->loadModel( &file );
					break;
				}
			}
		}

	}

	return model;
}

/*
Copyright (c) 2017 532235

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