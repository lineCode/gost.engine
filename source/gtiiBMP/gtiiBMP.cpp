#define GT_EXPORTS

#include "common.h"

#ifdef _MSC_VER
#ifdef _DEBUG
#pragma comment(lib, "gost_d.lib")
#else 
#pragma comment(lib, "gost.lib")
#endif
#endif

extern "C"{

	GT_API void __cdecl GetPluginInfo( gtPluginInfo* info ){
		info->m_author.assign( u"532235" );
		info->m_description.assign( u"Import .bmp .dib .rle files" );
		info->m_GUID=GT_UID_IMPORT_IMAGE_BMP;
		info->m_name.assign( u"Bitmap file importer" );
		info->m_type = gtPluginType::Import_image;
		info->m_version = 1;
		info->m_build = 12;
	}

	GT_API u32	__cdecl PluginGetExtCount(){
		return gtConst3U;
	}

	GT_API const s8*	__cdecl PluginGetExtension( u32 id ){
		GT_ASSERT1( id < gtConst3U, "Bad argument", "id < gtConst3U" );

		const s8 * exts[ gtConst3U ] = {
			"bmp",
			"dib",
			"rle"
		};

		return exts[ id ];
	}

	//	упаковка, типа __attribute__(__pack)
	//	размер BitmapHeader по настоящему становится 14 байт
	#pragma pack(push,2)
	struct BitmapHeader{
		u16		bfType;
        u32		bfSize;
        u16		bfReserved1;
        u16		bfReserved2;
        u32		bfOffBits;
	};
	#pragma pack(pop)

	//	для 8 и 4 битных картинок
	//	ячейка таблицы
	struct rgbquad{
		u8 Blue;
		u8 Green;
		u8 Red;
		u8 Reserved;
	};

	struct ciexyzTriple{
		v3i		ciexyzRed;
        v3i		ciexyzGreen;
        v3i		ciexyzBlue;
	};
	//	каждая новая версия BITMAPHEADER является продолжением предидущей
	//	поэтому для чтения header любого bmp подойдёт последняя версия BMPHEADER
	// п0следняя это версия или нет - хз
	// взял у windows 8.1 sdk
	
	struct BitmapInfoHeader_v5{ 
        u32			bV5Size;		//	размер header в файле
        s32			bV5Width;		//	ширина
        s32			bV5Height;		//	высота
        u16			bV5Planes;		//	хз что это, всегда 1
        u16			bV5BitCount;	//	биты
        u32			bV5Compression;	//	1 - RLE 8bit, 2 - RLE 4bit, 3 или что-то, видимо, специальные обозначения у разработчиков 2D редакторов.
        u32			bV5SizeImage;	//	размер массива пикселей/индексов
        s32			bV5XPelsPerMeter;// размер в чём-то, видимо для печати или вывода ещё кудато
        s32			bV5YPelsPerMeter;//	для обычного использования в ПК не играет никакой роли
        u32			bV5ClrUsed;		//	обычно тут ноль
        u32			bV5ClrImportant;//	и тут ноль
        u32			bV5RedMask;		//	для определения позиции цветов
        u32			bV5GreenMask;	//	в форматах типа x1r5g5b5
        u32			bV5BlueMask;
        u32			bV5AlphaMask;
        u32			bV5CSType;		//	далее информация для более специализированного
        ciexyzTriple bV5Endpoints;	//	использования.
        u32			bV5GammaRed;	//	для передачи простой картинки достаточно того
        u32			bV5GammaGreen;	//	что указано выше. А эта часть нужна для, например,
        u32			bV5GammaBlue;	//	тех кто делает видео плеер, видео редактор
        u32			bV5Intent;		//	что-то типа этого. Как бы универсальное решение
        u32			bV5ProfileData;	//	от Microsoft
        u32			bV5ProfileSize;
        u32			bV5Reserved;
	};

	//	изменённые функции декодирования, взяты у irrlicht
	void decompress4BitRLE( u8*& rleData, u8*& inds, u32 size, u32 width, u32 height );	
	void decompress8BitRLE( u8*& rleData, u8*& inds, u32 size, u32 width, u32 height );

		//	вызывается из gost.dll
	GT_API bool PluginLoadImage( gtImage* image, gtString* fileName ){

		if( !gtFileSystem::existFile( *fileName ) ){
			gtLogWriter::printWarning( u"BMP Plugin: File not exist [%s]", fileName->data() );
			return false;
		}

		BitmapHeader header;
		BitmapInfoHeader_v5 info;

		{	//	отдельный блок для того чтобы файл автоматически закрылся
			gtFile_t file = util::openFileForReadBin( *fileName );
			if( !file.data() ){
				gtLogWriter::printWarning( u"BMP Plugin: Can not open file [%s]", fileName->data() );
				return false;
			}

			u32 sz = file->size();
			if( sz < sizeof( BitmapHeader ) + sizeof( BitmapInfoHeader_v5 ) ){
				gtLogWriter::printWarning( u"BMP Plugin: Bad file size [%s]", fileName->data() );
				return false;
			}

			//	чтение заголовка
			u32 readNum = file->read( reinterpret_cast<u8*>(&header), sizeof(BitmapHeader) );

			if( readNum < sizeof(BitmapHeader) ){
				gtLogWriter::printWarning( u"BMP Plugin: Bad header [%s]", fileName->data() );
				return false;
			}

			if( header.bfType != 19778u ){
				gtLogWriter::printWarning( u"BMP Plugin: Bad magic [%s]", fileName->data() );
				return false;
			}

			//	чтение основной информации
			readNum = file->read( reinterpret_cast<u8*>(&info), sizeof(BitmapInfoHeader_v5) );
		}

		if( info.bV5Size < 40U ){
			gtLogWriter::printWarning( u"BMP Plugin: Bad header size [%s]", fileName->data() );
			return false;
		}

		if( !info.bV5Width ){
			gtLogWriter::printWarning( u"BMP Plugin: Bad width [%s]", fileName->data() );
			return false;
		}

		if( !info.bV5Height ){
			gtLogWriter::printWarning( u"BMP Plugin: Bad height [%s]", fileName->data() );
			return false;
		}

		if( info.bV5BitCount != gtConst1U &&
			info.bV5BitCount != gtConst4U &&
			info.bV5BitCount != gtConst8U &&
			info.bV5BitCount != 16u &&
			info.bV5BitCount != 24u &&
			info.bV5BitCount != 32u ){
			gtLogWriter::printWarning( u"BMP Plugin: Bad bit count [%s]", fileName->data() );
			return false;
		}

		image->width	= static_cast<u32>( info.bV5Width );
		image->height	= static_cast<u32>( info.bV5Height );
		image->bits		= info.bV5BitCount;

		gtFile_t file = util::openFileForReadBin( *fileName );
		if( !file.data() ){
			gtLogWriter::printWarning( u"BMP Plugin: Can not open file [%s]", fileName->data() );
			return false;
		}


		if( image->bits == 24u ){
			image->format	= gtImageFormat::R8G8B8;
			image->pitch	= image->width * gtConst3U;
			image->dataSize = image->pitch * image->height;

			image->data = (u8*)gtMemAllocE(image->dataSize);
			/*if( !gtMainSystem::getInstance()->allocateMemory( (void**)&image->data, image->dataSize ) ){
				gtLogWriter::printWarning( u"BMP Plugin: can not allocate memory. [%s]", fileName->data() );
				return false;
			}*/

			file->seek( 54u, gtFileSeekPos::Begin );

			u32 readNum = file->read( image->data, image->dataSize );
			if( readNum != image->dataSize ){
				gtLogWriter::printWarning( u"BMP Plugin: can not read file. [%s]", fileName->data() );
				//gtMainSystem::getInstance()->freeMemory( (void**)&image->data );
				gtMemFree(image->data);
				return false;
			}
			
			image->flipPixel();

		}else if( image->bits == 32u ){

			if( info.bV5Size != 40U && info.bV5Size != 56u ){
				gtLogWriter::printWarning( u"BMP Plugin: unsupported format. [%s]", fileName->data() );
				return false;
			}

			image->pitch = image->width * gtConst4U;

			if( info.bV5Size == 40U ){
				file->seek( 54u, gtFileSeekPos::Begin );
				image->format = gtImageFormat::A8R8G8B8;
			}else{
				file->seek( 70U, gtFileSeekPos::Begin );
				image->format = gtImageFormat::X8R8G8B8;
			}

			image->dataSize = image->pitch * image->height;
			image->data = (u8*)gtMemAllocE(image->dataSize);
			/*if( !gtMainSystem::getInstance()->allocateMemory( (void**)&image->data, image->dataSize ) ){
				gtLogWriter::printWarning( u"BMP Plugin: can not allocate memory. [%s]", fileName->data() );
				return false;
			}*/

			u32 readNum = file->read( image->data, image->dataSize );

			if( readNum != image->dataSize ){
				gtLogWriter::printWarning( u"BMP Plugin: can not read file. [%s]", fileName->data() );
				//gtMainSystem::getInstance()->freeMemory( (void**)&image->data );
				gtMemFree(image->data);
				return false;
			}

			image->flipPixel();

		}else if( image->bits == 16u ){

			if( info.bV5Size != 40U && info.bV5Size != 56u ){
				gtLogWriter::printWarning( u"BMP Plugin: unsupported format. [%s]", fileName->data() );
				return false;
			}

			image->pitch = image->width * gtConst2U;

			if( info.bV5Size == 40U ){ // x1r5g5b5
				file->seek( 54u, gtFileSeekPos::Begin );
				image->format = gtImageFormat::X1R5G5B5;
			}else{
				if( info.bV5RedMask == 3840U &&
					info.bV5GreenMask == 240U && 
					info.bV5BlueMask == 15u ){
					if( info.bV5AlphaMask ){
						image->format = gtImageFormat::A4R4G4B4;
					}else{
						image->format = gtImageFormat::X4R4G4B4;
					}
				}else if( info.bV5RedMask == 63488u &&
					info.bV5GreenMask == 2016u && 
					info.bV5BlueMask == 31u ){
					image->format = gtImageFormat::R5G6B5;
				}else if( info.bV5RedMask == 31744u &&
					info.bV5GreenMask == 992u && 
					info.bV5BlueMask == 31u ){
					if( info.bV5AlphaMask )
						image->format = gtImageFormat::A1R5G5B5;
				}else{
					gtLogWriter::printWarning( u"BMP Plugin: unsupported format. [%s]", fileName->data() );
					return false;
				}
				file->seek( 70U, gtFileSeekPos::Begin );
			}

			image->dataSize = image->pitch * image->height;

			image->data = (u8*)gtMemAllocE(image->dataSize);
			/*if( !gtMainSystem::getInstance()->allocateMemory( (void**)&image->data, image->dataSize ) ){
				gtLogWriter::printWarning( u"BMP Plugin: can not allocate memory. [%s]", fileName->data() );
				return false;
			}*/

			u32 readNum = file->read( image->data, image->dataSize );

			if( readNum != image->dataSize ){
				gtLogWriter::printWarning( u"BMP Plugin: can not read file. [%s]", fileName->data() );
				//gtMainSystem::getInstance()->freeMemory( (void**)&image->data );
				gtMemFree(image->data);
				return false;
			}


			 
		}else if( image->bits == gtConst8U ){
			
			image->format = gtImageFormat::R8G8B8A8;
			image->pitch = image->width * gtConst4U;
			image->dataSize = image->pitch * image->height;

			
			file->seek( 54u, gtFileSeekPos::Begin );

			u32 tableSize = header.bfOffBits - 54u;

			std::unique_ptr<u8[]> table( new u8[ tableSize ] );

			file->read( table.get(), tableSize );
			
			rgbquad * quadTable = reinterpret_cast<rgbquad*>(table.get());
			
			file->seek( header.bfOffBits, gtFileSeekPos::Begin );

			image->data = (u8*)gtMemAllocE(image->dataSize);
			/*if( !gtMainSystem::getInstance()->allocateMemory( (void**)&image->data, image->dataSize ) ){
				gtLogWriter::printWarning( u"BMP Plugin: can not allocate memory. [%s]", fileName->data() );
				return false;
			}*/

			u32 indSize = image->width * image->height;
			
			u8 * _data = new u8[ indSize ];
			std::unique_ptr<u8[]> inds( _data );

			if( !info.bV5Compression ){
				u32 readNum = file->read( inds.get(), indSize );
				if( readNum != indSize ){
					gtLogWriter::printWarning( u"BMP Plugin: can not read file. [%s]", fileName->data() );
					//gtMainSystem::getInstance()->freeMemory( (void**)&image->data );
					gtMemFree(image->data);
					return false;
				}
			}else{
				
				u8 * _data2 = new u8[ info.bV5SizeImage ];
				std::unique_ptr<u8[]> rle( _data2 );
				u32 readNum = file->read( rle.get(), info.bV5SizeImage );
				if( readNum != info.bV5SizeImage ){
					gtLogWriter::printWarning( u"BMP Plugin: can not read file. [%s]", fileName->data() );
				//	gtMainSystem::getInstance()->freeMemory( (void**)&image->data );
					gtMemFree(image->data);
					return false;
				}

				decompress8BitRLE( _data2, _data, info.bV5SizeImage, image->width, image->height );
			}

			u8 * u8_ptr = inds.get();

			for( u32 i = gtConst0U, count = gtConst0U; i < indSize; ){
				image->data[ count ] = quadTable[ u8_ptr[ i ] ].Red;
				image->data[ count+gtConst1U ] = quadTable[ u8_ptr[ i ] ].Green;
				image->data[ count+gtConst2U ] = quadTable[ u8_ptr[ i ] ].Blue;
				image->data[ count+gtConst3U ] = 255u;
				count += gtConst4U;
				i += gtConst1U;
			}

		}else if( image->bits == gtConst4U ){

			image->format = gtImageFormat::R8G8B8A8;

			image->pitch = image->width * gtConst4U;

			image->dataSize = image->pitch * image->height;

			
			file->seek( 54u, gtFileSeekPos::Begin );

			u32 tableSize = header.bfOffBits - 54u;

			std::unique_ptr<u8[]> table( new u8[ tableSize ] );

			file->read( table.get(), tableSize );


			rgbquad * quadTable = reinterpret_cast<rgbquad*>(table.get());
			
			file->seek( header.bfOffBits, gtFileSeekPos::Begin );

			
			u32 indSize = image->width * image->height/2;

			image->data = (u8*)gtMemAllocE(image->dataSize);

			/*if( !gtMainSystem::getInstance()->allocateMemory( (void**)&image->data, image->dataSize ) ){
				gtLogWriter::printWarning( u"BMP Plugin: can not allocate memory. [%s]", fileName->data() );
				return false;
			}*/


			std::unique_ptr<u8[]> inds( new u8[ indSize ] );

			u8 * u8_ptr = inds.get();

			if( !info.bV5Compression ){
				u32 readNum = file->read( inds.get(), indSize );
				if( readNum != indSize ){
					gtLogWriter::printWarning( u"BMP Plugin: can not read file. [%s]", fileName->data() );
					//gtMainSystem::getInstance()->freeMemory( (void**)&image->data );
					gtMemFree(image->data);
					return false;
				}
			}else{
				std::unique_ptr<u8[]> rle( new u8[ info.bV5SizeImage ] );
				u32 readNum = file->read( rle.get(), info.bV5SizeImage );
				if( readNum != info.bV5SizeImage ){
					gtLogWriter::printWarning( u"BMP Plugin: can not read file. [%s]", fileName->data() );
					//gtMainSystem::getInstance()->freeMemory( (void**)&image->data );
					gtMemFree(image->data);
					return false;
				}

				u8 * rlePtr = rle.get();
				decompress4BitRLE( rlePtr, u8_ptr, indSize, image->width, image->height );

			}
				
			


			for( u32 i = gtConst0U, count = gtConst0U; i < indSize; ){

				u8 color = u8_ptr[ i ];
				u8 part1 = color & 15u;
				u8 part2 = (color & 240U)>>gtConst4U;

				image->data[ count ] = quadTable[ part1 ].Red;
				image->data[ count+gtConst1U ] = quadTable[ part1 ].Green;
				image->data[ count+gtConst2U ] = quadTable[ part1 ].Blue;
				image->data[ count+gtConst3U ] = 255u;
				image->data[ count+gtConst4U ] = quadTable[ part2 ].Red;
				image->data[ count+gtConst5U ] = quadTable[ part2 ].Green;
				image->data[ count+gtConst6U ] = quadTable[ part2 ].Blue;
				image->data[ count+gtConst7U ] = 255u;
				count += gtConst8U;
				i += gtConst1U;
			}

			


		}else if( image->bits == gtConst1U ){
			image->format = gtImageFormat::One_bit;
			image->pitch = image->width;
			image->dataSize = info.bV5SizeImage;
			file->seek( header.bfOffBits, gtFileSeekPos::Begin );
			image->data = (u8*)gtMemAllocE(image->dataSize);

			/*if( !gtMainSystem::getInstance()->allocateMemory( (void**)&image->data, image->dataSize ) ){
				gtLogWriter::printWarning( u"BMP Plugin: can not allocate memory. [%s]", fileName->data() );
				return false;
			}*/

			u32 readNum = file->read( image->data, image->dataSize );
			if( readNum != image->dataSize ){
				gtLogWriter::printWarning( u"BMP Plugin: can not read file. [%s]", fileName->data() );
				gtMemFree(image->data);
				//gtMainSystem::getInstance()->freeMemory( (void**)&image->data );
				return false;
			}

			return true;
		}else{
			return false;
		}

		image->flipVertical();

		return true;
	}


//	irrlicht
void decompress4BitRLE( u8*& rleData, u8*& inds, u32 size, u32 width, u32 height ){
		u32 lineWidth = (width+gtConst1U)/gtConst2U/*+pitch*/;
		u8* p = rleData;
		u8* newBmp = inds;
		u8* d = newBmp;
		u8* destEnd = newBmp + lineWidth*height;
		s32 line = 0;
		s32 shift = 4;

		while( rleData - p < (s32)size && d < destEnd){
			if( *p == 0 ){
				++p;

				switch( *p ){
				case 0: // end of line
					++p;
					++line;
					d = newBmp + (line*lineWidth);
					shift = 4;
					break;
				case 1: // end of bmp
					return;
				case 2:
					{
						++p;
						s32 x = (u8)*p; ++p;
						s32 y = (u8)*p; ++p;
						d += x/2 + y*lineWidth;
						shift = x%2==0 ? 4 : 0;
					}
					break;
				default:
					{
						// absolute mode
						s32 count = (u8)*p; ++p;
						s32 readAdditional = ((2-((count)%2))%2);
						s32 readShift = 4;
						s32 i;

						for (i=0; i<count; ++i)
						{
							s32 color = (((u8)*p) >> readShift) & 0x0f;
							readShift -= 4;
							if (readShift < 0)
							{
								++*p;
								readShift = 4;
							}

							u8 mask = static_cast<u8>( 0x0f << shift );
							*d = static_cast<u8>((*d & (~mask)) | ((color << shift) & mask));

							shift -= 4;
							if (shift < 0)
							{
								shift = 4;
								++d;
							}

						}

						for (i=0; i<readAdditional; ++i)
							++p;
					}
				}
			}
			else
			{
				s32 count = (u8)*p; ++p;
				s32 color1 = (u8)*p; color1 = color1 & 0x0f;
				s32 color2 = (u8)*p; color2 = (color2 >> 4) & 0x0f;
				++p;

				for (s32 i=0; i<count; ++i)
				{
					u8 mask = static_cast<u8>(0x0f << shift);
					u8 toSet = static_cast<u8>((shift==0 ? color1 : color2) << shift);
					*d = static_cast<u8>((*d & (~mask)) | (toSet & mask));

					shift -= 4;
					if (shift < 0)
					{
						shift = 4;
						++d;
					}
				}
			}
		}
	}

	void decompress8BitRLE( u8*& rleData, u8*& inds, u32 size, u32 width, u32 height ){
		u8* p = rleData;
		u8* newBmp = inds;
		u8* d = newBmp;
		u8* destEnd = newBmp + (width*height);
		s32 line = 0;

		while( rleData - p < (s32)size && d < destEnd)
		{
			if (*p == 0)
			{
				++p;

				switch(*p)
				{
				case 0: // end of line
					++p;
					++line;
					d = newBmp + (line*(width));
					break;
				case 1: // end of bmp
					return;
				case 2:
					++p; d +=(u8)*p;  // delta
					++p; d += ((u8)*p)*(width);
					++p;
					break;
				default:
					{
						// absolute mode
						s32 count = (u8)*p; ++p;
						s32 readAdditional = ((2-(count%2))%2);
						s32 i;

						for (i=0; i<count; ++i)
						{
							*d = *p;
							++p;
							++d;
						}

						for (i=0; i<readAdditional; ++i)
							++p;
					}
				}
			}
			else
			{
				s32 count = (u8)*p; ++p;
				u8 color = *p; ++p;
				for (s32 i=0; i<count; ++i)
				{
					*d = color;
					++d;
				}
			}
		}
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