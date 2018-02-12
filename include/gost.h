/*!	GOST
	\file gost.h
	\brief GoST starts here
*/

#pragma once
#ifndef __GOST_H__
#define __GOST_H__ ///< include guard

#pragma warning(disable : 4582)

/*!
	\mainpage GoST 0.x API \a documentation
	\warning Made in Russia
	\attention Документация должна быть написана полностью на английском языке(дополнительно на русском). Автор не знаток english, по этому могут быть ошибки.

	Изучать движок лучше начиная с файла \ref gost.h\n

	\b Наименование \b плагинов:\n
	стандартные плагины должны начинаться с "gt"\n
	далее идут буквы обозначающие что это за плагин\n
	"ii" - import image\n
	"im" - import model\n
	"ei" - export image\n
	"em" - export model\n
	"r" - renderer\n
	"a" - audio\n

*/


//C++
#include <cctype> //isalpha
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <typeinfo>

//	GOST 
#include <gtInfo.h>
#include <gtTypes.h>


#include <gtError.h>

#include <gtString.h>

#include <gtBaseObject.h>
#include <gtRefObject.h>
#include <gtPtr.h>

#include <gtLoger.h>

#include <gtAllocator.h>

#include <gtInput.h>
#include <gtEvent.h>


#include <gtUtilities.h>


#include <gtArray.h>
#include <gtList.h>
#include <gtPriorQueue.h>

#include <gtMath.h>

#include <gtVector.h>
#include <gtQuaternion.h>
#include <gtMatrix.h>
#include <gtColor.h>

#include <gtAnimation.h>

#include <gtVertex.h>

#include <gtAABB.h>

#include <gtThread.h>


#include <gtSceneSystem.h>
#include <gtAudioSystem.h>

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







namespace gost{

	/*!	\brief GoST initialization

		\param params: параметры запуска движка
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