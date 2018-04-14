#pragma once
#ifndef __GT_GUI_FONT_IMPL_H__
#define __GT_GUI_FONT_IMPL_H__

namespace gost{

	class gtGUIFontImpl : public gtGUIFont{

		struct character_base{
			character_base():/*c(0),*/texture_id(0u){}
			//char16_t		c;
			gtVector4<u16>	coords;
			s16 texture_id; 
		};
			

		/*struct character{
			character():ch(nullptr){}
			~character(){
				if(ch)
					delete ch;
			}
			character_base * ch;
		};*/

		//gtArray<character*> m_chars;
		gtArray<character_base*> m_chars;

		gtDriver * m_driver;

		gtArray<gtPtr<gtTexture>> m_textureArray;

		bool initFromFile( const gtString& font );
		bool initFromSystem( const gtString& font );

		//gtArray<gtPtr<gtImage>> m_image;

	public:

		gtGUIFontImpl( gtDriver * d );
		~gtGUIFontImpl( void );

		bool init( const gtString& font, gtImage * fromImage );

		void render( void );
		void setOpacity( f32 );

		gtTexture *			getTexture( u32 id );
		u32					getTextureID( char16_t c );
		gtVector4<u16>* 	getRect( char16_t c );

		//void setDriver( gtDriver* d );
	};

}

#endif

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