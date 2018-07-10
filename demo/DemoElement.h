#pragma once
#ifndef __GT_DEMO_ELEMENT_H__
#define __GT_DEMO_ELEMENT_H__

namespace demo{

	class DemoApplication;
	class DemoElement{

		bool			   m_isExample;
		DemoExample*       m_example;
		DemoExampleProxy*  m_exampleProxy;
		gtString		   m_title, m_tid;
		gtString		   m_description, m_did;


	public:

		DemoElement():
			m_isExample( false ),
			m_example( nullptr ),
			m_exampleProxy( nullptr )
		{}

		DemoElement( const gtString& title, const gtString& desc, 
			bool isExample = false, 
			DemoExampleProxy * example = nullptr ):
			m_isExample( isExample ),
			m_exampleProxy( example ),
			m_example( nullptr ),
			m_tid( title ),
			m_did( desc )
		{
		
		}

		virtual ~DemoElement(){}

		bool Init( DemoApplication * app ){
			if( m_isExample ){
				m_example = m_exampleProxy->allocate( app );
				if( m_example )
					return m_example->Init();
			}
			return false;
		}
		 
		void Update(){
			m_example->Update();
		}

		void Input( f32 delta ){
			m_example->Input( delta );
		}

		void Render(){
			m_example->Render();
		}

		void Shutdown(){
			m_example->Shutdown();
			delete m_example;
			m_example = nullptr;
		}

		void Render2D(){
			m_example->Render2D();
		}

		const gtString& GetTitle() const {
			return m_title;
		}

		const gtString& GetDesc() const {
			return m_description;
		}

		const gtString& GetTitleID() const {
			return m_tid;
		}

		const gtString& GetDescID() const {
			return m_did;
		}

		void SetTitle( const gtString& title ){
			m_title = title;
		}

		void SetDesc( const gtString& desc ){
			m_description = desc;
		}


		bool isDemo() const {
			return m_isExample;
		}

		void clear(){
			if( m_example )
				delete m_example;
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