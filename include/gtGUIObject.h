#pragma once
#ifndef __GT_GUI_OBJECT_H__
#define __GT_GUI_OBJECT_H__

namespace gost{

	enum class gtGUIObjectType{
		Font,	
		Text,	
		Button,	
		Shape,
		TextField
	};

	class gtGUIObject : public gtRefObject{
	protected:
		gtGUIObjectType m_type;
		bool			m_visible;
	public:

		gtGUIObject() : m_visible( true ){}
		virtual ~gtGUIObject(){}

		virtual void render() = 0;

		virtual void setOpacity( f32 opacity = gtConst1F ) = 0;
		virtual f32  getOpacity() = 0;

		virtual gtGUIObjectType getType(){
			return m_type;
		}

		virtual bool isVisible() const {
			return m_visible;
		}

		virtual void setVisible( bool value ){
			m_visible = value;
		}

		

	};

}

#include <gtGUIShape.h>

#include <gtGUIFont.h>
#include <gtGUIStaticText.h>
#include <gtGUITextField.h>

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