#include "common.h"

gtGUIMenuImpl::gtGUIMenuImpl( gtGraphicsSystem * d, const gtGUIMenuParameters& params ):
	m_gs( d ),
	m_mainSystem( nullptr ),
	m_modelSystem( nullptr ),
	m_wnd( nullptr ),
	m_widthLen( 0 )
{
	m_mainSystem = gtMainSystem::getInstance();
	m_modelSystem = m_mainSystem->getModelSystem();
	m_type = gtGUIObjectType::Menu;
	m_gui = m_mainSystem->getGUISystem( d );
	m_wnd = m_gs->getParams().m_outWindow;
	m_params = params;
}

gtGUIMenuImpl::~gtGUIMenuImpl(){
}

s32 gtGUIMenuImpl::_getLineHeight(){
	return m_params.m_menuHeight;
}

const gtColor& gtGUIMenuImpl::_getItemHoverColor(){
	return m_params.m_itemHoverColor;
}

gtGUIMenuParameters * gtGUIMenuImpl::_getParams(){
	return &m_params;
}

void gtGUIMenuImpl::update(){

	auto wrc = m_wnd->getClientRect();

	m_rect = v4i( 0, 0, wrc.z, m_params.m_menuHeight );
	m_rect.w = m_params.m_menuHeight;

	m_backgroundShape = m_gui->createShapeRectangle( m_rect, m_params.m_menuColor, true, m_params.m_menuGradientColor1, m_params.m_menuGradientColor2 );
	m_backgroundShape->setColor( m_params.m_menuColor );
	
	if( m_params.m_flags & m_params.flag_menuHover ){
		m_backgroundShapeHover = m_gui->createShapeRectangle( m_rect, m_params.m_menuColorHover, true, m_params.m_menuGradientColor1Hover, m_params.m_menuGradientColor2Hover );
		m_backgroundShapeHover->setColor( m_params.m_menuColorHover );
	}
	
	if( m_params.m_menuBackgroundTexture ){
		m_backgroundShapeWithTexture = m_gui->createShapeRectangle( m_rect, m_params.m_menuColor );
		m_backgroundShapeWithTexture->setTexture( m_params.m_menuBackgroundTexture );
	}

	setActiveArea( m_rect );

	for( auto i : m_elements ){
		if( i.m_first->isVisible() ){
			i.m_first->update();
		}
	}

	for( auto i : m_items ){
		if( i->isVisible() )
			i->update();
	}


}

void gtGUIMenuImpl::render(){
	
	bool normalShape = true;
	if( this->isMouseEnter() ){
		if( m_params.m_flags & m_params.flag_menuHover ){
			normalShape = false;
			if( m_backgroundShapeHover->isVisible() )
				m_backgroundShapeHover->render();
		}
	}

	if( m_backgroundShape && normalShape ){
		if( m_backgroundShape->isVisible() )
			m_backgroundShape->render();
	}

	if( m_params.m_menuBackgroundTexture ){
		if( m_backgroundShapeWithTexture ){
			m_backgroundShapeWithTexture->render();
		}
	}

	for( auto i : m_elements ){
		if( i.m_first->isVisible() )
			i.m_first->render();
	}

	for( auto i : m_items ){
		if( i->isVisible() )
			i->render();
	}
}

void gtGUIMenuImpl::setGradientColor( const gtColor& color1, const gtColor& color2 ){
	m_params.m_menuGradientColor1 = color1;
	m_params.m_menuGradientColor2 = color2;
}

void gtGUIMenuImpl::setTransparent( f32 transparent ){
}

bool gtGUIMenuImpl::_init(){
	setBacgroundColor( m_params.m_menuColor );

	if( m_params.m_flags & m_params.flag_gradient ){
		setGradientColor( m_params.m_menuGradientColor1, m_params.m_menuGradientColor2 );
	}

	update();
	return true;
}

f32  gtGUIMenuImpl::getTransparent(){
	return 0.f;
}

void gtGUIMenuImpl::setColor( const gtColor& color ){
}

gtMaterial* gtGUIMenuImpl::getMaterial(){
	return &m_material;
}

void gtGUIMenuImpl::setTexture( gtTexture* texture ){
	m_material.textureLayer[ gtConst0U ].texture = texture;
}

gtTexture* gtGUIMenuImpl::getTexture(){
	return m_material.textureLayer[ gtConst0U ].texture;
}

const gtColor& gtGUIMenuImpl::getBacgroundColor(){
	return m_params.m_menuColor;
}

void gtGUIMenuImpl::setBacgroundColor( const gtColor& color ){
	m_params.m_menuColor = color;
	if( m_backgroundShape ){
		m_backgroundShape->setColor( color );
	}
}

void gtGUIMenuImpl::setMouseHoverColor( const gtColor& color ){
	m_params.m_itemHoverColor = color;
}

void gtGUIMenuImpl::addElement( gtGUIObject* element, s32 id ){
	m_elements.push_back( gtPair<gtGUIObject*,s32>( element, id ) );

	auto r = element->getRect();
	auto w = r.getWidth();


	r.x += m_widthLen;
	r.z += m_widthLen;

	m_widthLen += w;


	element->setRect( r );
	element->setActiveArea( r );
	element->update();
}

void gtGUIMenuImpl::setVisible( bool value ){ 
	m_visible = value;
	for( auto i : m_elements ){
		i.m_first->setVisible( value );
	}

	for( auto i : m_items ){
		i->setVisible( value );
	}
}
gtGUIMenuItem* gtGUIMenuImpl::addMenuItem( const gtString& text, s32 userInput_id ){

	gtGUIMenuItemImpl * item = new gtGUIMenuItemImpl( m_gs, this );
	if( item->_init( text, userInput_id ) ){

		item->setTextColor( m_params.m_itemTextColor );

		m_items.push_back( gtPtrNew<gtGUIMenuItem>( item ) );

		auto r = item->getRect();
		auto w = r.getWidth();


		r.x += m_widthLen;
		r.z += m_widthLen;
		r.w = m_rect.w;

		m_widthLen += w;


		r.y += m_params.m_menuTextIndent;
		item->setRect( r );

		r.y -= m_params.m_menuTextIndent;
		item->setActiveArea( r );
		item->update();

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