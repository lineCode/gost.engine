//	GOST

#include "common.h"
/*
template<class gtPluginImportModel_t>
void gtPluginImpl<gtPluginImportModel_t>::load(){
	if( !m_isLoad ){

		m_info.m_handle = GT_LOAD_LIBRARY((wchar_t*)m_info.m_path.data());

		if( !m_info.m_handle ){
			gtLogWriter::printWarning( u"Can not load plugin [%s]", m_info.m_path.data() );
			return;
		}

		//gtPluginLoadModel_t loadProc = (gtPluginLoadModel_t)GT_LOAD_FUNCTION( m_info.m_handle, "PluginLoadModel" );
		gtPluginLoadModel_t loadProc = GT_LOAD_FUNCTION_SAFE_CAST(gtPluginLoadModel_t,m_info.m_handle, "PluginLoadModel" );
		if( !loadProc ){
			gtLogWriter::printWarning( u"Can not get procedure address [%s] from plugin [%s]", u"PluginLoadModel", m_info.m_path.data() );
			GT_FREE_LIBRARY( m_info.m_handle );
			return;
		}

		m_pl.f_loadModel = loadProc;

		m_isLoad = true;
	}
}

template<class gtPluginImportModel_t>
void gtPluginImpl<gtPluginImportModel_t>::unload(){
	if( m_isLoad ){

		if( m_info.m_handle )
			GT_FREE_LIBRARY( m_info.m_handle );

		m_isLoad = false;
	}
}

template<class gtPluginImportModel_t>
const gtPluginInfoDL&	gtPluginImpl<gtPluginImportModel_t>::getInfo(){
	return m_info;
}

template<class gtPluginImportModel_t>
bool gtPluginImpl<gtPluginImportModel_t>::checkLibraryFunctions(){

	GT_LIBRARY_HANDLE lib = m_info.m_handle;

	gtPluginGetExtCount_t gtPluginGetExtCount = GT_LOAD_FUNCTION_SAFE_CAST(gtPluginGetExtCount_t,lib,"PluginGetExtCount");
	if( !gtPluginGetExtCount ){
		gtLogWriter::printWarning( u"Can not get procedure address [%s] from plugin [%s]", u"PluginGetExtCount", m_info.m_path.data() );
		GT_FREE_LIBRARY( lib );
		return false;
	}

	gtPluginGetExtension_t gtPluginGetExtension = GT_LOAD_FUNCTION_SAFE_CAST(gtPluginGetExtension_t,lib,"PluginGetExtension");
	if( !gtPluginGetExtension ){
		gtLogWriter::printWarning( u"Can not get procedure address [%s] from plugin [%s]", u"PluginGetExtension", m_info.m_path.data() );
		GT_FREE_LIBRARY( lib );
		return false;
	}

	gtPluginLoadModel_t gtPluginLoadModel = GT_LOAD_FUNCTION_SAFE_CAST(gtPluginLoadModel_t,lib,"PluginLoadModel");
	if( !gtPluginLoadModel ){
		gtLogWriter::printWarning( u"Can not get procedure address [%s] from plugin [%s]", u"PluginLoadModel", m_info.m_path.data() );
		GT_FREE_LIBRARY( lib );
		return false;
	}

	u32 sz = gtPluginGetExtCount();
	if( sz ){
		for( u32 i = 0u; i < sz; ++i ){
			const s8 * str = gtPluginGetExtension( i );
			gtString ext;
			util::stringAppend( ext, str, u'a' );
			util::stringToLower( ext );
			m_pl.m_extensions.push_back( ext );
		}
		return true;
	}
	return false;
}*/

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