cmake_minimum_required(VERSION 2.6)
project(gost)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

set(SRC_FILE_SYSTEM_SYSTEM FileSystem/System/gtFileSystemCommon.cpp FileSystem/System/gtFileSystemImpl.cpp FileSystem/System/gtFileSystemWin32.cpp FileSystem/System/gtFileWin32.cpp)
set(SRC_FILE_SYSTEM_XML FileSystem/XML/gtXMLImpl.cpp FileSystem/XML/gtXMLDocumentImpl.cpp )
set(SRC_FILE_SYSTEM ${SRC_FILE_SYSTEM_XML} ${SRC_FILE_SYSTEM_SYSTEM})

set(SRC_GUI_SYSTEM_FONTS GUISystem/Fonts/gtGUIFontImpl.cpp)
set(SRC_GUI_SYSTEM_OBJECTS GUISystem/Objects/gtGUIMenuImpl.cpp GUISystem/Objects/gtGUIMenuItemImpl.cpp GUISystem/Objects/gtGUIMenuSubItemImpl.cpp GUISystem/Objects/gtGUIShapeImpl.cpp GUISystem/Objects/gtGUIStaticTextImpl.cpp GUISystem/Objects/gtGUITextFieldImpl.cpp)
set(SRC_GUI_SYSTEM_SYSTEM GUISystem/System/gtGUISystemImpl.cpp)
set(SRC_GUI_SYSTEM ${SRC_GUI_SYSTEM_SYSTEM} ${SRC_GUI_SYSTEM_FONTS} ${SRC_GUI_SYSTEM_OBJECTS})

set(SRC_INPUT_SYSTEM InputSystem/gtInputSystemImpl.cpp)

set(SRC_MAIN_SYSTEM_ASYNC MainSystem/Async/gtMutexWin32.cpp MainSystem/Async/gtThreadWin32.cpp)
set(SRC_MAIN_SYSTEM_CVAR MainSystem/ConsoleVariable/gtCvarSystemImpl.cpp)
set(SRC_MAIN_SYSTEM_EVENTS MainSystem/Events/gtEventSystem.cpp)
set(SRC_MAIN_SYSTEM_LOG MainSystem/Log/gtLogImpl.cpp)
set(SRC_MAIN_SYSTEM_TIMER MainSystem/Timer/gtTimerWin32.cpp)
set(SRC_MAIN_SYSTEM_WINDOW MainSystem/Window/gtOutputWindowWin32.cpp MainSystem/Window/gtWindowCommon.cpp MainSystem/Window/gtWindowWin32.cpp)
set(SRC_MAIN_SYSTEM_MEMORY MainSystem/Memory/gtMemorySystem.cpp MainSystem/Memory/gtMemorySystemImplWin32.cpp)
set(SRC_MAIN_SYSTEM_SYSTEM MainSystem/System/gtMainSystemCommon.cpp MainSystem/System/gtMainSystemWin32.cpp)

set(SRC_MAIN_SYSTEM ${SRC_MAIN_SYSTEM_ASYNC} ${SRC_MAIN_SYSTEM_CVAR} ${SRC_MAIN_SYSTEM_EVENTS} ${SRC_MAIN_SYSTEM_LOG} ${SRC_MAIN_SYSTEM_TIMER} ${SRC_MAIN_SYSTEM_WINDOW} ${SRC_MAIN_SYSTEM_MEMORY} ${SRC_MAIN_SYSTEM_SYSTEM})

set(SRC_MODEL_SYSTEM ModelSystem/gtModelImpl.cpp ModelSystem/gtModelSystemImpl.cpp)

set(SRC_PLUGIN_SYSTEM_PLUGINS PluginSystem/Plugins/gtAudioPlugin.cpp PluginSystem/Plugins/gtPluginImportImage.cpp PluginSystem/Plugins/gtPluginImportModel.cpp PluginSystem/Plugins/gtPluginInput.cpp PluginSystem/Plugins/gtPluginPhysics.cpp PluginSystem/Plugins/gtPluginRender.cpp)
set(SRC_PLUGIN_SYSTEM_SYSTEM PluginSystem/System/gtPluginSystemImpl.cpp)
set(SRC_PLUGIN_SYSTEM ${SRC_PLUGIN_SYSTEM_SYSTEM} ${SRC_PLUGIN_SYSTEM_PLUGINS})

set(SRC_SCENE_SYSTEM_OBJECTS SceneSystem/Objects/gtCameraImpl.cpp SceneSystem/Objects/gtDummyObjectImpl.cpp SceneSystem/Objects/gtStaticObjectImpl.cpp)
set(SRC_SCENE_SYSTEM_SYSTEM SceneSystem/System/gtSceneSystemImpl.cpp)
set(SRC_SCENE_SYSTEM ${SRC_SCENE_SYSTEM_SYSTEM} ${SRC_SCENE_SYSTEM_OBJECTS})

set(SOURCES ${SRC_FILE_SYSTEM} ${SRC_GUI_SYSTEM} ${SRC_INPUT_SYSTEM} ${SRC_MAIN_SYSTEM} ${SRC_MODEL_SYSTEM} ${SRC_PLUGIN_SYSTEM} ${SRC_SCENE_SYSTEM} dllmain.cpp gost.cpp)

add_executable(gost ${SOURCES})
