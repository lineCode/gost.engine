
#pragma once
#ifndef __GT_ARRAY_H__
#define __GT_ARRAY_H__


namespace gost{

		//	Dynamic array
	template<typename type>
	class gtArray{

		typedef type* pointer;
		typedef type& reference;
		typedef const type& const_reference;

		pointer m_data;
		u32     m_size;
		u32     m_allocated;
		u32     m_addMemory;   // При достижении максимума в отведённой памяти, объект выделит дополнительную память размером новый_размер_массива + m_addMemory
		gtAllocator<type> m_allocator;

		void reallocate( u32 new_capacity ){
			new_capacity += m_addMemory;
			pointer new_data = m_allocator.allocate( new_capacity /** sizeof( type )*/ );

			if( m_data ){
				for( u32 i = gtConst0U; i < m_size; ++i ){
					m_allocator.construct( &new_data[i], m_data[i] );
					m_allocator.destruct( &m_data[i] );
				}

				m_allocator.deallocate( m_data );
			}

			m_data = new_data;
			m_allocated = new_capacity;
		}

	public:

		pointer begin(){
			return m_data;
		}

		pointer end(){
			return (m_data+(m_size));
		}

		gtArray( u32 addSize = gtConst8U ):
			m_data( nullptr ),
			m_size( gtConst0U ),
			m_allocated( gtConst0U ),
			m_addMemory( addSize )
		{}

		gtArray( const gtArray& other ):
			m_data( nullptr ),
			m_size( gtConst0U ),
			m_allocated( gtConst0U ),
			m_addMemory( gtConst8U ){
			m_size = other.size();
			m_allocated = other.capacity();
			reallocate( m_allocated );
			memcpy( m_data, other.data(), m_size * sizeof( type ) );
		}

		~gtArray(){ clear(); }
		pointer data() const     { return m_data; }
		void    setSize( u32 s ) { m_size = s; }
		u32     size() const     { return m_size; }
		u32     capacity() const { return m_allocated; }
		
		
			// Если добавление в массив происходит часто, и операция увеличения размера занимает много времени
			// можно увеличить значение m_addMemory, и объект будет выделять больше дополнительной памяти
		void    setAddMemoryValue( u32 v ){ m_addMemory = v; }
		
		
		bool    empty() const    { return m_size == gtConst0U; }

		const_reference at( u32 id ) const { return m_data[id]; }
		reference       at( u32 id ){ return m_data[id]; }
		const_reference operator[]( u32 id ) const { return m_data[id]; }
		reference       operator[]( u32 id ){ return m_data[id]; }
		reference       back(){ return m_data[ m_size - gtConst1U ]; }
		const_reference back() const { return m_data[ m_size - gtConst1U ]; }
		const_reference front() const { return m_data[ gtConst0U ]; }

			//	Allocate memory
		void reserve( u32 new_capacity ){
			if( new_capacity > m_allocated )
				reallocate( new_capacity );
		}

			//	Insert element
		void push_back( type object ){
			u32 new_size = m_size + gtConst1U;
			if( new_size > m_allocated )
				reallocate( new_size );
			m_allocator.construct( &m_data[m_size], object );
			m_size = new_size;
		}

			//	Remove all elements
		void clear(){
			if( m_data ){
				for( u32 i = gtConst0U; i < m_size; ++i )
					m_allocator.destruct( &m_data[i] );
				m_allocator.deallocate( m_data );

				m_allocated = m_size = gtConst0U;
				m_data = nullptr;
			}
		}

			//	Free not used memory
		void shrink_to_fit(){
			reallocate( m_size - m_addMemory );
		}

			//	Remove element
		void erase( u32 index ){
			erase( index, index );
		}

		void erase_first( const_reference ref ){
			for( u32 i = 0u; i < m_size; ++i ){
				if( m_data[ i ] == ref ){
					erase( i );
					return;
				}
			}
		}

			//	Remove elements
		void erase( u32 begin, u32 end ){
			if( m_size ){

				u32 last = m_size - gtConst1U;
				u32 len = end - begin;


				for( u32 i = begin; i < m_size; ++i ){

					m_allocator.destruct( &m_data[ i ] );

					if( i < last ){

						u32 next = i + gtConst1U + len;

						if( next < m_size ){

							m_allocator.construct( &m_data[ i ], m_data[ next ] );

						}
					}

				}

				m_size = m_size - gtConst1U - len;
			}
		}

	//	void sort(){
	//		for( u32 i = gtConst0U; i < m_size; ++i ){
	//		}
	//	}

		gtArray<type>& operator=( const gtArray<type>& other ){
			clear();
			m_size = other.size();
			m_allocated = other.capacity();
			reallocate( m_allocated );
			for( u32 i = gtConst0U; i < m_size; ++i ){
				m_allocator.construct( &m_data[ i ], other.m_data[ i ] );
			}
			return *this;
		}

		/*void pop_back(){
			if( m_size ){
				m_allocator.destruct( &m_data[ m_size - gtConst1U ] );
				--m_size;
			}
		}*/

	};

	namespace util{
		
		// Сортировка слиянием
		template<typename Type> bool predicateGreatOrEqual( const Type& o1, const Type& o2 ){ return o1 >= o2; }
		template<typename Type> bool predicateLessOrEqual( const Type& o1, const Type& o2 ){ return o1 <= o2; }
		template<typename Type> bool predicateGreat( const Type& o1, const Type& o2 ){ return o1 > o2; }
		template<typename Type> bool predicateLess( const Type& o1, const Type& o2 ){ return o1 < o2; }

		template<typename array_type>
		void __merging( s32 low, s32 mid, s32 high, gtArray<array_type>* a, bool(*pred)(const array_type& o1, const array_type& o2 ) ) {
			gtArray<array_type> b;
			b.reserve( high );
			b.setSize( high );

		   s32 l1, l2, i;

			for(l1 = low, l2 = mid + gtConst1, i = low; l1 <= mid && l2 <= high; i++) {
			  //if((*a)[l1] >= (*a)[l2])
				if(pred((*a)[l1],(*a)[l2]))
					b[i] = (*a)[l1++];
				else
					b[i] = (*a)[l2++];
		   }
   
		   while(l1 <= mid)    
			  b[i++] = (*a)[l1++];

		   while(l2 <= high)   
			  b[i++] = (*a)[l2++];

		   for(i = low; i <= high; i++)
			  (*a)[i] = b[i];
		}

		template<typename array_type>
		void __sort(s32 low, s32 high,gtArray<array_type>* arr, bool(*pred)(const array_type& o1, const array_type& o2 )) {
			s32 mid;
	
			if( low < high ){
				mid = (low + high) / gtConst2;
				__sort( low, mid, arr, pred );
				__sort( mid + gtConst1, high, arr, pred );
				__merging( low, mid, high, arr, pred );
			}else{ 
				return;
			}   
		}

		template<typename array_type>
		void mergesort( gtArray<array_type> * array, bool(*pred)(const array_type& o1, const array_type& o2 ) ){
			__sort( gtConst0U, (s32)array->size() - gtConst1, array, pred );
		}
	}
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
