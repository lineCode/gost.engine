#include "common.h"

gtGUIMenuItemImpl::gtGUIMenuItemImpl( gtGraphicsSystem * d, gtGUIFont* font, gtGUIMenuImpl* menu ):
	m_gs( d ),
	m_mainSystem( nullptr ),
	m_modelSystem( nullptr ),
	m_wnd( nullptr ),
	m_font( font ),
	m_menu( menu ),
	m_userInput_id( -1 ),
	m_HeightLen( 0 ),
	m_active( false )
{
	m_mainSystem = gtMainSystem::getInstance();
	m_modelSystem = m_mainSystem->getModelSystem();
	m_type = gtGUIObjectType::MenuItem;
	m_gui = m_mainSystem->getGUISystem( d );
	m_wnd = m_gs->getParams().m_outWindow;

}

gtGUIMenuItemImpl::~gtGUIMenuItemImpl(){
}

gtGUIShape* gtGUIMenuItemImpl::getMouseHoverShape(){
	return m_itemMouseHover.data();
}

void gtGUIMenuItemImpl::update(){

	auto wrc = m_wnd->getClientRect();

	m_textField = m_gui->createTextField( m_rect, m_font, false, false );

	if( m_textField ){
		m_textField->setText( m_text );
		m_textField->setBackgroundVisible( false );
		m_textField->getBackgroundShape()->setColor( m_backgroundColor );
	}


	v4i itemMouseHover_rect = m_rect;
	itemMouseHover_rect.y -= 2;
	itemMouseHover_rect.w = m_menu->_getLineHeight();
	
	f32 oldTr = 1.f;
	if( m_itemMouseHover )
		oldTr = m_itemMouseHover->getTransparent();

	m_itemMouseHover = m_gui->createShapeRectangle( itemMouseHover_rect, m_menu->_getMouseHoverColor() );
	m_itemMouseHover->setTransparent( oldTr );

	for( auto i : m_items ){
		if( i->isVisible() )
			i->update();
	}

	bool add = false;
	gtGUIShape * old = nullptr;
	if( !m_background ) add = true;
	else old = m_background.data();

	m_background = m_gui->createShapeRectangle( m_backgroundRect, m_menu->_getMouseHoverColor());
	m_background->setTransparent( 0.7f );

	m_background->setActiveArea( m_backgroundRect );

	if( add )
		 m_gui->addToUserInput( m_background.data(), m_userInput_id+1000 );
	else m_gui->replaceUserInput( old, m_background.data(), m_userInput_id+1000 );
	
}

void gtGUIMenuItemImpl::render(){

	if( m_itemMouseHover )
		m_itemMouseHover->render();

	if( m_textField )
		m_textField->render();

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
	m_gradientColor1 = color1;
	m_gradientColor2 = color2;
}

void gtGUIMenuItemImpl::setTransparent( f32 transparent ){
}

bool gtGUIMenuItemImpl::init(const gtString & text, s32 userInput_id){
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
	m_backgroundColor = color;
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
}


/*
	При добавлении нужно:
	    - создать текстовое поле и добавить в массив.
		- в соответствии с размером текстового поля построить прямоугольник нового размера.
*/
gtGUIMenuItem* gtGUIMenuItemImpl::addMenuItem( const gtString& text, s32 userInput_id ){
	gtGUIMenuItemImpl * item = new gtGUIMenuItemImpl( m_gs, m_font, m_menu );
	if( item->init( text, userInput_id ) ){

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