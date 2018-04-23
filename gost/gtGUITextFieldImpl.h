#pragma once
#ifndef __GT_GUI_TEXT_FIELD_IMPL_H__
#define __GT_GUI_TEXT_FIELD_IMPL_H__

namespace gost{

	class gtGUITextFieldImpl : public gtGUITextField{
		gtDriver *		m_driver;
		gtMainSystem *	m_mainSystem;
		gtModelSystem*	m_modelSystem;
		gtGUISystem*	m_gui;

		gtGUIFontImpl * m_font;
		bool			m_fixedH, m_fixedW;
		bool			m_showBackground;
		v4i				m_rect;
		gtColor			m_bgColor;
		gtString		m_text;

		gtPtr<gtGUIShape>			m_backgroundShape;
		gtArray<gtGUIStaticText*>	m_textWords;

		void update( void );

	public:

		gtGUITextFieldImpl( gtDriver* d );
		virtual ~gtGUITextFieldImpl( void );

		bool		init( const v4i& rect, gtGUIFont* font, bool fh, bool fw );
		
		void		setFont( gtGUIFont * font );
		void		setFont( const gtPtr<gtGUIFont>& font );
		void		setText( const gtString& text );
		void		setTextColor( const gtColor& color );
		void		setBackgroundVisible( bool value );
		void		setBackgroundColor( const gtColor& color );
		void		clear( void );
		gtGUIShape* getBackgroundShape( void );

		void		render( void );
		void		setOpacity( f32 opacity = 1.f );
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