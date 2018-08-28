//	GOST

#include "common.h"

gtPluginCommon::gtPluginCommon():
m_pl( nullptr )
{}

gtPluginCommon::~gtPluginCommon(){
	if( m_pl )
		delete m_pl;
}

// Загрузить и получить все нужные адреса
void gtPluginCommon::load(){
	if( !m_isLoad ){
		m_info.m_handle	=	GT_LOAD_LIBRARY( (wchar_t*)m_info.m_path.data() );
		if( !m_info.m_handle ){
			gtLogWriter::printWarning( u"Can not load plugin [%s]", m_info.m_path.data() );
			return;
		}
		
		auto sz = m_pl->getFunctionsCount();
		for( u32 i = 0u; i < sz; ++i ){
			void * ptr = (void*)GT_LOAD_FUNCTION( m_info.m_handle, m_pl->getFunctionName( i ).data() );
			if( !ptr ){
				gtLogWriter::printWarning( u"Can not get procedure address [%s] from plugin [%s]", m_pl->getFunctionName( i ).data(), m_info.m_path.data() );
				GT_FREE_LIBRARY( m_info.m_handle );
				return;
			}
			m_pl->setFunctionPtr( i, ptr );
		}
		m_isLoad = true;
	}
}

void gtPluginCommon::unload(){
	if( m_isLoad ){
		if( m_info.m_handle )
			GT_FREE_LIBRARY( m_info.m_handle );
		m_isLoad = false;
	}
}

bool gtPluginCommon::checkLibraryFunctions(){
	auto sz = m_pl->getFunctionsCount();
	for( u32 i = 0u; i < sz; ++i ){
		void * ptr = (void*)GT_LOAD_FUNCTION( m_info.m_handle, m_pl->getFunctionName( i ).data() );
		if( !ptr ){
			gtLogWriter::printWarning( u"Can not get procedure address [%s] from plugin [%s]", m_pl->getFunctionName( i ).data(), m_info.m_path.data() );
			GT_FREE_LIBRARY( m_info.m_handle );
			return false;
		}
	}
	return true;
}

//__________________________________________

gtPluginSystemImpl::gtPluginSystemImpl():
	m_numOfPlugins( 0u )
{}

gtPluginSystemImpl::~gtPluginSystemImpl(){}


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

				if( !f_GetPluginInfo ){
					gtLogWriter::printWarning( u"Can not get procedure address [%s] from plugin [%s]", u"GetPluginInfo", o->path );
					GT_FREE_LIBRARY( lib );
					continue;
				}

				gtPluginInfo pi;
				f_GetPluginInfo( &pi );

				if( pi.m_type == gtPluginType::Unknown ){
					gtLogWriter::printWarning( u"Unsupported plugin [%s]", o->path );
					GT_FREE_LIBRARY( lib );
					continue;
				}
				
				pi_dl.m_path.assign((char16_t*)o->path);
				pi_dl.m_info = pi;
				pi_dl.m_handle = lib;
				
				gtPtr_t( gtPluginCommon, plugin, new gtPluginCommon );
				
				plugin->setInfo( pi_dl );
				
				gtPluginImpl * pl_impl = nullptr;
				
				switch( pi.m_type ){
					case gtPluginType::Render:
						pl_impl = new gtPluginRender;
					break;
					case gtPluginType::Import_image:
						pl_impl = new gtPluginImportImage;
					break;
					case gtPluginType::Import_model:
						pl_impl = new gtPluginImportModel;
					break;
					case gtPluginType::Audio:
						pl_impl = new gtPluginAudio;
					break;
					case gtPluginType::Input:
						pl_impl = new gtPluginInput;
					break;
					case gtPluginType::Physics:
						pl_impl = new gtPluginPhysics;
					break;
					case gtPluginType::Script:
						pl_impl = new gtPluginScript;
					break;
					default:
						GT_FREE_LIBRARY( lib );
						gtLogWriter::printInfo( u"Unsupported plugin: %s. Ignore.", pi.m_name.data() );
						continue;
					break;
				}
				
				plugin->setImplementation( pl_impl );
				
				if( !plugin->checkLibraryFunctions() ){
					delete pl_impl;
					continue;
				}
				
				switch( pi.m_type ){
					case gtPluginType::Render:
						m_renderPluginCache.push_back( plugin.data() );
					break;
					case gtPluginType::Import_image:{
						gtPluginImportImage * impl = (gtPluginImportImage *)plugin->getImplementation();
					
						gtPluginGetExtCount_t f_PluginGetExtCount = GT_LOAD_FUNCTION_SAFE_CAST(gtPluginGetExtCount_t,lib,"PluginGetExtCount");
						gtPluginGetExtension_t f_gtPluginGetExtension = GT_LOAD_FUNCTION_SAFE_CAST(gtPluginGetExtension_t,lib,"PluginGetExtension");
						
						u32 ecnt = f_PluginGetExtCount();
						for( u32 j = 0u; j < ecnt; ++j ){
							gtString str;
							str.assign( f_gtPluginGetExtension( j ) );
							impl->m_extensions.push_back( str );
						}
						
						m_importImagePluginCache.push_back( plugin.data() );
					}break;
					case gtPluginType::Import_model:{
						gtPluginImportModel * impl = (gtPluginImportModel *)plugin->getImplementation();
					
						gtPluginGetExtCount_t f_PluginGetExtCount = GT_LOAD_FUNCTION_SAFE_CAST(gtPluginGetExtCount_t,lib,"PluginGetExtCount");
						gtPluginGetExtension_t f_gtPluginGetExtension = GT_LOAD_FUNCTION_SAFE_CAST(gtPluginGetExtension_t,lib,"PluginGetExtension");
						
						u32 ecnt = f_PluginGetExtCount();
						for( u32 j = 0u; j < ecnt; ++j ){
							gtString str;
							str.assign( f_gtPluginGetExtension( j ) );
							impl->m_extensions.push_back( str );
						}
						
						this->m_importModelPluginCache.push_back( plugin.data() );
					}break;
					case gtPluginType::Audio:
						m_audioPluginCache.push_back( plugin.data() );
					break;
					case gtPluginType::Input:
						m_inputPluginCache.push_back( plugin.data() );
					break;
					case gtPluginType::Physics:
						m_physicsPluginCache.push_back( plugin.data() );
					break;
					case gtPluginType::Script:
						m_scriptPluginCache.push_back( plugin.data() );
					break;
					default:
					break;
				}
				
				m_plugins.push_back( plugin.data() );

				gtLogWriter::printInfo( u"Add plugin: %s", pi.m_name.data() );
				GT_FREE_LIBRARY( lib );
				m_numOfPlugins++;

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

bool gtPluginSystemImpl::init(){

	gtString pluginsDir( gtFileSystem::getProgramPath() + u"plugins/" );

	if( !gtFileSystem::existDir( pluginsDir ) ){
		gtLogWriter::printWarning( u"Plugins folder not found" );
		gtFileSystem::createDir( gtFileSystem::getProgramPath() + u"plugins/" );
		return true;
	}

	scanFolder( pluginsDir );

	return true;
}

u32	gtPluginSystemImpl::getNumOfPlugins(){
	return m_numOfPlugins;
}

gtPlugin*	gtPluginSystemImpl::getPlugin( const GT_GUID& uid ){
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

gtImage * gtPluginSystemImpl::importImage( const gtString& fileName, const GT_GUID& guid, bool useguid ){

	gtString realPath;
	
	if( !gtFileSystem::existFile( fileName ) ){
		realPath = gtFileSystem::getRealPath( fileName );
	}else{
		realPath = fileName;
	}

	if( !gtFileSystem::existFile( realPath ) ){
		gtLogWriter::printWarning( u"Can not load image [%s]. File not exist.", realPath.data() );
		return nullptr;
	}

	gtString file = realPath;
	util::stringFlipSlash( file );
	
	gtString ext = util::stringGetExtension( file );
	util::stringToLower( ext );
	
	gtImage * image = new gtImage;
	u32 sz = this->m_importImagePluginCache.size();
	for( u32 i = 0u; i < sz; ++i ){
		auto * o = &this->m_importImagePluginCache[ i ];
		gtPluginImportImage * iip = (gtPluginImportImage *)o->data()->getImplementation();
		if( useguid ){
			if( o->data()->getInfo().m_info.m_GUID == guid ){
				o->data()->load();
				iip->loadImage( &file, &image );
				break;
			}
		}else{
			u32 esz = iip->m_extensions.size();
			
			for( u32 j = 0u; j < esz; ++j ){
				if( iip->m_extensions[ j ] == ext ){
					o->data()->load();
					iip->loadImage( &file, &image );
					if( image->data ) break;
				}
			}
		}

	}

	if( image->data ){
		gtLogWriter::printInfo(u"Loading image: %s", realPath.data());
		return image;
	}

	delete image;
	return nullptr;
}

gtModel * gtPluginSystemImpl::importModel( const gtString& fileName, const GT_GUID& guid, bool useguid){

	gtString realPath = gtFileSystem::getRealPath( fileName );

	if( !gtFileSystem::existFile( realPath ) ){
		gtLogWriter::printWarning( u"Can not load model [%s]. File not exist.", realPath.data() );
		return nullptr;
	}

	gtString file = realPath;
	util::stringFlipSlash( file );
	
	gtString ext = util::stringGetExtension( file );
	util::stringToLower( ext );
	
	gtModel * model = nullptr;//= gtMainSystem::getInstance()->getModelSystem()->createEmpty(;

	u32 sz = this->m_importModelPluginCache.size();
	for( u32 i = 0u; i < sz; ++i ){
		auto * o = &this->m_importModelPluginCache[ i ];
		gtPluginImportModel * iim = (gtPluginImportModel *)o->data()->getImplementation();

		if( useguid ){
			if( o->data()->getInfo().m_info.m_GUID == guid ){
				o->data()->load();
				return iim->loadModel( &file );
			}
		}else{
			u32 esz = iim->m_extensions.size();
			gtLogWriter::printWarning( u"iim->m_extensions.size() %u", esz );
			for( u32 j = 0u; j < esz; ++j ){
				if( iim->m_extensions[ j ] == ext ){
					o->data()->load();
					return iim->loadModel( &file );
				}
			}
		}

	}

	return model;
}

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