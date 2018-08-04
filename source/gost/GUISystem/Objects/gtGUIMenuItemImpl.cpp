#include "common.h"

gtGUIMenuItemImpl::gtGUIMenuItemImpl( gtGraphicsSystem * d, gtGUIMenuImpl* menu ):
	m_gs( d ),
	m_mainSystem( nullptr ),
	m_modelSystem( nullptr ),
	m_wnd( nullptr ),
	m_menu( menu ),
	m_userInput_id( -1 ),
	m_HeightLen( 0 ),
	m_active( false ),
	m_isMenuItem( true )
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

gtGUIShape* gtGUIMenuItemImpl::getMouseHoverShape(){
	return m_itemMouseHover.data();
}

void gtGUIMenuItemImpl::update(){

	auto wrc = m_wnd->getClientRect();

	m_textField = m_gui->createTextField( m_rect, m_params.m_font, false, false );

	if( m_textField ){
		m_textField->setText( m_text );
		m_textField->setBackgroundVisible( false );
		m_textField->setTextColor( m_params.m_textColor );
	}


	v4i itemMouseHover_rect = m_rect;
	itemMouseHover_rect.y = 0;
	itemMouseHover_rect.w = m_menu->_getLineHeight();
	
	m_itemMouseHover = m_gui->createShapeRectangle( itemMouseHover_rect, m_menu->_getMouseHoverColor() );
	m_itemMouseHover->setTransparent( m_params.m_mouseHoverTransparent );

	if( m_params.m_menuItemHoverTexture ){
		m_backgroundTexture = m_gui->createShapeRectangle( itemMouseHover_rect, m_menu->_getMouseHoverColor() );
		m_backgroundTexture->setTexture( m_params.m_menuItemHoverTexture );
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

	m_background = m_gui->createShapeRectangle( m_backgroundRect, m_menu->_getMouseHoverColor());
	m_background->setTransparent( 0.7f );

	m_background->setActiveArea( m_backgroundRect );
	m_background->setVisible( is_visible );

	if( old )
		 m_gui->replaceUserInput( old, m_background.data(), m_userInput_id+10000 );
	else m_gui->addToUserInput( m_background.data(), m_userInput_id+10000 );
	
}

void gtGUIMenuItemImpl::setMouseEnter(){
	m_mouseEnter = true;
	m_mouseLeave = false;

	if( m_isMenuItem ){
		m_itemMouseHover->setTransparent( m_params.m_mouseHoverTransparent );
		m_textField->setTextColor( m_params.m_mouseHoverTextColor );
	}
}

void gtGUIMenuItemImpl::setMouseLeave(){
	m_mouseLeave = true; 
	m_mouseEnter = false; 

	if( m_isMenuItem ){
		m_itemMouseHover->setTransparent( 1.f );
		m_textField->setTextColor( m_params.m_textColor );
	}
}

void gtGUIMenuItemImpl::render(){

	if( m_itemMouseHover ){
		if( m_mouseEnter ){
			if( m_params.m_menuItemHoverTexture )
				m_backgroundTexture->render();
			m_itemMouseHover->render();
		}
	}


	if( m_textField ){
		m_textField->render();
	}

	if( m_active ){
		if( m_background )
			m_background->render();

		for( auto i : m_items ){
			if( i->isVisible() )
				i->render();
		}
	}
}

void gtGUIMenuItemImpl::setGradientColor( const gtColor& color1, const gtColor& color2 ){
	m_params.m_gradientColor1 = color1;
	m_params.m_gradientColor2 = color2;
}

void gtGUIMenuItemImpl::setTransparent( f32 transparent ){
	if( m_isMenuItem ){ //если пункт в меню
		m_params.m_mouseHoverTransparent = transparent;
		m_itemMouseHover->setTransparent( m_params.m_mouseHoverTransparent );
	}else{ // если пункт в окошке
		//циклом пройтись
	}
}

bool gtGUIMenuItemImpl::_init(const gtString & text, s32 userInput_id, bool isMenuItem){
	m_isMenuItem = isMenuItem;
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
	m_params.m_textColor = color;
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
	m_params.m_menuItemBackgroundColor = color;
	if( m_textField ){
		m_textField->getBackgroundShape()->setColor( color );
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
	m_active = activate;
	if( m_background )
		m_background->setVisible(m_active);

	for( auto i : m_items ){
		i->setVisible(m_active);
	}
}


/*
	При добавлении нужно:
	    - создать текстовое поле и добавить в массив.
		- в соответствии с размером текстового поля построить прямоугольник нового размера.
*/
gtGUIMenuItem* gtGUIMenuItemImpl::addMenuItem( const gtString& text, s32 userInput_id ){
	gtGUIMenuItemImpl * item = new gtGUIMenuItemImpl( m_gs, m_menu );
	if( item->_init( text, userInput_id, false ) ){

		item->setVisible( false );

		m_items.push_back( gtPtrNew<gtGUIMenuItem>( item ) );

		auto r = item->getRect();
		auto w = r.getHeight();

		r.y = m_menu->_getLineHeight();
		r.w = r.y + r.y;
		r.y += m_HeightLen;
		r.w += m_HeightLen;

		r.x = m_rect.x;
		

		m_HeightLen += w;

		item->setRect( r );
		item->setActiveArea( r );
		item->update();


		r = item->getRect();



		m_backgroundRect.x = m_rect.x;
		m_backgroundRect.y = m_menu->_getLineHeight();
		if( r.x + r.z > m_backgroundRect.z ) m_backgroundRect.z = r.x + r.z;
		if( r.w > m_backgroundRect.w ) m_backgroundRect.w = r.w;

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