//	GOST

#include "common.h"

gtPluginRender::gtPluginRender( gtPluginInfoDL* info ){
	m_info = *info;
#ifdef GT_DEBUG
	m_debugName.assign(u"gtPluginRender");
#endif
}

gtPluginRender::~gtPluginRender( void ){
	if( m_isLoad ) unload();
}


gtDriver * gtPluginRender::loadDriver( const gtDriverInfo& params ){
	
	if( !m_isLoad ) load();

	if( m_isLoad ){
		return loadDriverProc( params );
	}

	return nullptr;
}

const gtPluginInfoDL&	gtPluginRender::getInfo( void ){
	return m_info;
}

	//	проверит есть ли нужные функции в плагине
bool gtPluginRender::checkLibraryFunctions(){

	GT_LIBRARY_HANDLE lib = m_info.m_handle;

	gtLoadGPUDriver_t gtLoadGPUDriver = 	GT_LOAD_FUNCTION_SAFE_CAST(gtLoadGPUDriver_t, lib,"gtLoadGPUDriver");

	if( !gtLoadGPUDriver ){
		gtLogWriter::printWarning( u"Can not get procedure address [%s] from plugin [%s]", u"gtLoadGPUDriver", m_info.m_path.data() );
		GT_FREE_LIBRARY( lib );
		return false;
	}

	return true;
}

	///	загрузить плагин
void gtPluginRender::load( void ){

	if( !m_isLoad ){
	
		m_info.m_handle	=	GT_LOAD_LIBRARY( (wchar_t*)m_info.m_path.data() );

		if( !m_info.m_handle ){
			gtLogWriter::printWarning( u"Can not load plugin [%s]", m_info.m_path.data() );
			return;
		}

		loadDriverProc = GT_LOAD_FUNCTION_SAFE_CAST(gtLoadGPUDriver_t,m_info.m_handle, "gtLoadGPUDriver" );
		if( !loadDriverProc ){
			gtLogWriter::printWarning( u"Can not get procedure address [%s] from plugin [%s]", u"gtLoadGPUDriver", m_info.m_path.data() );
			GT_FREE_LIBRARY( m_info.m_handle );
			return;
		}

		m_isLoad = true;

	}

}

	//	выгрузить плагин
void gtPluginRender::unload( void ){
	if( m_isLoad ){
		if( m_info.m_handle )
			GT_FREE_LIBRARY( m_info.m_handle );
		m_info.m_handle = nullptr;
		m_isLoad = false;
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