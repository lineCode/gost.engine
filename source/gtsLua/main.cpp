#include "common.h"

#ifdef GT_PLATFORM_WIN32
BOOL APIENTRY DllMain( HMODULE, DWORD  ul_reason_for_call, LPVOID ){
	switch( ul_reason_for_call ){
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
#endif

extern "C"{

	__declspec(dllexport) void __cdecl GetPluginInfo( gtPluginInfo* info ){
		info->m_author.assign( u"532235" );
		info->m_description.assign( u"Lua for gost.engine" );
		info->m_GUID = GT_UID_SCRIPT_LUA_5_3_4;
		info->m_name.assign( u"Lua plugin" );
		info->m_type = gtPluginType::Script;
		info->m_version = 1;
		info->m_build = 1;
	}
	
	__declspec(dllexport) gtScriptSystem * gtLoadScriptPlugin( /*params*/ ){
		gtPtr<gtScriptLua>	 lua = gtPtrNew<gtScriptLua>(new gtScriptLua());
		if( lua )
			lua->addRef();
		return lua.data();
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