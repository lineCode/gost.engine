//GoST

#include "common.h"

#ifdef _MSC_VER
#ifdef _DEBUG
#pragma comment(lib, "gost_d.lib")
#else 
#pragma comment(lib, "gost.lib")
#endif
#endif

XAudioVersion getXAudioVersion(){
		
		gtString systemPath = gtFileSystem::getSystemPath();
		
		s32 number = 9;

		XAudioVersion version = XAudioVersion::XAudio_2_9;

		for(;;){

			gtString path(systemPath);
			path += "XAudio2_";
			path += number;
			path += ".dll";

			if( gtFileSystem::existFile( path ) )
				return version;
			else{
				
				--number;

				if( number == -1 ){
					return XAudioVersion::none;
				}

				version = (XAudioVersion)((u32)version + 1u);
			}

		}

	}
	
extern "C"{


	__declspec(dllexport) void	GetPluginInfo( gtPluginInfo* info ){
		info->m_author.assign( u"532235" );
		info->m_description.assign( u"XAduio2 audio plugin" );
		info->m_GUID=GT_UID_AUDIO_XADUDIO2;
		info->m_name.assign( u"XAduio2" );
		info->m_type = gtPluginType::Audio;
		info->m_version = 1;
		info->m_build = 3;
	}

	__declspec(dllexport) gtAudioSystem * gtLoadAudioPlugin(){
		XAudioVersion version = getXAudioVersion();

		if( version > XAudioVersion::XAudio_2_7 )
			version = XAudioVersion::XAudio_2_7;

		switch( version ){
			case XAudioVersion::XAudio_2_9:
			break;
			case XAudioVersion::XAudio_2_8:
			break;
			case XAudioVersion::XAudio_2_7:{ // XAudio2 from DirectX SDK
				gtPtr_t(gtAudioSystemXAudio2_7,driver,new gtAudioSystemXAudio2_7());
				if( driver.data() ){

					if( driver->initialize( version )){
						driver->addRef();
					}else{
						driver->release();
						gtLogWriter::printError( u"Can not initialize XAudio2 plugin." );
						return nullptr;
					}
				}

				return driver.data();
			}break;
			case XAudioVersion::XAudio_2_6:
			break;
			case XAudioVersion::XAudio_2_5:
			break;
			case XAudioVersion::XAudio_2_4:
			break;
			case XAudioVersion::XAudio_2_3:
			break;
			case XAudioVersion::XAudio_2_2:
			break;
			case XAudioVersion::XAudio_2_1:
			break;
			case XAudioVersion::XAudio_2_0:
			break;
			case XAudioVersion::none:
				gtLogWriter::printError( u"XAudio2 not installed on this computer." );
				return nullptr;
			break;
		}

		
		return nullptr;
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