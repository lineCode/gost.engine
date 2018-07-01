#pragma once
#ifndef __GOST_H__
#define __GOST_H__

#if defined(_MSC_VER)
#pragma warning(disable : 4582)
#endif


//C++
#include <cstdlib>
#include <cctype> //isalpha
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <typeinfo>
#include <cmath>
#include <memory>

//	GOST
#include <gtInfo.h>
#include <gtTypes.h>
#include <gtGUID.h>

#include <gtPair.h>
#include <gtAllocator.h>
#include <gtArray.h>
#include <gtList.h>
#include <gtPriorQueue.h>
#include <gtString.h>

#include <gtBaseObject.h>
#include <gtRefObject.h>
#include <gtPtr.h>

#include <gtLog.h>

#include <gtError.h>

#include <gtInput.h>
#include <gtEvent.h>

#include <gtXMLDocument.h>

#include <gtMath.h>
#include <gtVector.h>
#include <gtQuaternion.h>
#include <gtMatrix.h>
#include <gtColor.h>

#include <gtAnimation.h>

#include <gtVertex.h>

#include <gtAABB.h>
#include <gtOBB.h>

#include <gtThread.h>
#include <gtMutex.h>

#include <gtGUIObject.h>


#include <gtSceneSystem.h>
#include <gtAudioSystem.h>
#include <gtGameController.h>
#include <gtGUISystem.h>

#include <gtMainSystem.h>


#include <gtImage.h>
#include <gtTexture.h>
#include <gtShader.h>
#include <gtMaterial.h>


#ifdef GT_DEBUG
#if defined( GT_PLATFORM_WIN32 )
#define GT_USE_STACK_TRACE
#endif
#endif
#include <gtStackTracer.h>


#include <gtSubModel.h>
#include <gtModel.h>
#include <gtRenderModel.h>
#include <gtModelSystem.h>


#include <gtTimer.h>

#include <gtFileSystem.h>

#include <gtWindowInfo.h>

#include <gtDriver.h>

#include <gtGameObject.h>

#include <gtPluginSystem.h>

#include <gtWindow.h>

#include <gtOutputWindow.h>

#include <gtUtilities.h>


#if defined(_MSC_VER)
#ifndef GOSTDLL
#ifdef GT_DEBUG
#pragma comment(lib, "gost_d.lib")
#else
#pragma comment(lib, "gost.lib")
#endif
#endif
#endif

namespace gost {

	extern "C" GT_API gtMainSystem* GT_CDECL InitializeGoSTEngine_internal( const gtDeviceCreationParameters& params );

	GT_FORCE_INLINE gtPtr<gtMainSystem> InitializeGoSTEngine( const gtDeviceCreationParameters& params = gtDeviceCreationParameters() ){
		return gtPtrNew<gtMainSystem>( InitializeGoSTEngine_internal( params ) );
	}


}


#endif

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
