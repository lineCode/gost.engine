#pragma once
#ifndef __GT_WINDOW_INFO_H__
#define __GT_WINDOW_INFO_H__ 

namespace gost{

	class gtWindow;

	struct gtWindowInfo{

			// c-tor
		gtWindowInfo():
		m_owner(nullptr){
			m_title.assign( u"GoST window" );
			m_rect.set( 0, 0, 800, 600 );
			m_style = 0u;
			m_state = 0u;
		}
		
		gtString	m_title;

		v4i			m_rect;
		
		gtWindow*	m_owner;
		v2i			m_borderSize;
		
		enum style{
			style_standart,
			style_popup    = BIT(1),
			style_resize   = BIT(2),
			style_maximize = BIT(3),
			style_center   = BIT(4)
		};

		enum state{
			state_normal,
			state_maximized = BIT(1),
			state_minimized = BIT(2)
		};

		u32 m_style;
		u32 m_state;

		bool	operator==( const gtWindowInfo& wi ){
			if( m_title == wi.m_title )
				if( m_rect == wi.m_rect )
					if( m_style == m_style )
						return true;

			return false;
		}
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