﻿//	GOST

#include "common.h"

gtWindowCommon::gtWindowCommon():
	f_onMove( nullptr ),
	f_onSize( nullptr ),
	f_onPaint( nullptr ),
	m_isFullscreen( false ){
}

                    gtWindowCommon::~gtWindowCommon()           { }
const v4i&		    gtWindowCommon::getRect()                   { return m_params.m_rect; }
const v4i&          gtWindowCommon::getClientRect()             { return m_clientRect;    }
const gtWindowInfo& gtWindowCommon::getWindowInfo()             { return m_params;        }
void                gtWindowCommon::setOnMove( void(*f)(void) ) { f_onMove = f;           }
void                gtWindowCommon::setOnSize( void(*f)(void) ) { f_onSize = f;           }
void                gtWindowCommon::setOnPaint( void(*f)(void) ) { f_onPaint = f;           }
bool                gtWindowCommon::isFullscreen()              { return m_isFullscreen;  }


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