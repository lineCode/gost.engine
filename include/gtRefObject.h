/**	GOST
	\file gtRefObject.h
	\brief Reference counter for gtBaseObject
*/

#pragma once
#ifndef __GT_REF_OBJECT_H__
#define __GT_REF_OBJECT_H__ ///< include guard

namespace gost{
	 
		///	Все объекты наследуют этот класс.
	class gtRefObject : public gtBaseObject{
	
			///	счётчик
		mutable u32 m_count;
	
	public:

			///	конструктор
		gtRefObject( void ) : m_count( 1U ){}

			///	деструктор
		virtual ~gtRefObject( void ){}

			///	получить количество ссылающихся на этот объект
			///	\return Количество ссылающихся на этот объект
		u32 getReferenceCount( void ) const {
			return m_count;
		}

			/// Если объект нужно передать кому нибудь ещё, нужно вызвать это. Это увеличит счётчик на 1.
		void addRef( void ) const {
			++m_count;
		}

			///	Если объект был создан вручную то при вызове этого метода он удалится, если ранее не было вызвано addRef.
			///	Если ранее был вызван addRef то вызов release уменьшит счётчик на 1
			///	Удалять объекты движка нужно лишь те, которые были созданы вручную либо вызовом методов с именем create
		void release( void ) const {
			--m_count;
			if( !m_count ){
				delete this;
			}
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