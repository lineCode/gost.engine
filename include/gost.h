
#pragma once
#ifndef __GOST_H__
#define __GOST_H__ //< include guard

#if defined(_MSC_VER)
#pragma warning(disable : 4582)
#endif


//C++
#include <cctype> //isalpha
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <typeinfo>
#include <cmath>

//	GOST
#include <gtInfo.h>
#include <gtTypes.h>


#include <gtAllocator.h>
#include <gtArray.h>
#include <gtList.h>
#include <gtPriorQueue.h>
#include <gtString.h>

#include <gtBaseObject.h>
#include <gtRefObject.h>
#include <gtPtr.h>

#include <gtLoger.h>

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
#include <gtStackTrace.h>


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

#if defined(GT_PLATFORM_WIN32)
#ifndef GOSTDLL
#include <Windows.h>
#ifdef GT_DEBUG
#pragma comment(lib, "gost_d.lib")
#else
#pragma comment(lib, "gost.lib")
#endif
#endif
#endif

//	Base namespace for GoST
//	All GoST objects here
namespace gost {

/*
	GoST initialization

	\param params: gtDeviceCreationParameters
	\return gtMainSystem
*/

	extern "C" GT_API gtMainSystem* GT_CDECL InitializeGoSTEngine_internal( const gtDeviceCreationParameters& params );

	GT_FORCE_INLINE gtPtr<gtMainSystem> InitializeGoSTEngine( const gtDeviceCreationParameters& params = gtDeviceCreationParameters() ){
		return gtPtrNew<gtMainSystem>( InitializeGoSTEngine_internal( params ) );
	}


}


#endif