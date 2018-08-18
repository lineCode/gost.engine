#include "common.h"

void gtImage_convert_1bit_to( gtImageFormat newFormat, gtImage * image, 
	u8 * newData, u32 newSize ){
		
	u8 * data = image->data;
	switch( newFormat ){
		case gtImageFormat::One_bit:
		case gtImageFormat::R1:{
			auto sz = (image->width*image->height) / 8;
			if( (image->width*image->height) % 8 )
				++sz;
			memcpy(newData,data,sz);
		}break;
		case gtImageFormat::A1R5G5B5:
		break;
		case gtImageFormat::X1R5G5B5:
		break;
		case gtImageFormat::A4R4G4B4:
		break;
		case gtImageFormat::X4R4G4B4:
		break;
		case gtImageFormat::B4G4R4A4:
		break;
		case gtImageFormat::R5G6B5:
		break;
		case gtImageFormat::B5G6R5:
		break;
		case gtImageFormat::B5G5R5A1:
		break;
		case gtImageFormat::A8:
		break;
		case gtImageFormat::R8:
		break;
		case gtImageFormat::R8G8:
		break;
		case gtImageFormat::R8G8B8:{
			for( u32 i = 0u, count = 0u; i < newSize; ){
			}
		}break;
		case gtImageFormat::R8G8B8A8:{
			u32 * data32 = reinterpret_cast<u32*>(newData);
			u32 sz = newSize / 4u;
			u32 shift = 0u;
			u32 mask = 1u;
			u8 curpixel = 0u;
			for( u32 i = 0u, count = 0u; i < sz; ++i ){
				
				/*
					0010 1001 -> 1001 0100
				*/
				if( !shift ){
					u8 tmp = data[ count ];
					curpixel = (tmp&1u)<<7u;
					curpixel |= (tmp&2u)<<5u;
					curpixel |= (tmp&4u)<<3u;
					curpixel |= (tmp&8u)<<1u;
					curpixel |= (tmp&16u)>>1u;
					curpixel |= (tmp&32u)>>3u;
					curpixel |= (tmp&64u)>>5u;
					curpixel |= (tmp&128u)>>7u;
				}
				
				data32[ i ] = ((curpixel & mask)>>shift)?0xffffffff:0u;
				
				mask <<= 1u;
				++shift;
				
				if( shift == 8u ){
					shift = 0u;
					mask = 1u;
					++count;
				}
				
			}
		}break;
		case gtImageFormat::R8G8B8G8:
		break;
		case gtImageFormat::G8R8G8B8:
		break;
		case gtImageFormat::B8G8R8A8:
		break;
		case gtImageFormat::B8G8R8X8:
		break;
		case gtImageFormat::X8R8G8B8:
		break;
		case gtImageFormat::A8R8G8B8:
		break;
		case gtImageFormat::R9G9B9E5:
		break;
		case gtImageFormat::R10G10B10A2:
		break;
		case gtImageFormat::R11G11B10:
		break;
		case gtImageFormat::R16:
		break;
		case gtImageFormat::R16G16:
		break;
		case gtImageFormat::R16G16B16A16:
		break;
		case gtImageFormat::R24G8:
		break;
		case gtImageFormat::R24X8:
		break;
		case gtImageFormat::X24G8:
		break;
		case gtImageFormat::R32:
		break;
		case gtImageFormat::R32G32:
		break;
		case gtImageFormat::R32G8X24:
		break;
		case gtImageFormat::R32G32B32:
		break;
		case gtImageFormat::R32G32B32A32:
		break;
		case gtImageFormat::DXT1:
		break;
		case gtImageFormat::DXT3:
		break;
		case gtImageFormat::DXT5:
		break;
		case gtImageFormat::DXT2:
		break;
		case gtImageFormat::DXT4:
		break;
	}
}

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
