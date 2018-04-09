#include "common.h"

gtGUISystemImpl::gtGUISystemImpl( void ):
m_driver( nullptr ){

#ifdef GT_DEBUG
	m_debugName.assign(u"gtGUISystem");
#endif

}

gtGUISystemImpl::~gtGUISystemImpl( void ){
}

void gtGUISystemImpl::setCurrentRenderDriver( gtDriver * driver ){
	m_driver = driver;
}

gtPtr<gtGUIFont> gtGUISystemImpl::createFont( const gtString& fontName ){

	gtPtr_t( gtGUIFontImpl, font, new gtGUIFontImpl( m_driver ) );

	if( !font.data() )
		return nullptr;

	if( !font->init( fontName ) ){
		gtLogWriter::printWarning( u"Can not create font \"%s\"", fontName.c_str() );
		return nullptr;
	}

	//font->addRef();

	return gtPtr<gtGUIFont>( font.data() );
}

gtPtr<gtGUIStaticText> gtGUISystemImpl::createStaticText( const gtString& text, s32 positionX, s32 positionY ){
	gtPtr_t( gtGUIStaticTextImpl, st, new gtGUIStaticTextImpl( m_driver ) );

	if( !st.data() )
		return nullptr;

	if( !st->init( text, positionX, positionY ) ){
		gtLogWriter::printWarning( u"Can not create static text " );
		return nullptr;
	}

//	st->addRef();

	return gtPtr<gtGUIStaticText>( st.data() );
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