#define GT_LUA

#include "common.h"

void * gtLua_Alloc( void *ud,
	void * ptr,
	std::size_t osize,
	std::size_t nsize ){
	return nullptr;
}

gtScriptLua::gtScriptLua():
	m_lua( nullptr )
{
	m_lua = luaL_newstate();
	luaopen_base( m_lua );
	luaopen_table( m_lua );
	luaopen_io( m_lua );
	luaopen_string( m_lua );
	luaopen_math( m_lua );
}

gtScriptLua::~gtScriptLua(){
	if( m_lua )
		lua_close( m_lua );
}
	
void gtScriptLua::addCFunction( const gtStringA& name, void * ptr ){
	
}

void gtScriptLua::callSFunction( const gtStringA& name ){
	
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