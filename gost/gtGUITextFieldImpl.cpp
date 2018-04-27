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
	clear();
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

const gtString& gtGUITextFieldImpl::getText( void ){
	return m_text;
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

		m_driver->scissorClear( false );
		m_driver->scissorAdd( m_scissorRect );

		if( m_showBackground )
			m_backgroundShape->render();

		u32 sz = m_textWords.size();
		for( u32 i = 0u; i < sz; ++i ){
			m_textWords[ i ]->render();
		}

		m_driver->scissorClear();
	}
}

void gtGUITextFieldImpl::setOpacity( f32 opacity ){
	m_material.opacity = opacity;

	u32 sz = m_textWords.size();
	for( u32 i = 0u; i < sz; ++i ){
		m_textWords[ i ]->setOpacity(opacity);
	}
	m_backgroundShape->setOpacity(opacity);
}

f32 gtGUITextFieldImpl::getOpacity( void ){
	return m_material.opacity;
}

void gtGUITextFieldImpl::update( void ){

	clear();

	gtArray<gtString> words;
	util::stringGetWords<char16_t>( &words, m_text, true, true, true );
	
	u32 v = 0u;
	
	u32 position_x = m_rect.x + 3u;

	u32 sz = words.size();
	for( u32 i = 0; i < sz; ++i ){
		if( words[ i ] == u" " ){
			position_x += 7u;
		}else if( words[ i ] == u"\t" ){
			position_x += 14u;
		}else if( words[ i ] == u"\n" ){
			position_x = m_rect.x + 14u;
			v += m_font->getHeight() + 3u;
		}else{

			auto word = m_gui->createStaticText( words[ i ], position_x, m_rect.y + m_font->getHeight() + v, m_font );
			word->addRef();
			word->setBackgroundVisible( false );

			position_x += word->getLength();

			if( m_fixedW ){
				if( position_x > m_rect.z + 3u ){
					position_x = m_rect.x + 3u;

					v += m_font->getHeight() + 3u;

					word->setPosition( v2i( position_x, m_rect.y + m_font->getHeight() + v ) );
				
					position_x += word->getLength();
				}
			}else{
				m_rect.z = position_x + 4u;
			}

			m_textWords.push_back( word.data() );
		}
	}

	if( !m_fixedH ){
		if( m_font->getHeight() + v >= m_rect.getHeight() ){
			m_rect.w = m_rect.y + m_font->getHeight() + v;
		}
	}


	gtTexture * t1 = nullptr;
	f32 bgop = 1.f;
	gtColor color;

	if( m_backgroundShape.data() ){
		t1 = m_backgroundShape->getTexture();
		bgop = m_backgroundShape->getOpacity();
	}

	auto rc = m_driver->getParams().m_outWindow->getRect();
	auto wndH = rc.getHeight();
	auto wndW = rc.getWidth();

	auto params = m_driver->getParams();
	auto bbsz = params.m_outWindow->getRect();

	f32 mulx = ((f32)m_driver->getParams().m_backBufferSize.x / (f32)wndW);
	f32 muly = ((f32)m_driver->getParams().m_backBufferSize.y / (f32)wndH);

	m_scissorRect.x = m_rect.x * mulx;
	m_scissorRect.y = m_rect.y * muly;
	m_scissorRect.z = m_rect.z * mulx;
	m_scissorRect.w = m_rect.w * muly;

	m_backgroundShape = m_gui->createShapeRectangle( m_rect, gtColor( 0.f, 0.f, 0.f, 1.f ) );
	m_backgroundShape->setTexture( t1 );
	m_backgroundShape->setColor( m_bgColor );
	m_backgroundShape->setOpacity( bgop );
}

const v4i&	gtGUITextFieldImpl::getRect( void ){
	return m_rect;
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