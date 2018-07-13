//	GOST
#include "common.h"


extern "C"{

	GT_API void GT_CDECL gost_init(){
		#if defined( GT_PLATFORM_WIN32 )
		if( !gtSingletone<gtMemorySystemCommon>::s_instance )
			gtSingletone<gtMemorySystemCommon>::s_instance = new gtMemorySystemImplWin32;

		if( !gtSingletone<gtFileSystemCommon>::s_instance )
			gtSingletone<gtFileSystemCommon>::s_instance = new gtFileSystemWin32;

		#endif
	}

	GT_API void GT_CDECL gost_clear(){
		if( gtSingletone<gtMemorySystemCommon>::s_instance ){
			delete gtSingletone<gtMemorySystemCommon>::s_instance;
			gtSingletone<gtMemorySystemCommon>::s_instance = nullptr;
		}
		if( gtSingletone<gtFileSystemCommon>::s_instance ){
			delete gtSingletone<gtFileSystemCommon>::s_instance;
			gtSingletone<gtFileSystemCommon>::s_instance = nullptr;
		}
	}

	GT_API gtMainSystem* gost_create_main_system_internal( const gtDeviceCreationParameters& params ){
		
		gtMainSystem*	main_system( nullptr );

		switch( params.m_device_type ){
			case gtDeviceType::Windows:{
	#if defined( GT_PLATFORM_WIN32 )
				main_system = new gtMainSystemWin32( params );

				if( !((gtMainSystemWin32*)main_system)->init() ){

					MessageBox( 0, L"Can not initialize GoST", L"Error", MB_OK | MB_ICONERROR );

					delete main_system;
					main_system = nullptr;
				}
	#endif
			}break;

			case gtDeviceType::Android:
			case gtDeviceType::iOS:
			case gtDeviceType::Linux:
			case gtDeviceType::OSX:
			case gtDeviceType::PlayStation:
			case gtDeviceType::Wii:
			case gtDeviceType::XBox:
			default:
				return nullptr;
				break;
		}

		return main_system;
	}

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
