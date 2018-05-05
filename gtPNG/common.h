#pragma once

#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#pragma warning(disable : 4820)
#pragma warning(disable : 4002)	//	to many actual parameters for macro
#pragma warning(disable : 4061)	//	Warning	C4061	enumerator 'D3D_SRV_DIMENSION_UNKNOWN' in switch of enum 'D3D_SRV_DIMENSION' is not explicitly handled by a case label

#pragma warning(disable : 4091) // dbghelp.h 
#pragma warning(disable : 4365) // Warning	C4365	'=': conversion from 'LONG' to 'UINT', signed/unsigned mismatch	gtD3D11	c:\program files (x86)\windows kits\8.1\include\um\d3d11.h

#pragma warning(disable : 4668)
#pragma warning(disable : 4710)
#pragma warning(disable : 4777) // _snprintf %i
#pragma warning(disable : 4477) // _snprintf %#010x
#pragma warning(disable : 4826)	//	conversion from 'unsigned long *' to DWORD64 is sign-extended
#pragma warning(disable : 4917)
#include <windows.h>


#include "gost.h"
using namespace gost;

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