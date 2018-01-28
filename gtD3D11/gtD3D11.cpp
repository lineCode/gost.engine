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
		info.m_description.assign( u"Direct3D 11 renderer" );
		info.m_GUID.assign( GT_UID_RENDER_D3D11 );
		info.m_name.assign( u"Direct3D 11 renderer" );
		info.m_type = gtPluginType::render;
		info.m_version = 1;
		info.m_build = 18;
	}

	__declspec(dllexport) gtDriver * gtLoadGPUDriver( /*gtMainSystem* System, */gtDriverInfo params ){

		gtLogWriter::printInfo( u"Init D3D11 driver..." );

		gtPtr<gtDriverD3D11>	 driver = gtPtrNew<gtDriverD3D11>(new gtDriverD3D11( /*System, */params ));
		
		if( driver.data() ){
			
			if( driver->initialize()){
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