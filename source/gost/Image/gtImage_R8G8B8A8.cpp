#include "common.h"

void gtImage_convert_r8g8b8a8_to( gtImageFormat newFormat, gtImage * image, u8 * newData, u32 newSize ){
	u8 * data = image->data;
	switch( newFormat ){
		case gtImageFormat::R1:
		case gtImageFormat::One_bit:{
			
			u8 byte = 0u;
			
			u32 bit_count = 0u;
			u32 count = 0u;
			
			for( u32 i = 0u; i < image->dataSize; ){
				
				u16 color = data[ i ]; 
				
				if( gtImageFormat::One_bit == newFormat ){
					color += data[ i + 1u ] / 2; //среднее значение цвета пикселя
					color += data[ i + 2u ] / 2;
				}
				
				if( color > 230 || data[ i + 3u ] < 170 ){
					byte |= 1u;
				}
				
				++bit_count;
				
				if( bit_count == 8u ){
					newData[ count ] = byte;
					byte = bit_count = 0u;
					++count;
				}
				
				byte <<= 1u;
				
				i += 4u;
			}
			
			if( bit_count ){
				newData[ count ] = byte;
			}
			
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
		case gtImageFormat::R8G8B8:
		break;
		case gtImageFormat::R8G8B8A8:
		break;
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
