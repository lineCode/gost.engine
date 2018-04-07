#include "common.h"

gtGUIStaticTextImpl::gtGUIStaticTextImpl( void ):
	m_font( nullptr ),
	m_mainSystem( nullptr ),
	m_modelSystem( nullptr )
{
	m_type = gtGUIObjectType::text;
	m_mainSystem = gtMainSystem::getInstance();
	m_modelSystem= m_mainSystem->getModelSystem();
}

gtGUIStaticTextImpl::~gtGUIStaticTextImpl( void ){
}

void gtGUIStaticTextImpl::setFont( gtGUIFont * font ){
	m_font = (gtGUIFontImpl*)font;
	setText( m_text );
}

void gtGUIStaticTextImpl::setFont( const gtPtr<gtGUIFont>& font ){
	m_font = (gtGUIFontImpl*)font.data();
	setText( m_text );
}

bool gtGUIStaticTextImpl::init( const gtString& text, s32 positionX, s32 positionY ){
	setText( text );
	return true;
}

void gtGUIStaticTextImpl::setText( const gtString& text ){
	m_text = text;
	if( m_font ){

		if( m_text.size() ){
			//auto soft = m_modelSystem->createPlane( 0.2f, 0.2f, gtSide::BACK );
		}

	//	m_font->

	}
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