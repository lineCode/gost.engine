#include "common.h"

gtGUIMenuSubItemImpl::gtGUIMenuSubItemImpl( gtGraphicsSystem * d, gtGUIMenuImpl* menu ):
	m_gs( d ),
	m_mainSystem( nullptr ),
	m_modelSystem( nullptr ),
	m_wnd( nullptr ),
	m_menu( menu ),
	m_parent( nullptr ),
	m_windowItemIcon_texture( nullptr ),
	m_userInput_id( -1 ),
	m_HeightLen( 0 ),
	m_active( false )
{
	m_mainSystem = gtMainSystem::getInstance();
	m_modelSystem = m_mainSystem->getModelSystem();
	m_type = gtGUIObjectType::MenuSubItem;
	m_gui = m_mainSystem->getGUISystem( d );
	m_wnd = m_gs->getParams().m_outWindow;
	m_params = *menu->_getParams();
}

gtGUIMenuSubItemImpl::~gtGUIMenuSubItemImpl(){
}

void gtGUIMenuSubItemImpl::setIcon( gtTexture * t ){
	if( m_windowItemIcon ){
		m_windowItemIcon->setTexture( t );
	}
	m_windowItemIcon_texture = t;
}

gtGUIShape* gtGUIMenuSubItemImpl::getMouseHoverShape(){
	return m_itemMouseHover.data();
}

void gtGUIMenuSubItemImpl::update(){

	auto wrc = m_wnd->getClientRect();

	m_windowItemIconRect.x = m_rect.x;
	m_windowItemIconRect.y = m_rect.y;
	m_windowItemIconRect.z = m_rect.x + m_params.m_iconSize.x;
	m_windowItemIconRect.w = m_rect.y + m_params.m_iconSize.y;

	m_windowItemIcon = m_gui->createShapeRectangle( m_windowItemIconRect, gtColorWhite );
	m_windowItemIcon->setTexture( m_windowItemIcon_texture );

	v4i text_rect = m_windowItemIconRect;
		text_rect.x += m_params.m_iconSize.x;

	m_textField = m_gui->createTextField( text_rect, m_params.m_font, false, false );

	if( m_textField ){

		m_textField->setText( m_text );
		m_textField->setBackgroundVisible( false );

		m_textField->setTextColor( m_params.m_subitemTextColor );
	}


	v4i itemMouseHover_rect = text_rect;
	itemMouseHover_rect.x = m_rect.x;

	if( m_parent ){
	//	itemMouseHover_rect.z = m_parent->getBackgroundRect().z;
	}
	
	m_itemMouseHover = m_gui->createShapeRectangle( m_rect, m_params.m_subitemBackgroundColorHover,
		(m_params.m_flags&m_params.flag_subitemGradient)?true:false,
		m_params.m_subitemBackgroundGradientColor1Hover,
		m_params.m_subitemBackgroundGradientColor2Hover);
	m_itemMouseHover->setTransparent( m_params.m_subitemBackgroundTransparentHover );

	//m_itemMouseHover->setActiveArea( itemMouseHover_rect );

	if( m_params.m_itemHoverTexture ){
		m_backgroundTexture = m_gui->createShapeRectangle( itemMouseHover_rect, m_menu->_getItemHoverColor() );

		if( m_active )
			m_backgroundTexture->setTexture( m_params.m_itemActiveTexture );
		else
			m_backgroundTexture->setTexture( m_params.m_itemHoverTexture );
		m_backgroundTexture->setColor( gtColorWhite );
	}

	for( auto i : m_items ){
		i->update();
	}

	/*if( old )
		m_gui->replaceUserInput( old, m_background.data(), m_userInput_id );
	else
		m_gui->addToUserInput( m_background.data(), m_userInput_id );
	*/
}

void gtGUIMenuSubItemImpl::setMouseEnter(){
	m_mouseEnter = true;
	m_mouseLeave = false;

}

void gtGUIMenuSubItemImpl::setMouseLeave(){
	m_mouseLeave = true; 
	m_mouseEnter = false; 

}

void gtGUIMenuSubItemImpl::render(){

	if( m_mouseEnter ){
		m_itemMouseHover->render();
	}

	if( m_windowItemIcon ){
		if( m_windowItemIcon_texture )
			m_windowItemIcon->render();
	}

	if( m_textField ){

		if(m_active || m_mouseEnter){
				m_textField->setTextColor( m_params.m_subitemTextColorHover );
		}
		else{
				m_textField->setTextColor( m_params.m_subitemTextColor );
		}
		m_textField->render();
	}

	

	if( m_active ){

		for( auto i : m_items ){
		//	if( i->isVisible() )
		//		i->render();
		}
	}
}

void gtGUIMenuSubItemImpl::setGradientColor( const gtColor& color1, const gtColor& color2 ){
	m_params.m_menuGradientColor1 = color1;
	m_params.m_menuGradientColor2 = color2;
}

void gtGUIMenuSubItemImpl::setTransparent( f32 transparent ){
}

bool gtGUIMenuSubItemImpl::_init(const gtString & text, s32 userInput_id, gtGUIMenuItemImpl * p ){
	m_parent = p;
	m_text = text;
	m_userInput_id = userInput_id;

	update();

	m_textField->update();
	m_rect = m_textField->getRect();

	return true;
}

f32  gtGUIMenuSubItemImpl::getTransparent(){
	return 0.f;
}

void gtGUIMenuSubItemImpl::setColor( const gtColor& color ){
}

void gtGUIMenuSubItemImpl::setTextColor( const gtColor& color ){
	m_textField->setTextColor( color );
	m_params.m_subitemTextColor = color;
}


gtMaterial* gtGUIMenuSubItemImpl::getMaterial(){
	return &m_material;
}

void gtGUIMenuSubItemImpl::setTexture( gtTexture* texture ){
	m_material.textureLayer[ gtConst0U ].texture = texture;
}

gtTexture* gtGUIMenuSubItemImpl::getTexture(){
	return m_material.textureLayer[ gtConst0U ].texture;
}

void gtGUIMenuSubItemImpl::setBacgroundColor( const gtColor& color ){
	//m_params.m_menuItemBackgroundColor = color;
	if( m_textField ){
	//	m_textField->getBackgroundShape()->setColor( color );
	}
}

void gtGUIMenuSubItemImpl::setRect( const v4i& rect ){
	m_rect = rect;
	m_textField->setRect( rect );
}

bool        gtGUIMenuSubItemImpl::isActive(){
	return m_active;
}

void        gtGUIMenuSubItemImpl::setActivate( bool activate ){
}

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