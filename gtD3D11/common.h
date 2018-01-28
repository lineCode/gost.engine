#pragma once

#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#pragma warning(disable : 4002)	//	to many actual parameters for macro
#pragma warning(disable : 4061)	//	Warning	C4061	enumerator 'D3D_SRV_DIMENSION_UNKNOWN' in switch of enum 'D3D_SRV_DIMENSION' is not explicitly handled by a case label

#pragma warning(disable : 4091) // dbghelp.h 
#pragma warning(disable : 4365) // Warning	C4365	'=': conversion from 'LONG' to 'UINT', signed/unsigned mismatch	gtD3D11	c:\program files (x86)\windows kits\8.1\include\um\d3d11.h

#pragma warning(disable : 4668)
#pragma warning(disable : 4710)
#pragma warning(disable : 4777) // _snprintf %i
#pragma warning(disable : 4477) // _snprintf %#010x
#pragma warning(disable : 4820)
#pragma warning(disable : 4826)	//	conversion from 'unsigned long *' to DWORD64 is sign-extended
#pragma warning(disable : 4917)
#include <windows.h>

#include <d3d11.h>

#include <memory>

#include "gost.h"
using namespace gost;

#include "gtShaderImpl.h"
#include "gtShaderProcessingD3D11.h"

#include "gtTextureD3D11.h"

#include "gtRenderModelD3D11.h"

#include "gtDriverD3D11.h"

