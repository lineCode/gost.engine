#include "common.h"

#ifdef _MSC_VER
#ifdef _DEBUG
#pragma comment(lib, "gost_d.lib")
#else 
#pragma comment(lib, "gost.lib")
#endif
#endif

extern "C"{
	__declspec(dllexport) void	GetPluginInfo( gtPluginInfo* info ){
		info->m_author.assign( u"532235" );
		info->m_description.assign( u"Bullet Physics plugin" );
		info->m_GUID = GT_UID_PHYSICS_BULLET_3_2_87;
		info->m_name.assign( u"Bullet Physics 2.87" );
		info->m_type = gtPluginType::Physics;
		info->m_version = 1;
		info->m_build = 1;
	}

	__declspec(dllexport) gtPhysicsSystem * gtLoadPhysicsPlugin( gtPhysicsSystemInfo* params ){
		gtPtr<gtPhysicsBullet>	 driver = gtPtrNew<gtPhysicsBullet>(new gtPhysicsBullet( params ));
		if( driver )
			driver->addRef();
		return driver.data();
	}
}

#if defined(GT_PLATFORM_WIN32)

BOOL __cdecl DllMain( HMODULE /*hModule*/,
                       DWORD  ul_reason_for_call,
                       LPVOID /*lpReserved*/
					 ){
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:{
	}break;
	}
	return TRUE;
}
#else
int main(){
return 0;
}
#endif


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