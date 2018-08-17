#pragma once
#ifndef __GOST_H__
#define __GOST_H__

#if defined(_MSC_VER)
#pragma warning(disable : 4582)
#endif


//C++
#include <new>    //нужно потихоньку избавиться от C++ вещей
#include <sstream>
#include <string>
#include <vector>
#include <typeinfo>
#include <cmath>
//#include <memory>

//	GOST
#include <Common/gtInfo.h>
#include <Common/gtTypes.h>
#include <Classes/Common/gtSingleton.h>
#include <MemorySystem/gtMemorySystem.h>
#include <Classes/Common/gtGUID.h>
#include <Classes/Containers/gtPair.h>
#include <Classes/Common/gtAllocator.h>
#include <Classes/Containers/gtArray.h>
#include <Classes/Containers/gtList.h>
#include <Classes/Containers/gtPriorQueue.h>
#include <Classes/Containers/gtString.h>

#include <Classes/Common/gtBaseObject.h>
#include <Classes/Common/gtRefObject.h>
#include <Classes/Common/gtPtr.h>

#include <Common/gtLog.h>

#include <Common/gtError.h>

#include <InputSystem/gtInput.h>
#include <MainSystem/gtEvent.h>

#include <FileSystem/gtXMLDocument.h>

#include <Common/gtMath.h>
#include <Classes/Math/gtVector.h>
#include <Classes/Math/gtRay.h>
#include <Classes/Math/gtQuaternion.h>
#include <Classes/Math/gtMatrix.h>

#include <Classes/gtColor.h>
#include <Classes/gtAnimation.h>
#include <Classes/gtVertex.h>
#include <Classes/gtAABB.h>
#include <Classes/gtOBB.h>

#include <MainSystem/gtThread.h>
#include <MainSystem/gtMutex.h>

#include <GUISystem/gtGUIObject.h>

#include <PhysicsSystem/gtCollisionShape.h>
#include <PhysicsSystem/gtRigidBody.h>


#include <MainSystem/gtCVarSystem.h>
#include <PhysicsSystem/gtPhysicsSystem.h>
#include <SceneSystem/gtSceneSystem.h>
#include <AudioSystem/gtAudioSystem.h>
#include <InputSystem/gtInputController.h>
#include <GUISystem/gtGUISystem.h>
#include <InputSystem/gtInputSystem.h>


#include <MainSystem/gtMainSystem.h>


#include <Classes/gtImage.h>
#include <GraphicsSystem/gtTexture.h>
#include <GraphicsSystem/gtShader.h>
#include <Classes/gtMaterial.h>


#ifdef GT_DEBUG
#if defined( GT_PLATFORM_WIN32 )
#define GT_USE_STACK_TRACE
#endif
#endif
#include <MainSystem/gtStackTracer.h>


#include <Classes/gtSubModel.h>
#include <Classes/gtModel.h>
#include <GraphicsSystem/gtRenderModel.h>
#include <Classes/gtModelSystem.h>


#include <MainSystem/gtTimer.h>

#include <FileSystem/gtFileSystem.h>

#include <MainSystem/gtWindowInfo.h>

#include <GraphicsSystem/gtGraphicsSystem.h>

#include <SceneSystem/gtGameObject.h>

#include <PluginSystem/gtPluginSystem.h>

#include <MainSystem/gtWindow.h>

#include <MainSystem/gtOutputWindow.h>

#include <Common/gtUtilities.h>





namespace gost {

	// Эта функция импортируется из gost.dll Возвращается голый указатель, и, если система создана вызовом этой функции
	// нужно вручную вызвать release() при завершении работы
	extern "C" GT_API gtMainSystem* GT_CDECL gost_create_main_system_internal( const gtDeviceCreationParameters& params );
	
	// Эта функция возвращает gtPtrNew<gtMainSystem>, который записывается в gtPtr<gtMainSystem>. С помощью gtPtr, объекты сами вызывают 
	// метод release() (когда gtPtr уничтожается).
	// gtPtr<gtMainSystem> ms = gost_create_main_system(); //всё. release() вызывать не нужно
	GT_FORCE_INLINE gtPtr<gtMainSystem> gost_create_main_system( const gtDeviceCreationParameters& params = gtDeviceCreationParameters() ){
		return gtPtrNew<gtMainSystem>( gost_create_main_system_internal( params ) );
	}

#if defined(_MSC_VER)
#ifndef GOSTDLL
#ifdef GT_DEBUG
#pragma comment(lib, "gost_d.lib")
#else
#pragma comment(lib, "gost.lib")
#endif
#endif
#endif

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
