#pragma once
#ifndef __GT_DEMO_CREATOR_H__
#define __GT_DEMO_CREATOR_H__

#include <random>

#include <gost.h>
using namespace gost;

constexpr u32 DEMO_GUI_ID_WELCOME_TEXT = 1u;
constexpr u32 DEMO_GUI_ID_PAUSE_CONTINUE = 2u;
constexpr u32 DEMO_GUI_ID_PAUSE_SETTINGS = 3u;
constexpr u32 DEMO_GUI_ID_PAUSE_EXIT = 4u;
constexpr u32 DEMO_GUI_ID_PAUSE_MAIN_MENU = 5u;
constexpr u32 DEMO_GUI_ID_LEFT_COLON = 100u;
constexpr u32 DEMO_GUI_ID_RIGHT_COLON = 200u;

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