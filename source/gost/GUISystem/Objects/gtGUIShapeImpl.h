#pragma once
#ifndef __GT_GUI_SHAPE_IMPL_H__
#define __GT_GUI_SHAPE_IMPL_H__

namespace gost{


	class gtGUIShapeImpl : public gtGUIShape, public gtGUIShapeCommon{
		gtMainSystem *	   m_mainSystem;
		gtModelSystem*	   m_modelSystem;
		gtGraphicsSystem*  m_gs;
		gtRenderModel*	   m_model;
		gtMaterial*		   m_material;
		gtTexture *        m_texture;
		bool               m_useVertGradient;
		
		gtVertexType	vt[ 5u ];
		gtColor         m_difColor;
		gtColor         m_gr1Color;
		gtColor         m_gr2Color;
		

	public:

		gtGUIShapeImpl( gtGraphicsSystem * );
		~gtGUIShapeImpl();
		
		bool		initRectangle( const v4i& rect, const gtColor& color, bool useGradient = false, 
			const gtColor& first_color = gtColorBlack, const gtColor& second_color = gtColorBlack, bool useVerticalGradient = false );

		void		update();
		void		render();
		void		setTransparent( f32 transparent = gtConst0F );
		f32			getTransparent();

		void		setColor( const gtColor& color );
		void		setTexture( gtTexture* texture );
		gtTexture*	getTexture();
		gtMaterial* getMaterial();

		void        setRect( const v4i& rect );
		const v4i&  getRect();
		
		gtGUIShapeType getShapeType();
		bool           isGradient();
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