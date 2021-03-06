﻿#pragma once
#ifndef __GT_GUI_OBJECT_H__
#define __GT_GUI_OBJECT_H__

namespace gost{

	enum class gtGUIObjectType : u32{
		None,
		Font,	
		Text,	
		Button,	
		Shape,
		TextField,
		Menu,
		MenuItem,
		MenuSubItem
	};

	class gtGUIObject : public gtRefObject{
	public:

		virtual void render() = 0;
		virtual void update() = 0;

		virtual void setTransparent( f32 transparent = 0.f ) = 0;
		virtual f32  getTransparent() = 0;

		virtual bool isMouseEnter() const = 0;
		virtual bool isMouseLeave() const = 0;
		virtual bool isVisible() const = 0;
		virtual void setMouseEnter() = 0;
		virtual void setMouseLeave() = 0;
		virtual void setVisible( bool value ) = 0;
		virtual void setActiveArea( const v4i& area ) = 0;
		virtual const v4i& getActiveArea() const = 0;

		virtual gtGUIObjectType getType() = 0;

		virtual void setRect( const v4i& rect ) = 0;

		virtual const v4i& getRect() = 0;

	};

}

#include <GUISystem/gtGUIObjectCommon.h>

#include <GUISystem/gtGUIFont.h>
#include <GUISystem/gtGUIStaticText.h>
#include <GUISystem/gtGUITextField.h>
#include <GUISystem/gtGUIMenuSubItem.h>
#include <GUISystem/gtGUIMenuItem.h>
#include <GUISystem/gtGUIMenu.h>

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