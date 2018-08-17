//	GOST

#pragma once
#ifndef __STDAFX_H__
#define __STDAFX_H__


#define GOSTDLL
#define GT_EXPORTS


#include <gost.h>


using namespace gost;

#define GOST_MAJOR_VERSION 0
#define GOST_MINOR_VERSION 2


//	GOST
#include "MainSystem/ConsoleVariable/gtCvarSystemImpl.h"

#include "MainSystem/Memory/gtMemorySystemCommon.h"
#include "MainSystem/Memory/gtMemorySystemImplWin32.h"

#include "SceneSystem/Objects/gtCameraImpl.h"
#include "SceneSystem/Objects/gtStaticObjectImpl.h"
#include "SceneSystem/Objects/gtDummyObjectImpl.h"

#include "ModelSystem/gtModelImpl.h"
#include "ModelSystem/gtModelSystemImpl.h"
#include "SceneSystem/System/gtSceneSystemImpl.h"

#include "MainSystem/Window/gtOutputWindowWin32.h"

#include "FileSystem/System/gtFileWin32.h"

#include "FileSystem/System/gtFileSystemCommon.h"
#include "FileSystem/System/gtFileSystemWin32.h"

#include "PluginSystem/System/gtPluginSystemImpl.h"

#include "MainSystem/Timer/gtTimerWin32.h"

#include "InputSystem/gtInputSystemImpl.h"
#include "MainSystem/Events/gtEventSystem.h"

#include "MainSystem/Async/gtThreadWin32.h"
#include "MainSystem/Async/gtMutexWin32.h"

#include "FileSystem/XML/gtXMLDocumentImpl.h"

#include "GUISystem/Objects/gtGUIShapeImpl.h"
#include "GUISystem/System/gtGUISystemImpl.h"
#include "GUISystem/Fonts/gtGUIFontImpl.h"
#include "GUISystem/Objects/gtGUIStaticTextImpl.h"
#include "GUISystem/Objects/gtGUITextFieldImpl.h"
#include "GUISystem/Objects/gtGUIMenuSubItemImpl.h"
#include "GUISystem/Objects/gtGUIMenuItemImpl.h"
#include "GUISystem/Objects/gtGUIMenuImpl.h"


#include "MainSystem/System/gtMainSystemCommon.h"
#include "MainSystem/System/gtMainSystemWin32.h"

#include "MainSystem/Log/gtLogImpl.h"


#include "MainSystem/Window/gtWindowCommon.h"
#include "MainSystem/Window/gtWindowWin32.h"



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