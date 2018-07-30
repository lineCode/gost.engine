#include "common.h"

gtGUIMenuImpl::gtGUIMenuImpl( gtGraphicsSystem * d ):
	m_gs( d ),
	m_mainSystem( nullptr ),
	m_modelSystem( nullptr ),
	m_wnd( nullptr ),
	m_paramHeight( 10 )
{
	m_mainSystem = gtMainSystem::getInstance();
	m_modelSystem = m_mainSystem->getModelSystem();
	m_type = gtGUIObjectType::Menu;
	m_gui = m_mainSystem->getGUISystem( d );
	m_wnd = m_gs->getParams().m_outWindow;
}

gtGUIMenuImpl::~gtGUIMenuImpl(){
}

void gtGUIMenuImpl::update(){

	auto wrc = m_wnd->getClientRect();

	v4i bgrc( 0, 0, wrc.z, m_paramHeight );

	if( m_backgroundRect != bgrc ){
		m_backgroundShape = m_gui->createShapeRectangle( bgrc, gtColor( 1.f, 1.f, 1.f, 1.f ), false );
	}

	m_backgroundRect = bgrc;
}

void gtGUIMenuImpl::render(){
	if( m_backgroundShape ){
		if( m_backgroundShape->isVisible() )
			m_backgroundShape->render();
	}
}

void gtGUIMenuImpl::setTransparent( f32 transparent ){
}

bool        gtGUIMenuImpl::init( s32 h ){
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