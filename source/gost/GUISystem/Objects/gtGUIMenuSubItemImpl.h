#pragma once
#ifndef __GT_GUI_MENU_SUB_ITEM_IMPL_H__
#define __GT_GUI_MENU_SUB_ITEM_IMPL_H__

namespace gost{

	class gtGUIMenuImpl;
	class gtGUIMenuItemImpl;
	class gtGUIMenuSubItemImpl : public gtGUIMenuSubItem{
		gtMainSystem *	   m_mainSystem;
		gtModelSystem*	   m_modelSystem;
		gtGraphicsSystem*  m_gs;
		gtGUISystem*	   m_gui;
		gtWindow *         m_wnd;
		gtGUIMenuImpl*     m_menu;
		gtGUIMenuItemImpl* m_parent;
		gtTexture *        m_windowItemIcon_texture;
		s32                m_userInput_id;
		s32                m_HeightLen; //������ ����, ����� ����� ���� ��������� ����� ��������
		bool               m_active;
		
		gtMaterial            m_material;
		gtPtr<gtGUIShape>     m_backgroundTexture; //�������� ������� ��������� �� �������
		gtPtr<gtGUIShape>     m_itemMouseHover;
		gtPtr<gtGUIShape>     m_windowItemIcon;
		gtPtr<gtGUITextField> m_textField;
		gtString              m_text;
		v4i                   m_windowItemIconRect;
		gtGUIMenuParameters   m_params;

		gtArray<gtPtr<gtGUIMenuItem>>     m_items;
		

	public:

		gtGUIMenuSubItemImpl( gtGraphicsSystem *, gtGUIMenuImpl* );
		~gtGUIMenuSubItemImpl();
		
		bool        _init(const gtString & text, s32 userInput_id, gtGUIMenuItemImpl *);

		void		update();
		void		render();
		void		setTransparent( f32 transparent = gtConst0F );
		f32			getTransparent();

		void        setTextColor( const gtColor& color );
		void		setColor( const gtColor& color );
		void        setGradientColor( const gtColor& color1, const gtColor& color2 );
		void		setTexture( gtTexture* texture );
		gtTexture*	getTexture();
		gtMaterial* getMaterial();
		void        setIcon( gtTexture * );
		bool        isActive();
		void        setActivate( bool activate );

		void        setBacgroundColor( const gtColor& color );

		void        setRect( const v4i& rect ) GT_OVERRIDE;
		
		gtGUIShape* getMouseHoverShape();

		void setMouseEnter()GT_OVERRIDE;
		void setMouseLeave()GT_OVERRIDE;
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