#pragma once
#ifndef __GT_DEMO_CREATOR_H__
#define __GT_DEMO_CREATOR_H__

#include <random>

#include <gost.h>
using namespace gost;

enum E_GUI_INPUT{
	DEMO_GUI_ID_WELCOME_TEXT = 1u,
	DEMO_GUI_ID_PAUSE_CONTINUE = 2u,
	DEMO_GUI_ID_PAUSE_SETTINGS = 3u,
	DEMO_GUI_ID_PAUSE_EXIT = 4u,
	DEMO_GUI_ID_PAUSE_MAIN_MENU = 5u,
	DEMO_GUI_ID_LEFT_COLON = 100u,
	DEMO_GUI_ID_RIGHT_COLON = 200u,
	//EGI_NONE = 1000,  // для того чтобы корректно работал GUI ввод нужно чтобы каждый элемент имел никальный номер
	//EGI_MENU,
	//EGI_MENU_FILE,
	//EGI_MENU_EDIT,
	//EGI_MENU_EDIT_DELETE,
	//EGI_MENU_EDIT_CUT,
	//EGI_MENU_EDIT_UNDO,
	//EGI_MENU_EDIT_REDO,
	//EGI_MENU_EDIT_SELECT_ALL,
	//EGI_MENU_EDIT_PASTE,
	//EGI_MENU_EDIT_COPY,
	//EGI_MENU_VIEW,
	//EGI_MENU_VIEW_FULLSCREEN,
	//EGI_MENU_FILE_EXIT,
	//EGI_MENU_FILE_NEW,
	//EGI_MENU_FILE_SAVE,
	//EGI_MENU_FILE_SAVE_AS,
	//EGI_MENU_FILE_CLOSE,
	//EGI_MENU_FILE_OPEN,
	//EGI_MAIN_BACKGROUND
};

#include "DemoExample.h"
#include "DemoElement.h"

#include "DemoApplicationEventConsumer.h"

#include "DemoApplication.h"
#include "DemoApplicationOutputWindow.h"

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