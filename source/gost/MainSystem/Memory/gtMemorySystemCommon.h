#pragma once
#ifndef __GT_MEMORY_SYSTEM_COMMON_H__
#define __GT_MEMORY_SYSTEM_COMMON_H__


namespace gost{

	class gtMemorySystemCommon{
	public:
		virtual ~gtMemorySystemCommon(){}

		virtual void * allocate( u32 size, bool exceptions = false, bool zeroMemory = false ) = 0;
		virtual bool free( void * ptr ) = 0;
		virtual void * reallocate( u32 size, void * ptr, bool exceptions = false, bool zeroMemory = false ) = 0;
		virtual void set( void * data, void * value, u32 stride, u32 size ) = 0;
	};

	gtMemorySystemCommon* gtSingletone<gtMemorySystemCommon>::s_instance = nullptr;
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