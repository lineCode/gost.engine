#define GT_EXPORTS

#include "common.h"

#include <memory>

#pragma warning(disable : 4820)

#include <png.h>
#include <zlib.h>

extern "C"{

#define GT_EXT_COUNT 1u

	GT_API void	GetPluginInfo( gtPluginInfo& info ){
		info.m_author.assign( u"532235" );
		info.m_description.assign( u"Import .png files" );
		info.m_GUID=GT_UID_IMPORT_IMAGE_PNG;
		info.m_name.assign( u"PNG file importer" );
		info.m_type = gtPluginType::import_image;
		info.m_version = 1;
		info.m_build = 13;
	}

	GT_API u32	PluginGetExtCount( void ){
		return GT_EXT_COUNT;
	}

	GT_API s8*	PluginGetExtension( u32 id ){
		GT_ASSERT1( id < GT_EXT_COUNT, "Bad argument", "id < 1u" );

		s8 * exts[ GT_EXT_COUNT ] = {
			"png"
		};

		return exts[ id ];
	}

	/* ������������ libpng ��� ������ ������ */
	static void __cdecl user_error_fn( png_structp png_ptr, png_const_charp msg ){
		gtString umsg;
		util::stringAppend( umsg, msg, u'a' );

		gtLogWriter::printError( u"PNG Plugin: %s", umsg.data() );

		longjmp(png_jmpbuf(png_ptr), 1);
	}

	/* ������������ libpng ��� ������ �������������� */
	static void __cdecl user_warning_fn( png_structp /*png_ptr*/, png_const_charp msg ){
		gtString umsg;
		util::stringAppend( umsg, msg, u'a' );

		gtLogWriter::printWarning( u"PNG Plugin: %s", umsg.data() );
	}


	/* �������� ����� ������������� ��������� 
		png_structp png_ptr;
		png_infop info_ptr;
	*/
	class PNG : public gtRefObject{
	public:

		PNG():
			png_ptr( nullptr ),
			info_ptr( nullptr )
		{}

		~PNG(){
			if( info_ptr ){
				png_destroy_read_struct( &png_ptr, &info_ptr, NULL);
			}else if( png_ptr )
				png_destroy_read_struct( &png_ptr, NULL, NULL);
		}

		png_structp png_ptr;
		png_infop info_ptr;
	};

	/* ���� �������� libpng ������ ���� */
	void PNGAPI user_read_fn( png_structp png_ptr, png_bytep data, png_size_t length ){
		gtFile * file = (gtFile*)png_get_io_ptr( png_ptr );

		png_size_t check = (png_size_t)file->read(
			(u8*)data, (u32)length );

		if( check != length )
			png_error( png_ptr, "Read Error" );
	}

		//	���������� �� gost.dll
	GT_API bool PluginLoadImage( gtImage* image, gtString* fileName ){

		if( !gtFileSystem::existFile( *fileName ) ){
			gtLogWriter::printWarning( u"PNG Plugin: File not exist [%s]", fileName->data() );
			return false;
		}


		{	
			gtFile_t file = util::openFileForReadBin( *fileName );
			if( !file.data() ){
				gtLogWriter::printWarning( u"PNG Plugin: Can not open file [%s]", fileName->data() );
				return false;
			}


			/* �� ����� ��� � ������� */
			#define PNG_BYTES_TO_CHECK 4
			u8 buf[PNG_BYTES_TO_CHECK];

			if( file->read( buf, PNG_BYTES_TO_CHECK ) != PNG_BYTES_TO_CHECK ){
				gtLogWriter::printWarning( u"PNG Plugin: Bad header [%s]", fileName->data() );
				return false;
			}

			if( png_sig_cmp( buf, (png_size_t)0, PNG_BYTES_TO_CHECK ) ){
				gtLogWriter::printWarning( u"PNG Plugin: Bad file [%s]", fileName->data() );
				return false;
			}

			gtPtr<PNG> png = gtPtrNew<PNG>( new PNG );
			png->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
			  nullptr, user_error_fn, user_warning_fn );
			if( !png->png_ptr ){
				gtLogWriter::printError( u"PNG Plugin: Can not create read struct [%s]", fileName->data() );
				return false;
			}


			png->info_ptr = png_create_info_struct( png->png_ptr );
			if( !png->info_ptr ){
				gtLogWriter::printError( u"PNG Plugin: Can not create info struct [%s]", fileName->data() );
				return false;
			}

			if( setjmp( png_jmpbuf( png->png_ptr ) ) ){
				return false;
			}

			png_set_read_fn( png->png_ptr, (void *)file.data(), user_read_fn);

			png_set_sig_bytes( png->png_ptr, PNG_BYTES_TO_CHECK );

			png_read_info( png->png_ptr, png->info_ptr );

			u32 w, h;
			s32 bits;
			s32 color_type;
			png_get_IHDR( png->png_ptr, png->info_ptr, &w, &h, &bits, &color_type,
				NULL, NULL, NULL);

			if( color_type == PNG_COLOR_TYPE_PALETTE )
				png_set_palette_to_rgb( png->png_ptr );

			if( bits < 8 ){
				if( color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA )
					png_set_expand_gray_1_2_4_to_8( png->png_ptr );
			else
				png_set_packing( png->png_ptr );
			}

			if( png_get_valid( png->png_ptr, png->info_ptr, PNG_INFO_tRNS ) )
				png_set_tRNS_to_alpha( png->png_ptr );

			if( bits == 16 )
				png_set_strip_16( png->png_ptr );

			if( color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA )
				png_set_gray_to_rgb( png->png_ptr );

			s32 intent;
			const f64 screen_gamma = 2.2;

			if( png_get_sRGB( png->png_ptr, png->info_ptr, &intent ) )
				png_set_gamma( png->png_ptr, screen_gamma, 0.45455 );
			else{
				double image_gamma;
				if( png_get_gAMA( png->png_ptr, png->info_ptr, &image_gamma ) )
					png_set_gamma( png->png_ptr, screen_gamma, image_gamma );
				else
					png_set_gamma( png->png_ptr, screen_gamma, 0.45455);
			}

			png_read_update_info( png->png_ptr,  png->info_ptr );
			png_get_IHDR( png->png_ptr, png->info_ptr, &w, &h, &bits, &color_type,
				NULL, NULL, NULL);

			image->width = w;
			image->height = h;

			if( color_type == PNG_COLOR_TYPE_RGB_ALPHA ){
				image->format = gtImage::Format::FMT_R8G8B8A8;
				image->pitch	= image->width * 4u;
			}else{
				image->format = gtImage::Format::FMT_R8G8B8;
				image->pitch	= image->width * 3u;
			}


			image->dataSize = image->pitch * image->height;
			if( !gtMainSystem::getInstance()->allocateMemory( (void**)&image->data, image->dataSize ) ){
				gtLogWriter::printWarning( u"PNG Plugin: can not allocate memory. [%s]", fileName->data() );
				return false;
			}

			//	��������� �� ������/�����
			std::unique_ptr<png_bytep[]> row_pointers( new png_bytep[ image->height ] );
			for( u32 row = 0u, p = 0u; row < image->height; ++row ){
				row_pointers[ row ] = &image->data[ p ];
				p += image->pitch;
			}

			png_read_image( png->png_ptr, row_pointers.get() );

			png_read_end( png->png_ptr, NULL );

		}

		return true;
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