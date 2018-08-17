#include "common.h"

gtGUIMenuItemImpl::gtGUIMenuItemImpl( gtGraphicsSystem * d, gtGUIMenuImpl* menu ):
	m_mainSystem( nullptr ),
	m_modelSystem( nullptr ),
	m_gs( d ),
	m_gui( nullptr ),
	m_wnd( nullptr ),
	m_menu( menu ),
	m_windowItemIcon_texture( nullptr ),
	m_userInput_id( -1 ),
	m_HeightLen( 0 ),
	m_active( false )
{
	m_mainSystem = gtMainSystem::getInstance();
	m_modelSystem = m_mainSystem->getModelSystem();
	m_type = gtGUIObjectType::MenuItem;
	m_gui = m_mainSystem->getGUISystem( d );
	m_wnd = m_gs->getParams().m_outWindow;
	m_params = *menu->_getParams();
}

gtGUIMenuItemImpl::~gtGUIMenuItemImpl(){
}

void gtGUIMenuItemImpl::setIcon( gtTexture * t ){
	if( m_windowItemIcon ){
		m_windowItemIcon->setTexture( t );
	}
	m_windowItemIcon_texture = t;
}

const v4i&  gtGUIMenuItemImpl::getBackgroundRect(){
	return m_backgroundRect;
}

gtGUIShape* gtGUIMenuItemImpl::getMouseHoverShape(){
	return m_itemMouseHover.data();
}

void gtGUIMenuItemImpl::update(){

//	auto wrc = m_wnd->getClientRect();

	v4i text_rect = m_rect;

	m_textField = m_gui->createTextField( text_rect, m_params.m_font, false, false );

	if( m_textField ){
		m_textField->setText( m_text );
		m_textField->setBackgroundVisible( false );

			m_textField->setTextColor( m_params.m_itemTextColor );
	}


	v4i itemMouseHover_rect = m_rect;
	
		itemMouseHover_rect.y = 0;
		itemMouseHover_rect.w = m_menu->_getLineHeight();
	
	m_itemMouseHover = m_gui->createShapeRectangle( itemMouseHover_rect, m_menu->_getItemHoverColor() );
	m_itemMouseHover->setTransparent( m_params.m_itemHoverTransparent );
	m_itemMouseHover->setActiveArea( itemMouseHover_rect );

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

	gtGUIShape * old = nullptr;
	bool is_visible = false;
	if( m_background ){
		old = m_background.data();
		is_visible = old->isVisible();
	}

	m_background = m_gui->createShapeRectangle( m_backgroundRect, m_menu->_getItemHoverColor());
	m_background->setTransparent( m_params.m_itemBackgroundTransparent );
	m_background->setActiveArea( m_backgroundRect );
	m_background->setVisible( is_visible );
	m_background->setColor( m_params.m_itemBackgroundColor );

	if( old )
		m_gui->replaceUserInput( old, m_background.data(), m_userInput_id );
	else
		m_gui->addToUserInput( m_background.data(), m_userInput_id );
	
}

void gtGUIMenuItemImpl::setMouseEnter(){
	m_mouseEnter = true;
	m_mouseLeave = false;

	m_itemMouseHover->setTransparent( m_params.m_itemHoverTransparent );
	m_textField->setTextColor( m_params.m_itemHoverTextColor );

	m_menu->setMouseEnter();
}

void gtGUIMenuItemImpl::setMouseLeave(){
	m_mouseLeave = true; 
	m_mouseEnter = false; 

	m_itemMouseHover->setTransparent( 1.f );
	m_textField->setTextColor( m_params.m_itemTextColor );
	
	m_menu->setMouseLeave();
}

void gtGUIMenuItemImpl::render(){

	if( ((m_itemMouseHover==true) && (m_mouseEnter==true))
		|| (m_active==true) ){
		if( m_params.m_itemHoverTexture || m_params.m_itemActiveTexture )
			m_backgroundTexture->render();
		m_itemMouseHover->render();
	}
	
	if( m_windowItemIcon ){
		if( m_windowItemIcon_texture )
			m_windowItemIcon->render();
	}

	if( m_textField ){

		if(m_active || m_mouseEnter){
			m_textField->setTextColor( m_params.m_itemHoverTextColor );
		}
		else{
			m_textField->setTextColor( m_params.m_itemTextColor );
		}
		m_textField->render();
	}

	

	if( m_active ){
		if( m_background ){
			m_background->render();
		}

		for( auto i : m_items ){
			if( i->isVisible() )
				i->render();
		}
	}
}

void gtGUIMenuItemImpl::setGradientColor( const gtColor& color1, const gtColor& color2 ){
	m_params.m_menuGradientColor1 = color1;
	m_params.m_menuGradientColor2 = color2;
}

void gtGUIMenuItemImpl::setTransparent( f32 transparent ){
		m_params.m_itemHoverTransparent = transparent;
		m_itemMouseHover->setTransparent( m_params.m_itemHoverTransparent );
}

bool gtGUIMenuItemImpl::_init(const gtString & text, s32 userInput_id ){
	m_text = text;
	m_userInput_id = userInput_id;

	update();

	m_textField->update();
	m_rect = m_textField->getRect();

	return true;
}

f32  gtGUIMenuItemImpl::getTransparent(){
	return 0.f;
}

void gtGUIMenuItemImpl::setColor( const gtColor& color ){
}

void gtGUIMenuItemImpl::setTextColor( const gtColor& color ){
	m_textField->setTextColor( color );
	m_params.m_itemTextColor = color;
}


gtMaterial* gtGUIMenuItemImpl::getMaterial(){
	return &m_material;
}

void gtGUIMenuItemImpl::setTexture( gtTexture* texture ){
	m_material.textureLayer[ gtConst0U ].texture = texture;
}

gtTexture* gtGUIMenuItemImpl::getTexture(){
	return m_material.textureLayer[ gtConst0U ].texture;
}

void gtGUIMenuItemImpl::setBacgroundColor( const gtColor& color ){
	m_params.m_itemBackgroundColor = color;
	if( m_textField ){
	//	m_textField->getBackgroundShape()->setColor( color );
	}
}

void gtGUIMenuItemImpl::setRect( const v4i& rect ){
	m_rect = rect;
	m_textField->setRect( rect );
}

bool        gtGUIMenuItemImpl::isActive(){
	return m_active;
}

void        gtGUIMenuItemImpl::setActivate( bool activate ){
		if( m_active == activate ) return;

		if( m_backgroundTexture ){
			if( activate ){
				if( m_params.m_itemActiveTexture )
					m_backgroundTexture->setTexture( m_params.m_itemActiveTexture );
			}else
				if( m_params.m_itemHoverTexture )
					m_backgroundTexture->setTexture( m_params.m_itemHoverTexture );
		}

		if( m_background )
			m_background->setVisible( activate );

		for( auto i : m_items ){
			i->setVisible( activate );
		}

		m_active = activate;
}


/*
	При добавлении нужно:
	    - создать текстовое поле и добавить в массив.
		- в соответствии с размером текстового поля построить прямоугольник нового размера.
*/
gtGUIMenuSubItem* gtGUIMenuItemImpl::addMenuSubItem( const gtString& text, s32 userInput_id ){
	gtGUIMenuSubItemImpl * item = new gtGUIMenuSubItemImpl( m_gs, m_menu );
	if( item->_init( text, userInput_id, this ) ){

		item->setVisible( false );

		m_items.push_back( gtPtrNew<gtGUIMenuSubItem>( item ) );

		auto r = item->getRect();
		auto h = r.getHeight();
		r.x = m_rect.x;

		r.y = m_menu->_getLineHeight();
		

		r.y += m_HeightLen;

		r.w = r.y + h;


		m_HeightLen += h + m_params.m_menuTextIndent;

		item->setRect( r );
		item->setActiveArea( r );
		item->update();
		
		
		m_backgroundRect.x = m_rect.x;
		m_backgroundRect.y = m_menu->_getLineHeight();
		if( r.x + r.z > m_backgroundRect.z ) m_backgroundRect.z = r.x + r.z + m_params.m_iconSize2.x;
		if( r.w > m_backgroundRect.w ) m_backgroundRect.w = r.w;
		m_backgroundRect.w -= m_params.m_menuTextIndent;

		for( auto i : m_items ){
			v4i rect = i->getRect();
			rect.z = m_backgroundRect.z;
			i->setRect( rect );
			i->setActiveArea( rect );
		}
		

		m_gui->addToUserInput( item, userInput_id );
		
		return item;
	}

	return nullptr;
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