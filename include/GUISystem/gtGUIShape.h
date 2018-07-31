﻿#pragma once
#ifndef __GT_GUI_SHAPE_H__
#define __GT_GUI_SHAPE_H__

namespace gost{

	enum class gtGUIShapeType{
		Rectangle
	};

	class gtTexture;
	class gtMaterial;
	class gtGUIShape : public gtGUIObject{
	protected:
		gtGUIShapeType m_shapeType;
		bool m_isGradient;
	public:
		gtGUIShape():m_isGradient(false){}
		virtual ~gtGUIShape(){}

		virtual void setColor( const gtColor& color ) = 0;
		virtual void setTexture( gtTexture* texture ) = 0;
		virtual gtTexture* getTexture() = 0;
		virtual gtMaterial* getMaterial() = 0;

		virtual gtGUIShapeType getShapeType(){ return m_shapeType; }
		virtual bool isGradient(){ return m_isGradient; }
	
		virtual void setRect( const v4i& rect ) = 0; //set abstract rect
		virtual const v4i& getRect() = 0; //get abstract rect
	};

}

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