#include "common.h"

void gtImage_convert_a1r5g5b5_to( gtImageFormat newFormat, gtImage * image, u8 * newData, u32 newSize );
void gtImage_convert_r11g11b10_to( gtImageFormat newFormat, gtImage * image, u8 * newData, u32 newSize );
void gtImage_convert_r16_to( gtImageFormat newFormat, gtImage * image, u8 * newData, u32 newSize );
void gtImage_convert_r16g16_to( gtImageFormat newFormat, gtImage * image, u8 * newData, u32 newSize );
void gtImage_convert_r16g16b16a16_to( gtImageFormat newFormat, gtImage * image, u8 * newData, u32 newSize );
void gtImage_convert_r24g8_to( gtImageFormat newFormat, gtImage * image, u8 * newData, u32 newSize );
void gtImage_convert_r24x8_to( gtImageFormat newFormat, gtImage * image, u8 * newData, u32 newSize );
void gtImage_convert_r32_to( gtImageFormat newFormat, gtImage * image, u8 * newData, u32 newSize );
void gtImage_convert_r32g8x24_to( gtImageFormat newFormat, gtImage * image, u8 * newData, u32 newSize );
void gtImage_convert_r32g32_to( gtImageFormat newFormat, gtImage * image, u8 * newData, u32 newSize );
void gtImage_convert_r32g32b32_to( gtImageFormat newFormat, gtImage * image, u8 * newData, u32 newSize );
void gtImage_convert_r32g32b32a32_to( gtImageFormat newFormat, gtImage * image, u8 * newData, u32 newSize );
void gtImage_convert_x1r5g5b5_to( gtImageFormat newFormat, gtImage * image, u8 * newData, u32 newSize );
void gtImage_convert_x4r4g4b4_to( gtImageFormat newFormat, gtImage * image, u8 * newData, u32 newSize );
void gtImage_convert_x8r8g8b8_to( gtImageFormat newFormat, gtImage * image, u8 * newData, u32 newSize );
void gtImage_convert_x24g8_to( gtImageFormat newFormat, gtImage * image, u8 * newData, u32 newSize );
void gtImage_convert_a4r4g4b4_to( gtImageFormat newFormat, gtImage * image, u8 * newData, u32 newSize );
void gtImage_convert_a8_to( gtImageFormat newFormat, gtImage * image, u8 * newData, u32 newSize );
void gtImage_convert_a8r8g8b8_to( gtImageFormat newFormat, gtImage * image, u8 * newData, u32 newSize );
void gtImage_convert_b4g4r4a4_to( gtImageFormat newFormat, gtImage * image, u8 * newData, u32 newSize );
void gtImage_convert_b5g5r5a1_to( gtImageFormat newFormat, gtImage * image, u8 * newData, u32 newSize );
void gtImage_convert_b5g6r5_to( gtImageFormat newFormat, gtImage * image, u8 * newData, u32 newSize );
void gtImage_convert_b8g8r8a8_to( gtImageFormat newFormat, gtImage * image, u8 * newData, u32 newSize );
void gtImage_convert_b8g8r8x8_to( gtImageFormat newFormat, gtImage * image, u8 * newData, u32 newSize );
void gtImage_convert_dxt1_to( gtImageFormat newFormat, gtImage * image, u8 * newData, u32 newSize );
void gtImage_convert_dxt2_to( gtImageFormat newFormat, gtImage * image, u8 * newData, u32 newSize );
void gtImage_convert_dxt3_to( gtImageFormat newFormat, gtImage * image, u8 * newData, u32 newSize );
void gtImage_convert_dxt4_to( gtImageFormat newFormat, gtImage * image, u8 * newData, u32 newSize );
void gtImage_convert_dxt5_to( gtImageFormat newFormat, gtImage * image, u8 * newData, u32 newSize );
void gtImage_convert_g8r8g8b8_to( gtImageFormat newFormat, gtImage * image, u8 * newData, u32 newSize );
void gtImage_convert_1bit_to( gtImageFormat newFormat, gtImage * image, u8 * newData, u32 newSize );
void gtImage_convert_r1_to( gtImageFormat newFormat, gtImage * image, u8 * newData, u32 newSize );
void gtImage_convert_r5g6b5_to( gtImageFormat newFormat, gtImage * image, u8 * newData, u32 newSize );
void gtImage_convert_r8_to( gtImageFormat newFormat, gtImage * image, u8 * newData, u32 newSize );
void gtImage_convert_r8g8_to( gtImageFormat newFormat, gtImage * image, u8 * newData, u32 newSize );
void gtImage_convert_r8g8b8_to( gtImageFormat newFormat, gtImage * image, u8 * newData, u32 newSize );
void gtImage_convert_r8g8b8a8_to( gtImageFormat newFormat, gtImage * image, u8 * newData, u32 newSize );
void gtImage_convert_r8g8b8g8_to( gtImageFormat newFormat, gtImage * image, u8 * newData, u32 newSize );
void gtImage_convert_r9g9b9e5_to( gtImageFormat newFormat, gtImage * image, u8 * newData, u32 newSize );
void gtImage_convert_r10g10b10e2_to( gtImageFormat newFormat, gtImage * image, u8 * newData, u32 newSize );

u32 gtImage_get_new_size( gtImageFormat newFormat, gtImage * image ){
	u32 newSize = 0;
	
	u32 picSize = image->width * image->height;
	
	switch( newFormat ){
		case gtImageFormat::One_bit:
		case gtImageFormat::R1:
			newSize = picSize / 8;
			if( picSize % 8 )++newSize;
		break;
		
		case gtImageFormat::A8:
		case gtImageFormat::R8:
			newSize = picSize;
		break;
		
		//16 bit
		case gtImageFormat::A1R5G5B5:
		case gtImageFormat::X1R5G5B5:
		case gtImageFormat::A4R4G4B4:
		case gtImageFormat::X4R4G4B4:
		case gtImageFormat::B4G4R4A4:
		case gtImageFormat::R5G6B5:
		case gtImageFormat::B5G6R5:			
		case gtImageFormat::B5G5R5A1:
		case gtImageFormat::R8G8:
		case gtImageFormat::R16:
			newSize = picSize * 2u;
		break;
		
		// 24
		case gtImageFormat::R8G8B8:
			newSize = picSize * 3u;
		break;
		
		// 32
		case gtImageFormat::R8G8B8A8:
		case gtImageFormat::R8G8B8G8:
		case gtImageFormat::G8R8G8B8:
		case gtImageFormat::B8G8R8A8:
		case gtImageFormat::B8G8R8X8:
		case gtImageFormat::X8R8G8B8:
		case gtImageFormat::A8R8G8B8:
		case gtImageFormat::R9G9B9E5:
		case gtImageFormat::R10G10B10A2:
		case gtImageFormat::R11G11B10:
		case gtImageFormat::R16G16:
		case gtImageFormat::R24G8:
		case gtImageFormat::R24X8:
		case gtImageFormat::X24G8:
		case gtImageFormat::R32:
			newSize = picSize * 4u;
		break;
		
		// 64
		case gtImageFormat::R16G16B16A16:
		case gtImageFormat::R32G32:
		case gtImageFormat::R32G8X24:
			newSize = picSize * 8u;
		break;
		
		// 96
		case gtImageFormat::R32G32B32:
			newSize = picSize * 12u;
		break;
		
		// 128
		case gtImageFormat::R32G32B32A32:
			newSize = picSize * 16u;
		break;
		
		case gtImageFormat::DXT1:{
		}break;
		case gtImageFormat::DXT2:{
		}break;
		case gtImageFormat::DXT3:{
		}break;
		case gtImageFormat::DXT4:{
		}break;
		case gtImageFormat::DXT5:{
		}break;
	}
	
	return newSize;
}

u32 gtImage_get_pitch( gtImage * image ){
	u32 pitch = 0;

	switch( image->format ){
		case gtImageFormat::One_bit:
		case gtImageFormat::R1:
		break;
		
		case gtImageFormat::A8:
		case gtImageFormat::R8:
			pitch = image->width;
		break;
		
		//16 bit
		case gtImageFormat::A1R5G5B5:
		case gtImageFormat::X1R5G5B5:
		case gtImageFormat::A4R4G4B4:
		case gtImageFormat::X4R4G4B4:
		case gtImageFormat::B4G4R4A4:
		case gtImageFormat::R5G6B5:
		case gtImageFormat::B5G6R5:			
		case gtImageFormat::B5G5R5A1:
		case gtImageFormat::R8G8:
		case gtImageFormat::R16:
			pitch = image->width * 2u;
		break;
		
		// 24
		case gtImageFormat::R8G8B8:
			pitch = image->width * 3u;
		break;
		
		// 32
		case gtImageFormat::R8G8B8A8:
		case gtImageFormat::R8G8B8G8:
		case gtImageFormat::G8R8G8B8:
		case gtImageFormat::B8G8R8A8:
		case gtImageFormat::B8G8R8X8:
		case gtImageFormat::X8R8G8B8:
		case gtImageFormat::A8R8G8B8:
		case gtImageFormat::R9G9B9E5:
		case gtImageFormat::R10G10B10A2:
		case gtImageFormat::R11G11B10:
		case gtImageFormat::R16G16:
		case gtImageFormat::R24G8:
		case gtImageFormat::R24X8:
		case gtImageFormat::X24G8:
		case gtImageFormat::R32:
			pitch = image->width * 4u;
		break;
		
		// 64
		case gtImageFormat::R16G16B16A16:
		case gtImageFormat::R32G32:
		case gtImageFormat::R32G8X24:
			pitch = image->width * 8u;
		break;
		
		// 96
		case gtImageFormat::R32G32B32:
			pitch = image->width * 12u;
		break;
		
		// 128
		case gtImageFormat::R32G32B32A32:
			pitch = image->width * 16u;
		break;
		
		case gtImageFormat::DXT1:{
		}break;
		case gtImageFormat::DXT2:{
		}break;
		case gtImageFormat::DXT3:{
		}break;
		case gtImageFormat::DXT4:{
		}break;
		case gtImageFormat::DXT5:{
		}break;
	}
	
	return pitch;
}

// command = 1 - конвертирование
void gtImage_converter( s32 command, gtImageFormat format, gtImageFormat newFormat, gtImage * image ){
	
	u8 * old_data = image->data;
	u32 newSize = 0;
	u8 * newData = nullptr;
	
	if( command == 1 ){
		newSize = gtImage_get_new_size( newFormat, image );
		newData = (u8*)gtMemAlloc(newSize);
	}
	
	switch( format ){
		case gtImageFormat::One_bit:{
			if( command == 1 )
				gtImage_convert_1bit_to( newFormat, image, newData, newSize );
		}break;
		
		case gtImageFormat::R1:{
			if( command == 1 )
				gtImage_convert_r1_to( newFormat, image, newData, newSize );
		}break;
		
		case gtImageFormat::A1R5G5B5:{
			if( command == 1 )
				gtImage_convert_a1r5g5b5_to( newFormat, image, newData, newSize );
		}break;
		case gtImageFormat::X1R5G5B5:{			
			if( command == 1 )
				gtImage_convert_x1r5g5b5_to( newFormat, image, newData, newSize );
		}break;
		
		case gtImageFormat::A4R4G4B4:{			
			if( command == 1 )
				gtImage_convert_a4r4g4b4_to( newFormat, image, newData, newSize );
		}break;
		case gtImageFormat::X4R4G4B4:{			
			if( command == 1 )
				gtImage_convert_x4r4g4b4_to( newFormat, image, newData, newSize );
		}break;
		case gtImageFormat::B4G4R4A4:{			
			if( command == 1 )
				gtImage_convert_b4g4r4a4_to( newFormat, image, newData, newSize );
		}break;
		
		case gtImageFormat::R5G6B5:{			
			if( command == 1 )
				gtImage_convert_r5g6b5_to( newFormat, image, newData, newSize );
		}break;
		case gtImageFormat::B5G6R5:{			
			if( command == 1 )
				gtImage_convert_b5g6r5_to( newFormat, image, newData, newSize );
		}break;
		case gtImageFormat::B5G5R5A1:{			
			if( command == 1 )
				gtImage_convert_b5g5r5a1_to( newFormat, image, newData, newSize );
		}break;
		
		case gtImageFormat::A8:{			
			if( command == 1 )
				gtImage_convert_a8_to( newFormat, image, newData, newSize );
		}break;
		
		case gtImageFormat::R8:{			
			if( command == 1 )
				gtImage_convert_r8_to( newFormat, image, newData, newSize );
		}break;
		case gtImageFormat::R8G8:{			
			if( command == 1 )
				gtImage_convert_r8g8_to( newFormat, image, newData, newSize );
		}break;
		case gtImageFormat::R8G8B8:{			
			if( command == 1 )
				gtImage_convert_r8g8b8_to( newFormat, image, newData, newSize );
		}break;
		case gtImageFormat::R8G8B8A8:{			
			if( command == 1 )
				gtImage_convert_r8g8b8a8_to( newFormat, image, newData, newSize );
		}break;
		case gtImageFormat::R8G8B8G8:{			
			if( command == 1 )
				gtImage_convert_r8g8b8g8_to( newFormat, image, newData, newSize );
		}break;
		case gtImageFormat::G8R8G8B8:{			
			if( command == 1 )
				gtImage_convert_g8r8g8b8_to( newFormat, image, newData, newSize );
		}break;
		case gtImageFormat::B8G8R8A8:{			
			if( command == 1 )
				gtImage_convert_b8g8r8a8_to( newFormat, image, newData, newSize );
		}break;
		case gtImageFormat::B8G8R8X8:{			
			if( command == 1 )
				gtImage_convert_b8g8r8x8_to( newFormat, image, newData, newSize );
		}break;
		case gtImageFormat::X8R8G8B8:{			
			if( command == 1 )
				gtImage_convert_x8r8g8b8_to( newFormat, image, newData, newSize );
		}break;
		case gtImageFormat::A8R8G8B8:{			
			if( command == 1 )
				gtImage_convert_a8r8g8b8_to( newFormat, image, newData, newSize );
		}break;
		
		case gtImageFormat::R9G9B9E5:{			
			if( command == 1 )
				gtImage_convert_r9g9b9e5_to( newFormat, image, newData, newSize );
		}break;
		
		case gtImageFormat::R10G10B10A2:{			
			if( command == 1 )
				gtImage_convert_r10g10b10e2_to( newFormat, image, newData, newSize );
		}break;
		
		case gtImageFormat::R11G11B10:{			
			if( command == 1 )
				gtImage_convert_r11g11b10_to( newFormat, image, newData, newSize );
		}break;
		
		case gtImageFormat::R16:{			
			if( command == 1 )
				gtImage_convert_r16_to( newFormat, image, newData, newSize );
		}break;
		case gtImageFormat::R16G16:{			
			if( command == 1 )
				gtImage_convert_r16g16_to( newFormat, image, newData, newSize );
		}break;
		case gtImageFormat::R16G16B16A16:{			
			if( command == 1 )
				gtImage_convert_r16g16b16a16_to( newFormat, image, newData, newSize );
		}break;
		
		case gtImageFormat::R24G8:{			
			if( command == 1 )
				gtImage_convert_r24g8_to( newFormat, image, newData, newSize );
		}break;
		case gtImageFormat::R24X8:{			
			if( command == 1 )
				gtImage_convert_r24x8_to( newFormat, image, newData, newSize );
		}break;
		case gtImageFormat::X24G8:{			
			if( command == 1 )
				gtImage_convert_x24g8_to( newFormat, image, newData, newSize );
		}break;
		
		case gtImageFormat::R32:{			
			if( command == 1 )
				gtImage_convert_r32_to( newFormat, image, newData, newSize );
		}break;
		case gtImageFormat::R32G32:{			
			if( command == 1 )
				gtImage_convert_r32g32_to( newFormat, image, newData, newSize );
		}break;
		case gtImageFormat::R32G8X24:{			
			if( command == 1 )
				gtImage_convert_r32g8x24_to( newFormat, image, newData, newSize );
		}break;
		case gtImageFormat::R32G32B32:{			
			if( command == 1 )
				gtImage_convert_r32g32b32_to( newFormat, image, newData, newSize );
		}break;
		case gtImageFormat::R32G32B32A32:{			
			if( command == 1 )
				gtImage_convert_r32g32b32a32_to( newFormat, image, newData, newSize );
		}break;
		
		case gtImageFormat::DXT1:{			
			if( command == 1 )
				gtImage_convert_dxt1_to( newFormat, image, newData, newSize );
		}break;
		case gtImageFormat::DXT2:{			
			if( command == 1 )
				gtImage_convert_dxt2_to( newFormat, image, newData, newSize );
		}break;
		case gtImageFormat::DXT3:{			
			if( command == 1 )
				gtImage_convert_dxt3_to( newFormat, image, newData, newSize );
		}break;
		case gtImageFormat::DXT4:{			
			if( command == 1 )
				gtImage_convert_dxt4_to( newFormat, image, newData, newSize );
		}break;
		case gtImageFormat::DXT5:{			
			if( command == 1 )
				gtImage_convert_dxt5_to( newFormat, image, newData, newSize );
		}break;
	}
	
	if( command == 1 ){
		image->format = newFormat;
		image->dataSize = newSize;
		gtMemFree(old_data);
		image->data = newData;
		image->pitch = gtImage_get_pitch(image);
	}
}

void gtImage::_convert( gtImageFormat newFormat, gtImage * image ){
	if( newFormat == image->format ) 
		return;
	gtImage_converter( 1, image->format, newFormat, image );
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