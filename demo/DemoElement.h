#pragma once
#ifndef __GT_DEMO_ELEMENT_H__
#define __GT_DEMO_ELEMENT_H__

namespace demo{

	class DemoElement{

		bool			m_isExample;
		DemoExample*	m_example;
		gtString		m_title;
		gtString		m_description;

	public:

		DemoElement():
			m_isExample( false ),
			m_example( nullptr )
		{}

		DemoElement( const gtString& title, const gtString& desc, 
			bool isExample = false, DemoExample * example = nullptr ):
			m_isExample( isExample ),
			m_example( example ),
			m_title( title ),
			m_description( desc )
		{}

		void Run( void ){
			m_example->Run();
		}

		const gtString& GetTitle( void ) const {
			return m_title;
		}

		const gtString& GetDesc( void ) const {
			return m_description;
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