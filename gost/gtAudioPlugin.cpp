//	GOST

#include "common.h"

gtPluginAudio::gtPluginAudio( gtPluginInfoDL* info ){
	m_info = *info;
}

gtPluginAudio::~gtPluginAudio( void ){
	if( m_isLoad ) unload();
}

const gtPluginInfoDL&	gtPluginAudio::getInfo( void ){
	return m_info;
}



bool gtPluginAudio::checkLibraryFunctions(){

	GT_LIBRARY_HANDLE lib = m_info.m_handle;

	gtLoadAudioDriver_t loadAudioDriverProc1 = 	GT_LOAD_FUNCTION_SAFE_CAST(gtLoadAudioDriver_t, lib,"gtLoadAudioPlugin");

	if( !loadAudioDriverProc1 ){
		gtLogWriter::printWarning( u"Can not get procedure address [%s] from plugin [%s]", u"gtLoadAudioPlugin", m_info.m_path.data() );
		GT_FREE_LIBRARY( lib );
		return false;
	}

	return true;
}

gtAudioSystem* gtPluginAudio::loadAudioDriver(){
	if( !m_isLoad ) load();

	if( m_isLoad ){
		return loadAudioDriverProc();
	}

	return nullptr;
}

void gtPluginAudio::load( void ){

	if( !m_isLoad ){

		m_info.m_handle	=	GT_LOAD_LIBRARY( (wchar_t*)m_info.m_path.data() );

		if( !m_info.m_handle ){
			gtLogWriter::printWarning( u"Can not load plugin [%s]", m_info.m_path.data() );
			return;
		}

		loadAudioDriverProc = GT_LOAD_FUNCTION_SAFE_CAST(gtLoadAudioDriver_t,m_info.m_handle, "gtLoadAudioPlugin" );
		if( !loadAudioDriverProc ){
			gtLogWriter::printWarning( u"Can not get procedure address [%s] from plugin [%s]", u"gtLoadAudioPlugin", m_info.m_path.data() );
			GT_FREE_LIBRARY( m_info.m_handle );
			return;
		}

		m_isLoad = true;

	}

}

void gtPluginAudio::unload( void ){
	if( m_isLoad ){
		if( m_info.m_handle )
			GT_FREE_LIBRARY( m_info.m_handle );
		m_info.m_handle = nullptr;
		m_isLoad = false;
	}
}


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
