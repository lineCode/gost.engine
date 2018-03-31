#pragma once
#ifndef __GOST_H__
#define __GOST_H__ ///< include guard

#pragma warning(disable : 4582)

//C++
#include <cctype> //isalpha
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <typeinfo>
#include <codecvt>

//	GOST 
#include <gtInfo.h>
#include <gtTypes.h>


#include <gtError.h>

#include <gtAllocator.h>
#include <gtArray.h>
#include <gtList.h>
#include <gtPriorQueue.h>
#include <gtString.h>

#include <gtBaseObject.h>
#include <gtRefObject.h>
#include <gtPtr.h>

#include <gtLoger.h>


#include <gtInput.h>
#include <gtEvent.h>





#include <gtXMLDocument.h>

#include <gtVector.h>
#include <gtMath.h>
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
#include <gtGUISystem.h>

#include <gtMainSystem.h>


#include <gtImage.h>
#include <gtTexture.h>
#include <gtShader.h>
#include <gtMaterial.h>


#ifdef GT_DEBUG
#define GT_USE_STACK_TRACE
#endif 
#include <gtStackTrace.h>


#include <gtSubModel.h>
#include <gtModel.h>
#include <gtRenderModel.h>
#include <gtModelSystem.h>


#include <gtTimer.h>

#include <gtFileSystem.h>

#include <gtDriver.h>

#include <gtGameObject.h>

#include <gtPluginSystem.h>

#include <gtWindow.h>

#include <gtOutputWindow.h>

#include <gtUtilities.h>

//	Base namespace for GoST
//	All GoST objects here
namespace gost {

/*
	GoST initialization

	\param params: gtDeviceCreationParameters
	\return gtMainSystem
*/

extern "C" GT_API gtMainSystem* GT_CDECL InitializeGoSTEngine( const gtDeviceCreationParameters& params );


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
