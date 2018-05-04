#pragma once
#ifndef __GT_GUID_H__
#define __GT_GUID_H__

namespace gost{

	struct GT_GUID{

		GT_GUID(){}
		GT_GUID(u32 d1, u16 d2, u16 d3, u8 d4, u8 d5, u8 d6, u8 d7, u8 d8, u8 d9, u8 d10, u8 d11){
			Data1 = d1;
			Data2 = d2;
			Data3 = d3;
			Data4[ 0u ] = d4;
			Data4[ 1u ] = d5;
			Data4[ 2u ] = d6;
			Data4[ 3u ] = d7;
			Data4[ 4u ] = d8;
			Data4[ 5u ] = d9;
			Data4[ 6u ] = d10;
			Data4[ 7u ] = d11;
		}

		u32 Data1;
		u16 Data2;
		u16 Data3;
		u8  Data4[ 8u ];

		bool operator==( const GT_GUID& other )const{
			if( Data1 == other.Data1 )
				if( Data2 == other.Data2 )
					if( Data3 == other.Data3 )
						if( Data4[ 0u ] == other.Data4[ 0u ] )
							if( Data4[ 1u ] == other.Data4[ 1u ] )
								if( Data4[ 2u ] == other.Data4[ 2u ] )
									if( Data4[ 3u ] == other.Data4[ 3u ] )
										if( Data4[ 4u ] == other.Data4[ 4u ] )
											if( Data4[ 5u ] == other.Data4[ 5u ] )
												if( Data4[ 6u ] == other.Data4[ 6u ] )
													if( Data4[ 7u ] == other.Data4[ 7u ] )
														return true;
			return false;
		}
	};
	#define GT_DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) const GT_GUID name = GT_GUID( l, w1, w2, b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 )
	

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
