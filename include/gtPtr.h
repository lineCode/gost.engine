#pragma once
#ifndef __GT_PTR_H__
#define __GT_PTR_H__

namespace gost{

	template <typename Type>
	class gtPtrNew{

		Type * m_pointer;
	
	protected:
		gtPtrNew( void ) : m_pointer( nullptr ) {}

	public:

		gtPtrNew( Type* o ) : m_pointer( o ){}

		Type*	data( void ) const {
			return m_pointer;
		}
	};

	template <typename Type>
	class gtPtr{
	
		Type * m_pointer;

	public:
	
		gtPtr( void ) : m_pointer( nullptr) {}

		gtPtr( const gtPtr& ptr ){
			if( ptr.m_pointer ){
				ptr.m_pointer->addRef();
			}
			m_pointer = ptr.m_pointer;
		}
	
		gtPtr( Type* object ){
			if( object ){
				object->addRef();
			}
			m_pointer = object;
		}

		gtPtr( const gtPtrNew<Type>& ptr ){
			m_pointer = ptr.data();
		}

		~gtPtr(){
			if( m_pointer ){
				m_pointer->release();
			}
			m_pointer = nullptr;
		}

		Type* operator->() const {
			return m_pointer;
		}

		void operator=( Type* e ) {
			if( e ){
				e->addRef();
			}
			if( m_pointer ){
				m_pointer->release();
			}
			m_pointer = e;
		}

		void operator=( const gtPtrNew<Type>& rp ){
			if( m_pointer ){
				m_pointer->release();
			}
			m_pointer = rp.data();
		}


		void operator=( const gtPtr& rp ){
			if( rp.m_pointer ){
				rp.m_pointer->addRef();
			}
			if( m_pointer ){
				m_pointer->release();
			}
			m_pointer = rp.m_pointer;
		}

		Type* data( void ) const {
			return m_pointer;
		}

		bool	operator==( const gtPtr& v ){
			return m_pointer == v.m_pointer;
		}
	
	};

}

#endif

/*
Copyright (c) 2017-2018 532235

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