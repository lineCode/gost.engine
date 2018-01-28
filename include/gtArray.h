/*!	GOST
	\file gtArray.h
	\brief array
*/

#pragma once
#ifndef __GT_ARRAY_H__
#define __GT_ARRAY_H__


namespace gost{

	template<typename type>
	class gtArray{

		typedef type* pointer;
		typedef type& reference;
		typedef const type& const_reference;

		pointer m_data;
		
		u32 m_size;
		u32 m_allocated;

		u32 m_addMemory;

		gtAllocator<type> m_allocator;

		void reallocate( u32 new_capacity ){

			new_capacity += m_addMemory;

			pointer new_data = m_allocator.allocate( new_capacity );

			if( m_data ){
					
				for( u32 i = 0u; i < m_size; ++i ){

					m_allocator.construct( &new_data[i], m_data[i] );

					m_allocator.destruct( &m_data[i] );
				}

				m_allocator.deallocate( m_data );
			}

			m_data = new_data;

			m_allocated = new_capacity;
		}

	public:
	
		gtArray( u32 addSize = 8u ):
			m_data( nullptr ),
			m_size( 0u ),
			m_allocated( 0u ),
			m_addMemory( addSize )
		{}

		~gtArray( void ){
			clear();
		}

		pointer data( void ) const {
			return m_data;
		}
	
		u32 size( void ) const {
			return m_size;
		}

		u32 capacity( void ) const {
			return m_allocated;
		}

		bool empty( void ) const {
			return m_size == 0u;
		}

		const_reference at( u32 id ) const {
			GT_ASSERT2(id<m_size,"id>=m_size");
			return m_data[id];
		}

		reference at( u32 id ){
			GT_ASSERT2(id<m_size,"id>=m_size");
			return m_data[id];
		}

		const_reference operator[]( u32 id ) const {
			GT_ASSERT2(id<m_size,"id>=m_size");
			return m_data[id];
		}

		reference operator[]( u32 id ){
			GT_ASSERT2(id<m_size,"id>=m_size");
			return m_data[id];
		}

		reference back( void ){
			GT_ASSERT3(m_size);
			return m_data[ m_size - 1u ];
		}

		const_reference back( void ) const {
			GT_ASSERT3(m_size);
			return m_data[ m_size - 1u ];
		}

		const_reference front( void ) const {
			GT_ASSERT3(m_size);
			return m_data[ 0u ];
		}

		void reserve( u32 new_capacity ){
			if( new_capacity > m_allocated )
				reallocate( new_capacity );
		}

		void push_back( type object ){
			u32 new_size = m_size + 1u;
			if( new_size > m_allocated )
				reallocate( new_size );
			m_allocator.construct( &m_data[m_size], object );
			m_size = new_size;
		}
	
		void clear( void ){
			if( m_data ){
				for( u32 i = 0u; i < m_size; ++i )
					m_allocator.destruct( &m_data[i] );
				m_allocator.deallocate( m_data );
				m_allocated = m_size = 0u;
				m_data = nullptr;
			}
		}

		void shrink_to_fit( void ){
			reallocate( m_size - m_addMemory );
		}

		void erase( u32 index ){
			erase( index, index );
		}

		void erase( u32 begin, u32 end ){
			GT_ASSERT3(begin<m_size);
			GT_ASSERT3(end<m_size);
			GT_ASSERT3(begin<=end);

			if( m_size ){

				u32 last = m_size - 1u;
				u32 len = end - begin;

				
				for( u32 i = begin; i < m_size; ++i ){
				
					m_allocator.destruct( &m_data[ i ] );

					if( i < last ){
					
						u32 next = i + 1u + len;

						if( next < m_size ){

							m_allocator.construct( &m_data[ i ], m_data[ next ] );
							m_allocator.destruct( &m_data[ next ] );

						}
					}

				}

				m_size = m_size - 1u - len;
			}
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