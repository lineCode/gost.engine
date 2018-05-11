#pragma once
#ifndef __GT_VERTEX_H__
#define __GT_VERTEX_H__

namespace gost{
	constexpr u32 gtStrideStandart = sizeof( gtReal ) * 9u;

	struct gtVertex{

		u8	* data;

		gtVertex():
			data( nullptr )
		{}

		~gtVertex(){
			if( data ) delete []data;
		}


		gtVertex*	allocate( u32 stride ){
			data = new u8[ stride ];
			return this;
		}
	};

	enum class gtVertexType{
		position,	//< 4 * sizeof( gtReal )
		normal,		//< 3 * sizeof( gtReal )
		uv,			//< 2 * sizeof( gtReal )
		color,		//< 4 * sizeof( gtReal )
		binormal,	//< 3 * sizeof( gtReal )
		tangent,	//< 3 * sizeof( gtReal )
		end			
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