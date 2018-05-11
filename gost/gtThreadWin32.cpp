#include "common.h"

#if defined(GT_PLATFORM_WIN32)
#include <process.h>

#if defined(GT_COMPILER_GCC)
typedef unsigned (__stdcall* _beginthreadex_proc_type)(void*);
#endif

gtThreadWin32::gtThreadWin32():
	m_handle( nullptr )
{}

gtThreadWin32::~gtThreadWin32(){
	join();
}

bool gtThreadWin32::start( StartFunction f, void* args, u32 stackSize ){

	m_handle = (HANDLE)_beginthreadex( NULL, stackSize, (_beginthreadex_proc_type)f, args, 0, &m_id );

	if( !m_handle ) return false;

	m_status = gtThreadStatus::running;

	return true;
}

void gtThreadWin32::join(){

	if( m_handle ){

		WaitForSingleObject( m_handle, INFINITE );

		m_status = gtThreadStatus::terminated;


		CloseHandle( m_handle );
		m_handle = nullptr;

	}
}

void * gtThreadWin32::getHandle(){
	return (void*)m_handle;
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
