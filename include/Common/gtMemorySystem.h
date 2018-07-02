#pragma once
#ifndef __GT_MEMORY_SYSTEM_H__
#define __GT_MEMORY_SYSTEM_H__


namespace gost{
	
	class gtMemorySystem{
	public:
		
		 GT_API static void * allocate( u32 size, bool exceptions = false, bool zeroMemory = false );

		 GT_API static bool free( void * ptr );

		 GT_API static void * reallocate( u32 size, void * ptr, bool exceptions = false, bool zeroMemory = false );

		 /*
			int arr[ 10 ];
			int val = 666;
			set( arr, &val, sizeof( int ), 10 );
		 */
		 GT_API static void set( void * data, void * value, u32 stride, u32 size );
	};

#define gtMemAlloc(size) gtMemorySystem::allocate((size))
#define gtMemAllocE(size) gtMemorySystem::allocate((size),true)
#define gtMemFree(ptr) gtMemorySystem::free((ptr));ptr=nullptr
#define gtMemReAlloc(ptr,size) gtMemorySystem::reallocate((size),(ptr))
	
	struct gtMemSet_t{
		template<typename type>
		void operator()(type val,void*ptr,u32 size) const {
			gtMemorySystem::set(ptr,&val,sizeof(type),size);
		}
	}const gtMemSet;


#define GT_DECLARE_STANDART_ALLOCATOR void* operator new( u32 size ){ return gtMemorySystem::allocate( size ); } \
	void* operator new[]( u32 size ){ return gtMemorySystem::allocate( size ); } \
	void operator delete( void * p ){ gtMemorySystem::free( p ); } \
	void operator delete[]( void * p ){ gtMemorySystem::free( p ); }


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