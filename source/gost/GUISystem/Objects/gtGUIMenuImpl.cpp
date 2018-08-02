#include "common.h"

gtGUIMenuImpl::gtGUIMenuImpl( gtGraphicsSystem * d, gtGUIFont* font ):
	m_gs( d ),
	m_mainSystem( nullptr ),
	m_modelSystem( nullptr ),
	m_wnd( nullptr ),
	m_font( font ),
	m_paramHeight( 10 ),
	m_widthLen( 0 )
{
	m_mainSystem = gtMainSystem::getInstance();
	m_modelSystem = m_mainSystem->getModelSystem();
	m_type = gtGUIObjectType::Menu;
	m_gui = m_mainSystem->getGUISystem( d );
	m_wnd = m_gs->getParams().m_outWindow;
}

gtGUIMenuImpl::~gtGUIMenuImpl(){
}

s32 gtGUIMenuImpl::_getLineHeight(){
	return m_paramHeight;
}

const gtColor& gtGUIMenuImpl::_getMouseHoverColor(){
	return m_mouseHoverColor;
}

void gtGUIMenuImpl::update(){

	auto wrc = m_wnd->getClientRect();

	v4i bgrc( 0, 0, wrc.z, m_paramHeight );
	bgrc.w = m_paramHeight;

	m_backgroundShape = m_gui->createShapeRectangle( bgrc, m_backgroundColor, true, m_gradientColor1, m_gradientColor2 );
	m_backgroundShape->setColor( m_backgroundColor );

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

	if( m_backgroundShape ){
		if( m_backgroundShape->isVisible() )
			m_backgroundShape->render();
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
	m_gradientColor1 = color1;
	m_gradientColor2 = color2;
}

void gtGUIMenuImpl::setTransparent( f32 transparent ){
}

bool gtGUIMenuImpl::_init( s32 h ){
	m_paramHeight = h;
	
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
	return m_backgroundColor;
}

void gtGUIMenuImpl::setBacgroundColor( const gtColor& color ){
	m_backgroundColor = color;
	if( m_backgroundShape ){
		m_backgroundShape->setColor( color );
	}
}

void gtGUIMenuImpl::setMouseHoverColor( const gtColor& color ){
	m_mouseHoverColor = color;
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

gtGUIMenuItem* gtGUIMenuImpl::addMenuItem( const gtString& text, s32 userInput_id ){

	gtGUIMenuItemImpl * item = new gtGUIMenuItemImpl( m_gs, m_font, this );
	if( item->init( text, userInput_id ) ){

		m_items.push_back( gtPtrNew<gtGUIMenuItem>( item ) );

		auto r = item->getRect();
		auto w = r.getWidth();


		r.x += m_widthLen;
		r.z += m_widthLen;
		r.y += 2;
		r.w += 2;

		m_widthLen += w;

		item->setRect( r );
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