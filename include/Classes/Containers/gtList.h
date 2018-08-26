#pragma once
#ifndef __GT_LIST_H__
#define __GT_LIST_H__

namespace gost{


	template<typename node_type>
	class gtListNode{
	public:

		typedef gtListNode			this_type;
		typedef this_type*			this_type_ptr;
		typedef const node_type&	const_reference;

		this_type_ptr	m_next;
		this_type_ptr	m_prev;
		node_type		m_object;

		gtListNode( const_reference object ):
			m_next( nullptr ),
			m_prev( nullptr ),
			m_object( object )
		{}

	};

	template<typename element>
	class gtList{

		using const_reference = const element&;
		using node            = gtListNode<element>;
		using node_ptr        = gtListNode<element>*;

		node_ptr m_first;
		node_ptr m_last;

		gtAllocator<node> m_allocator;

		u32 m_size;

	public:
		gtList():
			m_first( nullptr ),
			m_last( nullptr ),
			m_size( 0u ){}
		~gtList(){ clear();}

		class const_iterator;
		class iterator{//irrlicht
		public:
			node_ptr curr;
			iterator():curr( nullptr ){}
			iterator( node_ptr p ):curr( p ){}

			iterator&	operator++(){ curr = curr->m_next; return *this; }
			iterator&	operator--(){ curr = curr->m_prev; return *this; }
			iterator	operator++( s32 ){ iterator tmp = *this; curr = curr->m_next; return tmp; }
			iterator	operator--( s32 ){ iterator tmp = *this; curr = curr->m_prev; return tmp; }
			iterator&	operator+=( s32 num ){
				if( num > 0 ){
					while( num-- && this->curr != 0 ) ++(*this);
				}else{
					while( num++ && this->curr != 0 ) --(*this);
				}
				return *this;
			}
			iterator  operator+( s32 num ) const { iterator tmp = *this; return tmp += num; }
			iterator& operator-=( s32 num ) { return (*this)+=(-num); }
			iterator  operator-( s32 num ) const { return (*this)+ (-num); }

			element&	operator*(){ return curr->m_object; }
			element*	operator->(){ return &curr->m_object; }

			bool operator==( const iterator& b ) const { return curr == b.curr; }
			bool operator!=( const iterator& b ) const { return curr != b.curr; }
			bool operator==( const const_iterator& b ) const { return curr == b.curr; }
			bool operator!=( const const_iterator& b ) const { return curr != b.curr; }

		};

		class const_iterator{//irrlicht
		public:

			node_ptr curr;

			const_iterator(): curr( nullptr ){}
			const_iterator( const const_iterator& it ): curr( it.curr ){}

			const_iterator&	operator++(){ curr = curr->m_next; return *this; }
			const_iterator&	operator--(){ curr = curr->m_prev; return *this; }
			const_iterator	operator++( s32 ){ const_iterator tmp = *this; curr = curr->m_next; return tmp; }
			const_iterator	operator--( s32 ){ const_iterator tmp = *this; curr = curr->m_prev; return tmp; }
			const_iterator&	operator+=( s32 num ){
				if( num > 0 ){
					while( num-- && this->curr != 0 ) ++(*this);
				}else{
					while( num++ && this->curr != 0 ) --(*this);
				}
				return *this;
			}
			const_iterator  operator+( s32 num ) const { const_iterator tmp = *this; return tmp += num; }
			const_iterator& operator-=( s32 num ) { return (*this)+=(-num); }
			const_iterator  operator-( s32 num ) const { return (*this)+ (-num); }

			const element&	operator*(){ return curr->m_object; }
			const element*	operator->(){ return &curr->m_object; }

			bool operator==( const const_iterator& b ) const { return curr == b.curr; }
			bool operator!=( const const_iterator& b ) const { return curr != b.curr; }
			bool operator==( const iterator& b ) const { return curr == b.curr; }
			bool operator!=( const iterator& b ) const { return curr != b.curr; }

			const_iterator& operator=( const iterator & it ){ curr = it.curr; return *this; }
		};

		iterator	begin(){
			return iterator(m_first);
		}
		iterator	end(){
			return iterator( nullptr );
		}
		const_iterator	begin() const {
			return const_iterator(m_first);
		}
		const_iterator	end() const {
			return const_iterator( nullptr );
		}



		void clear(){
			while( m_first ){
				node_ptr next = m_first->m_next;
				m_allocator.destruct( m_first );
				m_allocator.deallocate( m_first );
				m_first = next;
			}
			m_size = 0u;
			m_last = nullptr;
		}

		void	push_back( const_reference object ){

			node_ptr new_node = m_allocator.allocate( 1u );
			m_allocator.construct( new_node, object );

			
			new_node->m_prev = m_last;
			
			if( m_last )
				m_last->m_next = new_node;

			if( !m_first )
				m_first = new_node;

			m_last = new_node;
			++m_size;
		}

			// Insert before
			/**
				\code
					//Insert in m_first
					list.insert( list.begin(), val );

					//Insert after m_first
					list.insert( list.begin() + 1, val );
				\endcode
			*/
		void insert( const iterator& it, const_reference object ){
			node_ptr new_node = m_allocator.allocate( 1u );
			m_allocator.construct( new_node, object );

			new_node->m_prev = it.curr->m_prev;

			if( it.curr->m_prev )
				it.curr->m_prev->m_next = new_node;

			new_node->m_next = it.curr;
			it.curr->m_prev = new_node;
			++m_size;

			if( it.curr == m_first )
				m_first = new_node;
		}

		void push_front( const_reference object ){
			insert( begin(), object );
		}

		iterator erase( iterator& where ){

			iterator return_iterator( where );
			++return_iterator;

			(where.curr == m_first) ? m_first = where.curr->m_next : where.curr->m_prev->m_next = where.curr->m_next;
			(where.curr == m_last) ? m_last = where.curr->m_prev : where.curr->m_next->m_prev = where.curr->m_prev;

			m_allocator.destruct( where.curr );
			m_allocator.deallocate( where.curr );
			where.curr = nullptr;

			--m_size;

			return return_iterator;
		}

		u32 size() const {
			return m_size;
		}

		bool empty() const {
			return (m_first == nullptr);
		}

		void operator=( const gtList<element>& o ){
			if( this == &o ) return;
			
			clear();

			node_ptr new_node = o.m_first;
			while( new_node ){
				push_back( new_node->m_object );
				new_node = new_node->m_next;
			}
		}

		node_ptr getFirst() const {
			return m_first;
		}

		node_ptr getLast() const {
			return m_last;
		}

		void pop_back(){
			GT_ASSERT2(m_first,"m_first == nullptr");
			if( m_first )
				erase(iterator(m_first)+(m_size-1u));
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