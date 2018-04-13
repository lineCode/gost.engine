#pragma once
#ifndef __GT_GUI_STATIC_TEXT_IMPL_H__
#define __GT_GUI_STATIC_TEXT_IMPL_H__

namespace gost{

	class gtGUIStaticTextImpl : public gtGUIStaticText{

		struct bufferInfo{
			bufferInfo(u32 tid,gtSubModel*sm):texture_id(tid),sub(sm){}
			u32 texture_id;
			gtSubModel* sub;
		};
		 
		gtArray<bufferInfo> m_bufferInfo;

		gtString m_text;
		gtGUIFontImpl * m_font;

		gtArray<gtRenderModel*> m_buffers; //то что идёт на отрисовку. Зависит от используемого символа и количества текстур для gtGUIFont

		gtMainSystem * m_mainSystem;
		gtModelSystem* m_modelSystem;
		gtDriver*	   m_driver;	

		gtMaterial		m_material;

		v2i m_position;

		f32 m_length;

		void checkFont( void );

		void updateMaterial( void );

	public:

		gtGUIStaticTextImpl( gtDriver* d );
		~gtGUIStaticTextImpl( void );
		bool init( const gtString& text, s32 positionX, s32 positionY, gtGUIFont* font );



		void setFont( gtGUIFont * font );
		void setFont( const gtPtr<gtGUIFont>& font );

		void setText( const gtString& text );
		void setColor( const gtColor& color );

		f32  getLength( void );

		void clear( void );


		void render( void );

		

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