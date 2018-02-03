/**	GOST
	\file gtPtr.h
	\brief Pointer class for gtRefObject
*/

#pragma once
#ifndef __GT_PTR_H__
#define __GT_PTR_H__ ///< include guard

namespace gost{

		///	Через этот класс передаётся указатель объекта в gtPtr	
		///	Нужно использовать так (для удобства создан макрос \ref gtPtr_t):
		/// \code {.cpp}
		///	gtPtr<type> object( gtPtrNew<type>(new type(params)) );
		///	\endcode
	template <typename Type>
	class gtPtrNew{

		Type * m_pointer; /// указатель
	
	protected:
		gtPtrNew( void ) : m_pointer( nullptr ) {} /// запрет создания пустого объекта

	public:

			///	Создаёт объект принимая указатель
		gtPtrNew( Type* o ) : m_pointer( o ){}

			///	Возвратит указатель
			///	\return Указатель который хранит
		Type*	data( void ) const {
			return m_pointer;
		}
	};

		///	Простая оболочка к указателю
	template <typename Type>
	class gtPtr{
	
			///	Указатель
		Type * m_pointer;

	public:
	
			/// Не уверен что он нужен
		gtPtr( void ) : m_pointer( nullptr) {}

			///	добавит указатель с типом имеющим метод addRef
		gtPtr( const gtPtr& ptr ){
			if( ptr.m_pointer ){
				ptr.m_pointer->addRef();
			}
			m_pointer = ptr.m_pointer;
		}
	
			///	ненужно делать так gtPtr<чтото> (new gtИмя);
			///	для создания новых объектов есть gtPtrNew
		gtPtr( Type* object ){
			if( object ){
				object->addRef();
			}
			m_pointer = object;
		}

			///	просто передаёт указатель, не вызывая addRef
		gtPtr( const gtPtrNew<Type>& ptr ){
			m_pointer = ptr.data();
		}

			///	автоматическое удаление
		~gtPtr(){
			if( m_pointer ){
				m_pointer->release();
			}
			m_pointer = nullptr;
		}

			/// Get ptr
			///	\return Указатель который хранит
		Type* operator->() const {
			return m_pointer;
		}

			///	присваивание
		void operator=( Type* e ) {
			if( e ){
				e->addRef();
			}
			if( m_pointer ){
				m_pointer->release();
			}
			m_pointer = e;
		}

			///	присваивание
		void operator=( const gtPtrNew<Type>& rp ){
			if( m_pointer ){
				m_pointer->release();
			}
			m_pointer = rp.data();
		}


			///	присваивание
		void operator=( const gtPtr& rp ){
			if( rp.m_pointer ){
				rp.m_pointer->addRef();
			}
			if( m_pointer ){
				m_pointer->release();
			}
			m_pointer = rp.m_pointer;
		}

			/// Get ptr
			///	\return Указатель который хранит
		Type* data( void ) const {
			return m_pointer;
		}

			///	сравнение указателей
			///	\return \c true если указатели равны
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