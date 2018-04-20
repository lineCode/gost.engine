//GoST

#include "common.h"

#ifdef _DEBUG
#pragma comment(lib, "gost_d.lib")
#else 
#pragma comment(lib, "gost.lib")
#endif

extern "C"{
	__declspec(dllexport) void	GetPluginInfo( gtPluginInfo& info ){
		info.m_author.assign( u"532235" );
		info.m_description.assign( u"Plugin for using gamepads." );
		info.m_GUID = GT_UID_INPUT_DINPUT;
		info.m_name.assign( u"DirectInput" );
		info.m_type = gtPluginType::input;
		info.m_version = 1;
		info.m_build = 1;
	}
	__declspec(dllexport) gtGameController * gtLoadInputDriver( gtDriverInfo params ){
		gtLogWriter::printInfo( u"Init DirectInput..." );
		gtPtr<gtGameControllerImpl>	 driver = gtPtrNew<gtGameControllerImpl>(new gtGameControllerImpl());
		
		if( driver.data() ){
			
			if( driver->init()){
				driver->addRef();
			}else{
				driver->release();
				gtLogWriter::printError( u"Can not initialize D3D11 driver." );
				return nullptr;
			}
		}
		return driver.data();
	}
}

namespace gost{

	gtGameControllerImpl::gtGameControllerImpl( void ):
		m_mainSystem( gtMainSystem::getInstance() ),
		m_directInput( nullptr ),
		m_gamepad( nullptr )
	{
		m_type = gtGameControllerType::Gamepad;
	}
	gtGameControllerImpl::~gtGameControllerImpl( void ){
		if( m_directInput )
			m_directInput->Release();
	}

	bool gtGameControllerImpl::init( void ){
		HRESULT hr;
		if( FAILED( hr = DirectInput8Create( GetModuleHandle( NULL ), DIRECTINPUT_VERSION,
			IID_IDirectInput8, ( VOID** )&m_directInput, NULL ) ) ){
			gtLogWriter::printWarning( u"Can not initialize DirectInput plugin" );
			return false;
		}

		return true;
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