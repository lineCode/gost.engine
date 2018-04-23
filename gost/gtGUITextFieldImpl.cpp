#include "common.h"

gtGUITextFieldImpl::gtGUITextFieldImpl( gtDriver* d ):
	m_driver(nullptr),
	m_font(nullptr),
	m_fixedH(false),
	m_fixedW(false),
	m_showBackground(true){
	m_type = gtGUIObjectType::TextField;
	m_mainSystem = gtMainSystem::getInstance();
	m_driver = m_mainSystem->getMainVideoDriver();
	m_modelSystem= m_mainSystem->getModelSystem();
	m_gui = m_mainSystem->getGUISystem( d );
}

gtGUITextFieldImpl::~gtGUITextFieldImpl( void ){
}

bool gtGUITextFieldImpl::init( const v4i& rect, gtGUIFont* font, bool fh, bool fw ){
	m_rect   = rect;

	if( m_rect.z < m_rect.x ) m_rect.z = m_rect.x + 50;
	if( m_rect.w < m_rect.y ) m_rect.w = m_rect.y + 20;

	m_font   = (gtGUIFontImpl*)font;
	m_fixedH = fh;
	m_fixedW = fw;

	update();

	return true;
}

void gtGUITextFieldImpl::setFont( gtGUIFont * font ){
	m_font = (gtGUIFontImpl*)font;
	update();
}

void gtGUITextFieldImpl::setFont( const gtPtr<gtGUIFont>& font ){
	m_font = (gtGUIFontImpl*)font.data();
	update();
}

void gtGUITextFieldImpl::setText( const gtString& text ){
	m_text = text;
	update();
}

void gtGUITextFieldImpl::setTextColor( const gtColor& color ){
	u32 sz = m_textWords.size();
	for( u32 i = 0u; i < sz; ++i ){
		m_textWords[ i ]->setColor( color );
	}
}

void gtGUITextFieldImpl::setBackgroundVisible( bool value ){
	m_showBackground = value;
}

void gtGUITextFieldImpl::setBackgroundColor( const gtColor& color ){
	m_bgColor = color;
	m_backgroundShape->setColor( color );
}

void gtGUITextFieldImpl::clear( void ){
	u32 sz = m_textWords.size();
	for( u32 i = 0u; i < sz; ++i ){
		m_textWords[ i ]->release();
	}
	m_textWords.clear();
}

gtGUIShape* gtGUITextFieldImpl::getBackgroundShape( void ){
	return m_backgroundShape.data();
}

void gtGUITextFieldImpl::render( void ){
	if( m_visible ){
		if( m_showBackground )
			m_backgroundShape->render();
	}
}

void gtGUITextFieldImpl::setOpacity( f32 opacity ){

}

void gtGUITextFieldImpl::update( void ){

	//clear();



	gtTexture * t1 = nullptr;
	gtColor color;

	if( m_backgroundShape.data() ){
		t1 = m_backgroundShape->getTexture();
	}

	m_backgroundShape = m_gui->createShapeRectangle( m_rect, gtColor( 0.f, 0.f, 0.f, 1.f ) );
	m_backgroundShape->setTexture( t1 );
	m_backgroundShape->setColor( m_bgColor );
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