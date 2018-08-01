#include "common.h"

gtGUIMenuItemImpl::gtGUIMenuItemImpl( gtGraphicsSystem * d, gtGUIFont* font ):
	m_gs( d ),
	m_mainSystem( nullptr ),
	m_modelSystem( nullptr ),
	m_wnd( nullptr ),
	m_font( font ),
	m_userInput_id( -1 )
{
	m_mainSystem = gtMainSystem::getInstance();
	m_modelSystem = m_mainSystem->getModelSystem();
	m_type = gtGUIObjectType::Menu;
	m_gui = m_mainSystem->getGUISystem( d );
	m_wnd = m_gs->getParams().m_outWindow;

//	m_originalClientRect = m_wnd->getClientRect();
}

gtGUIMenuItemImpl::~gtGUIMenuItemImpl(){
}

void gtGUIMenuItemImpl::update(){

	auto wrc = m_wnd->getClientRect();
	//f32 px = (2.f/(f32)wrc.getWidth());
	//f32 py = (2.f/(f32)wrc.getHeight());
	
	//m_rect.x *= px;
	//m_rect.z *= px;
	//m_rect.y *= py;
	//m_rect.w *= py;

	/*
	auto wrc = m_wnd->getClientRect();

	v4i bgrc( 0, 0, wrc.z, m_paramHeight );
	bgrc.w = m_paramHeight;
	*/

	m_textField = m_gui->createTextField( m_rect, m_font, false, false );

	if( m_textField ){
		m_textField->setText( m_text );
		m_textField->setBackgroundVisible( false );
		m_textField->getBackgroundShape()->setColor( m_backgroundColor );
	}

	for( auto i : m_elements ){
		if( i.m_first->isVisible() ){
			i.m_first->update();
		}
	}

}

void gtGUIMenuItemImpl::render(){

	if( m_textField )
		m_textField->render();

	for( auto i : m_elements ){
		if( i.m_first->isVisible() )
			i.m_first->render();
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

void gtGUIMenuItemImpl::addElement( gtGUIObject* element, s32 id ){
	m_elements.push_back( gtPair<gtGUIObject*,s32>( element, id ) );
}

void gtGUIMenuItemImpl::setRect( const v4i& rect ){
	m_rect = rect;
	m_textField->setRect( rect );
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