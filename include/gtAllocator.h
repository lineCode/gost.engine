/*!	GOST
	\file gtAllocator.h
	\brief Allocator
*/

#pragma once
#ifndef __GT_ALLOCATOR_H__
#define __GT_ALLOCATOR_H__

namespace gost{

	template<typename Type>
	class gtAllocator{
	public:

		gtAllocator( void ){}

		Type * allocate( u32 size ){
			return static_cast<Type*>( operator new( size * sizeof( Type ) ) );
		}

		void deallocate( Type * ptr ){
			operator delete( ptr );
		}

		void construct( Type * new_data, const Type& old_data ){
			new((void*)new_data) Type( old_data );
		}

		void destruct( Type * ptr ){
			ptr->~Type();
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
