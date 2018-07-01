#include "common.h"

gtGUIFontImpl::gtGUIFontImpl( gtGraphicsSystem * d ):
m_gs( d ),
m_width(gtConst0U),
m_height(gtConst0U){
	m_type = gtGUIObjectType::Font;
}

gtGUIFontImpl::~gtGUIFontImpl(){
	u32 sz = m_chars.size();
	for( u32 i = gtConst0U; i < sz; ++i ){

		if( m_chars[ i ] ){
			delete m_chars[ i ];
		}
	}
}

void gtGUIFontImpl::render(){}
void gtGUIFontImpl::setOpacity( f32 ){}
f32  gtGUIFontImpl::getOpacity(){return 1.f;}

gtVector4<u16>* gtGUIFontImpl::getRect( char16_t c ){
	if( m_chars[ (u16)c ] )
			return &m_chars[ (u16)c ]->coords;
	return nullptr;
}

u32 gtGUIFontImpl::getTextureID( char16_t c ){
	if( m_chars[ (u16)c ] )
			return m_chars[ (u16)c ]->texture_id;
	return gtConst0U;
}

gtTexture * gtGUIFontImpl::getTexture( u32 id ){
	if( id < m_textureArray.size() ){
		return m_textureArray[ id ].data();
	}
	return nullptr;
}

bool gtGUIFontImpl::init( const gtString& font, gtImage * image ){
	if( image ){
		
		auto xml = gtFileSystem::XMLRead( font );
		if( !xml.data() ){
			gtLogWriter::printWarning( u"Can not read XML string" );
			return false;
		}

		image->makeAlphaFromBlack();
		

		if( m_gs ){
			auto texure = m_gs->createTexture( image, gtTextureFilterType::Anisotropic );
			if( !texure.data() ){
				gtLogWriter::printWarning( u"Can not texture for font" );
				return false;
			}
			m_textureArray.push_back( texure );
		}

		m_chars.reserve( 0xffff );
		for( u32 i = gtConst0U; i < 65535; ++i ){
			m_chars.push_back( nullptr );
		}
	
		auto arr_nodes = xml->selectNodes( u"/font/c" );
		if( !arr_nodes.size() ){
			gtLogWriter::printWarning( u"Can not get nodes from XML document" );
			return false;
		}

		auto sz = arr_nodes.size();

		for( u32 i = gtConst0U; i < sz; ++i ){

			gtXMLNode * n = arr_nodes[ i ];

			if( n->attributeList.size() ){
				gtXMLAttribute * a = n->getAttribute( u"c" );
				if( a ){

					u32 val = (u32)a->value[ gtConst0U ];
					if( val >= 0xffff ) continue;

					m_chars[ val ] = new character_base;

					a = n->getAttribute( u"r" );
					if( a ){
						util::getVec4iFromString( a->value, &m_chars[ val ]->coords );
						u32 w = m_chars[ val ]->coords.getWidth();
						u32 h = m_chars[ val ]->coords.getHeight();
						if( w > m_width ) m_width = w;
						if( h > m_height ) m_height = h;
					}

					a = n->getAttribute( u"i" );
					if( a ){
						m_chars[ val ]->texture_id = (s16)util::getIntFromString<s32>( a->value );
					}else{
						m_chars[ val ]->texture_id = 0;
					}

				}
			}
		}

		char16_t tab = u'\t';
		if( !m_chars[ tab ] ){
			m_chars[ tab ] = new character_base;
		}

		char16_t newline = u'\n';
		if( !m_chars[ newline ] ){
			m_chars[ newline ] = new character_base;
		}

		return true;
	}else{
		gtString filePath = gtFileSystem::getRealPath( font );

		if( gtFileSystem::existFile( filePath ) ){
			return initFromFile( filePath );
		}

		return initFromSystem( font );
	}
}

bool gtGUIFontImpl::initFromFile( const gtString& font ){

	gtString filePath = font;
	gtString folderPath = filePath;
	util::stringPopBackBefore( folderPath, '/' );
	
	auto xml = gtFileSystem::XMLRead( filePath );
	if( !xml.data() ){
		gtLogWriter::printWarning( u"Can not read XML file." );
		return false;
	}

	gtArray<gtXMLNode*> arr_nodes = xml->selectNodes( u"/font/Texture" );
	if( !arr_nodes.size() ){
		gtLogWriter::printWarning( u"Can not get nodes from XML document" );
		return false;
	}

	u32 sz = arr_nodes.size();
	gtString textureFilePath;
	for( u32 i = gtConst0U; i < sz; ++i ){
		textureFilePath = folderPath;
		gtXMLNode * node = arr_nodes[ i ];
		u32 asz = node->attributeList.size();
		if( !asz ){
			gtLogWriter::printWarning( u"Wrong XML file. Can not find attributes" );
			return false;
		}
		for( u32 o = gtConst0U; o < asz; ++o ){
			gtXMLAttribute * att = node->attributeList[ o ];
			if( !att ){
				gtLogWriter::printWarning( u"gtXMLAttribute == nullptr" );
				return false;
			}
			if( att->name != u"index" && 
				att->name != u"filename" &&
				att->name != u"hasAlpha" ){
				gtLogWriter::printWarning( u"Wrong XML file. Can not find attributes" );
				return false;
			}
		}

		for( u32 o = gtConst0U; o < asz; ++o ){
			gtXMLAttribute * att = node->attributeList[ o ];
			if( att->name == u"filename" ){
				if( !att->value.size() ){
					gtLogWriter::printWarning( u"Wrong XML file." );
					return false;
				}
				textureFilePath += att->value;
				if( !gtFileSystem::existFile(textureFilePath)){
					gtLogWriter::printWarning( u"File [%s] not exist.", textureFilePath.data() );
					return false;
				}

				auto image = gtMainSystem::getInstance()->loadImage( textureFilePath );
				
				if( image->format == gtImageFormat::R8G8B8 ){
					image->convert( gtImageFormat::R8G8B8A8 );
					image->makeAlphaFromBlack();
				}

				

				auto texure = m_gs->createTexture( image.data(), gtTextureFilterType::Anisotropic );
				if( !texure.data() ){
					gtLogWriter::printWarning( u"Can not texture for font." );
					return false;
				}

				m_textureArray.push_back( texure );
			}
		}
	}


	m_chars.reserve( 0xffff );
	for( u32 i = gtConst0U; i < 65535; ++i ){
		m_chars.push_back( nullptr );
	}
	
	arr_nodes = xml->selectNodes( u"/font/c" );
	if( !arr_nodes.size() ){
		gtLogWriter::printWarning( u"Can not get nodes from XML document" );
		return false;
	}

	sz = arr_nodes.size();

	for( u32 i = gtConst0U; i < sz; ++i ){

		gtXMLNode * n = arr_nodes[ i ];

		if( n->attributeList.size() ){
			gtXMLAttribute * a = n->getAttribute( u"c" );
			if( a ){

				u32 val = (u32)a->value[ gtConst0U ];
				if( val >= 0xffff ) continue;

				m_chars[ val ] = new character_base;

				a = n->getAttribute( u"r" );
				if( a ){
					util::getVec4iFromString( a->value, &m_chars[ val ]->coords );
					u32 w = m_chars[ val ]->coords.getWidth();
					u32 h = m_chars[ val ]->coords.getHeight();
					if( w > m_width ) m_width = w;
					if( h > m_height ) m_height = h;
				}

				a = n->getAttribute( u"i" );
				if( a ){
					m_chars[ val ]->texture_id = (s16)util::getIntFromString<s32>( a->value );
				}else{
					m_chars[ val ]->texture_id = 0;
				}

			}
		}
	}

	char16_t tab = u'\t';
	if( !m_chars[ tab ] ){
		m_chars[ tab ] = new character_base;
	}

	char16_t newline = u'\n';
	if( !m_chars[ newline ] ){
		m_chars[ newline ] = new character_base;
	}

	//.....

	return true;
}

bool gtGUIFontImpl::initFromSystem( const gtString& /*font*/ ){

	return true;
}

u32 gtGUIFontImpl::getWidth(){
	return m_width;
}

u32 gtGUIFontImpl::getHeight(){
	return m_height;
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