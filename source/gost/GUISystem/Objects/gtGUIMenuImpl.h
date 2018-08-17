#pragma once
#ifndef __GT_GUI_MENU_IMPL_H__
#define __GT_GUI_MENU_IMPL_H__

namespace gost{


	class gtGUIMenuImpl : public gtGUIMenu{
		gtMainSystem *	  m_mainSystem;
		gtModelSystem*	  m_modelSystem;
		gtGraphicsSystem* m_gs;
		gtGUISystem*	  m_gui;
		gtWindow *        m_wnd;
		s32               m_widthLen;
		
		gtMaterial      m_material;

		gtPtr<gtGUIShape> m_backgroundShape;
		gtPtr<gtGUIShape> m_backgroundShapeHover; //если нужно чтобы при намведении курсора полоска реагировала
		gtPtr<gtGUIShape> m_backgroundShapeWithTexture;
		
		gtGUIMenuParameters m_params;

		v4i             m_backgroundRect;
		

		gtArray<gtPair<gtGUIObject*,s32>> m_elements;
		gtArray<gtPtr<gtGUIMenuItem>>     m_items;

	public:

		gtGUIMenuImpl( gtGraphicsSystem *, const gtGUIMenuParameters& params );
		~gtGUIMenuImpl();
		
		s32         _getLineHeight();
		const gtColor& _getItemHoverColor();
		bool        _init();
		gtGUIMenuParameters * _getParams();

		void		update();
		void		render();
		void		setTransparent( f32 transparent = gtConst0F );
		f32			getTransparent();

		void        setBacgroundColor( const gtColor& color );
		const gtColor& getBacgroundColor();
		void        setMouseHoverColor( const gtColor& color );
		void		setColor( const gtColor& color );
		void        setGradientColor( const gtColor& color1, const gtColor& color2 );
		void		setTexture( gtTexture* texture );
		gtTexture*	getTexture();
		gtMaterial* getMaterial();

		void           addElement( gtGUIObject* element, s32 id );
		gtGUIMenuItem* addMenuItem( const gtString& text, s32 userInput_id );
		
		void  setVisible( bool value )GT_OVERRIDE;
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