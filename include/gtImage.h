/*!	GOST
	\file gtImage.h
	\brief software image
*/

#pragma once
#ifndef __GT_IMAGE_H__
#define __GT_IMAGE_H__ // include guard


namespace gost{

	class gtMainSystem;

		// software image
	class gtImage : public gtRefObject{
	public:

			// c-tor
		gtImage( void ):
			format( Format::FMT_R8G8B8A8 ),
			width( 0u ),
			height( 0u ),
			bits( 32u ),
			mipCount( 1u ),
			data( nullptr ),
			dataSize( 0u ),
			pitch( 0u ),
			frames( 1u )
		{}

			// d-tor
		virtual ~gtImage( void ){
			if( data ){
					gtMainSystem::getInstance()->freeMemory( (void**)&data );
				data = nullptr;
			}
		}


		enum Format{

			FMT_ONE_BIT,	// white or black
			FMT_A1R5G5B5,	// 1 11111 11111 11111
			FMT_X1R5G5B5,	// x 11111 11111 11111
			FMT_A4R4G4B4,	// 1111 1111 1111 1111
			FMT_X4R4G4B4,	// xxxx 1111 1111 1111
			FMT_R5G6B5,		// 11111 111111 11111
			FMT_X8R8G8B8,	// u8 u8 u8 u8
			FMT_A8R8G8B8,	// u8 u8 u8 u8

			FMT_R8G8B8A8,	// u8 u8 u8 u8

			FMT_R8G8B8,		// u8 u8 u8


			//	OpenGL
			FMT_COMPRESSED_RGBA_S3TC_DXT1 = 0x83F1, // dds dxt1
			FMT_COMPRESSED_RGBA_S3TC_DXT3 = 0x83F2, // dds dxt3
			FMT_COMPRESSED_RGBA_S3TC_DXT5 = 0x83F3	// dds dxt5

		}format;

		u32		width;

		u32		height;

		u32		bits;

		u32		mipCount;

		u8	*	data;

		u32		dataSize;

		u32		pitch;

		u32		frames;

		void	convert( Format newFormat ){
			if( newFormat == format ) return;

			if( format == FMT_R8G8B8 ){
				if( newFormat == FMT_A8R8G8B8 ){
					__R8G8B8_to_A8R8G8B8();
				}else if( newFormat == FMT_R8G8B8A8 ){
					__R8G8B8_to_R8G8B8A8();
				}else if( newFormat == FMT_X8R8G8B8 ){
					__R8G8B8_to_A8R8G8B8();
				}

			//	==================================================
			}else if( format == FMT_A8R8G8B8 ){
				if( newFormat == FMT_R8G8B8 ){
					__A8R8G8B8_to_R8G8B8();
				}else if( newFormat == FMT_R8G8B8A8 ){
					__A8R8G8B8_to_R8G8B8A8();
				}else if( newFormat == FMT_X8R8G8B8 ){
					__A8R8G8B8_to_X8R8G8B8();
				}
			//	==================================================
			}else if( format == FMT_X8R8G8B8 ){
				if( newFormat == FMT_R8G8B8 ){
					__A8R8G8B8_to_R8G8B8();
				}else if( newFormat == FMT_R8G8B8A8 ){
					__A8R8G8B8_to_R8G8B8A8();
				}else if( newFormat == FMT_A8R8G8B8 ){
					//__X8R8G8B8_to_A8R8G8B8();
				}

			//	==================================================
			}else if( format == FMT_R8G8B8A8 ){
				if( newFormat == FMT_R8G8B8 ){
					__R8G8B8A8_to_R8G8B8();
				}else if( newFormat == FMT_X8R8G8B8 ){
					__R8G8B8A8_to_X8R8G8B8();
				}else if( newFormat == FMT_A8R8G8B8 ){
					__R8G8B8A8_to_A8R8G8B8();
				}

				//	==================================================
			}else if( format == FMT_X4R4G4B4 ){
				if( newFormat == FMT_R8G8B8A8 ){
					__X4R4G4B4_to_R8G8B8A8();
				}else if( newFormat == FMT_A8R8G8B8 ){
					__X4R4G4B4_to_A8R8G8B8();
				}else if( newFormat == FMT_X8R8G8B8 ){
					__X4R4G4B4_to_A8R8G8B8();
				}else if( newFormat == FMT_R8G8B8 ){
					__X4R4G4B4_to_R8G8B8();
				}

				//	==================================================
			}else if( format == FMT_X1R5G5B5 ){
				if( newFormat == FMT_R8G8B8A8 ){
					__X1R5G5B5_to_R8G8B8A8();
				}

				//	==================================================
			}else if( format == FMT_A1R5G5B5 ){
				if( newFormat == FMT_R8G8B8A8 ){
					__A1R5G5B5_to_R8G8B8A8();
				}

			}else if( format == FMT_R5G6B5 ){
				if( newFormat == FMT_R8G8B8A8 ){
					__R5G6B5_to_R8G8B8A8();
				}

			}else if( format == FMT_ONE_BIT ){
				if( newFormat == FMT_R8G8B8A8 ){
					__1_to_R8G8B8A8();
				}

					//	==================================================
			}else if( format == FMT_A4R4G4B4 ){
				if( newFormat == FMT_R8G8B8A8 ){
					__X4R4G4B4_to_R8G8B8A8();
				}else if( newFormat == FMT_A8R8G8B8 ){
					__X4R4G4B4_to_A8R8G8B8();
				}else if( newFormat == FMT_X8R8G8B8 ){
					__X4R4G4B4_to_A8R8G8B8();
				}else if( newFormat == FMT_R8G8B8 ){
					__X4R4G4B4_to_R8G8B8();
				}
			}

			format = newFormat;
		}

		void	makeAlphaFromBlack( void ){
			if( format == gost::gtImage::FMT_R8G8B8A8){
				for( u32 i = 0u; i < dataSize; ){
					u8 r = data[ i ];
					u8 g = data[ i+1u ];
					u8 b = data[ i+2u ];
					data[ i+3u ] = (u8)std::ceil((f32)(r+g+b)*0.33333f);
					i += 4u;
				}
			}
		}

			// rgb -> bgr, rgba -> abgr, rgbx -> bgrx...
		void	flipPixel( void ){
			if( !data ) return;
			switch( format ){
			case gost::gtImage::FMT_ONE_BIT:
				break;

			case gost::gtImage::FMT_A1R5G5B5:
				__flip_pixel_A1R5G5B5();
				break;
			case gost::gtImage::FMT_X1R5G5B5:
				__flip_pixel_X1R5G5B5();
				break;
			case gost::gtImage::FMT_A4R4G4B4:
				__flip_pixel_X4R4G4B4();
				break;
			case gost::gtImage::FMT_X4R4G4B4:
				__flip_pixel_X4R4G4B4();
				break;
			case gost::gtImage::FMT_R5G6B5:
				__flip_pixel_R5G6B5();
				break;
			case gost::gtImage::FMT_X8R8G8B8:
				__flip_pixel_X8R8G8B8();
				break;
			case gost::gtImage::FMT_A8R8G8B8:
				__flip_pixel_A8R8G8B8();
				break;
			case gost::gtImage::FMT_R8G8B8A8:
				__flip_pixel_A8R8G8B8();
				break;
			case gost::gtImage::FMT_R8G8B8:
				__flip_pixel_R8G8B8();
				break;
			case gost::gtImage::FMT_COMPRESSED_RGBA_S3TC_DXT1:
				break;
			case gost::gtImage::FMT_COMPRESSED_RGBA_S3TC_DXT3:
				break;
			case gost::gtImage::FMT_COMPRESSED_RGBA_S3TC_DXT5:
				break;
			default:
				break;
			}
		}

			//	отразит по вертикали (для почти всех форматов (кроме однобайтового))
		void flipVertical( void ){
			u8 * line = nullptr;
			gtMainSystem::getInstance()->allocateMemory( (void**)&line, pitch );

			u8 * p_Up = &data[ 0u ];
			u8 * p_Down = &data[ pitch * height - pitch ];

			for( u32 i = 0u; i < height / 2u; ++i ){

				memcpy( line, p_Up, pitch );

				memcpy( p_Up, p_Down, pitch );

				memcpy( p_Down, line, pitch );

				p_Up += pitch;
				p_Down -= pitch;

			}

			gtMainSystem::getInstance()->freeMemory( (void**)&line );
		}

	private:

		//	=================	FLIP	PIXEL ============
		void __flip_pixel_A8R8G8B8( void ){
			for( u32 i = 0u; i < dataSize; ){
				u8 a = data[ i ];
				u8 r = data[ i+1u ];
				u8 g = data[ i+2u ];
				u8 b = data[ i+3u ];
				data[ i ] = b;
				data[ i+1u ] = g;
				data[ i+2u ] = r;
				data[ i+3u ] = a;
				i += 4u;
			}
		}

		void __flip_pixel_X8R8G8B8( void ){
			for( u32 i = 0u; i < dataSize; ){
				u8 r = data[ i+1u ];
				u8 g = data[ i+2u ];
				u8 b = data[ i+3u ];
				data[ i+1u ] = b;
				data[ i+2u ] = g;
				data[ i+3u ] = r;
				i += 4u;
			}
		}

		void __flip_pixel_X4R4G4B4( void ){
			const u32 redMask = 3840u;
			const u32 greenMask = 240u;
			const u32 blueMask = 15u;

			u16 * p = reinterpret_cast<u16*>(data);
			u32 sz = dataSize / 2u;
			for( u32 i = 0u; i < sz; ++i ){
				u16 pixel	= p[ i ];
				u16 red		= (pixel & redMask)>>8;
				u16 green	= (pixel & greenMask)>>4;
				u16 blue	= (pixel & blueMask);

				p[ i ] = red;
				p[ i ] |= green<<4;
				p[ i ] |= blue<<8;
			}
		}

		void __flip_pixel_R5G6B5( void ){
			const u32 redMask = 63488u;
			const u32 greenMask = 2016u;
			const u32 blueMask = 31u;

			u16 * p = reinterpret_cast<u16*>(data);
			u32 sz = dataSize / 2u;
			for( u32 i = 0u; i < sz; ++i ){
				u16 pixel	= p[ i ];
				u16 red		= (pixel & redMask)>>11;
				u16 green	= (pixel & greenMask)>>6;
				u16 blue	= (pixel & blueMask);

				p[ i ] = red;
				p[ i ] |= green<<6;
				p[ i ] |= blue<<11;
			}
		}

		void __flip_pixel_X1R5G5B5( void ){
			const u32 redMask = 31744u;
			const u32 greenMask = 992u;
			const u32 blueMask = 31u;

			u16 * p = reinterpret_cast<u16*>(data);
			u32 sz = dataSize / 2u;
			for( u32 i = 0u; i < sz; ++i ){
				u16 pixel	= p[ i ];
				u16 red		= (pixel & redMask)>>10;
				u16 green	= (pixel & greenMask)>>5;
				u16 blue	= (pixel & blueMask);

				p[ i ] = red;
				p[ i ] |= green<<5;
				p[ i ] |= blue<<10;
			}
		}

		void __flip_pixel_A1R5G5B5( void ){
			const u32 alphaMask = 32768u;
			const u32 redMask = 31744u;
			const u32 greenMask = 992u;
			const u32 blueMask = 31u;

			u16 * p = reinterpret_cast<u16*>(data);
			u32 sz = dataSize / 2u;
			for( u32 i = 0u; i < sz; ++i ){
				u16 pixel	= p[ i ];
				u16 alpha	= (pixel & alphaMask)>>15;
				u16 red		= (pixel & redMask)>>10;
				u16 green	= (pixel & greenMask)>>5;
				u16 blue	= (pixel & blueMask);

				p[ i ] = alpha;
				p[ i ] |= red<<5;
				p[ i ] |= green<<10;
				p[ i ] |= blue<<15;
			}
		}

		void __flip_pixel_R8G8B8( void ){
			for( u32 i = 0u; i < dataSize; ){
				u8 red = data[ i ];
				data[ i ] = data[ i+2u ];
				data[ i+2u ] = red;
				i += 3u;
			}
		}

		//	===================		CONVERT		=======================================
		void __R8G8B8_to_A8R8G8B8( void ){
			u8 * newData = nullptr;

			u32 newSize = dataSize + (dataSize / 3u);

			gtMainSystem::getInstance()->allocateMemory( (void**)&newData, newSize );

			u32 * data32 = reinterpret_cast<u32*>(newData);

			for( u32 i = 0u; i < dataSize; ++data32 ){
				*data32 = GT_MAKEFOURCC( 255, data[ i ], data[ i+1u ], data[ i+2u ] );
				i += 3u;
			}

			dataSize = newSize;
			gtMainSystem::getInstance()->freeMemory( (void**)&data );
			data = newData;
			pitch = width * 4u;
		}


		void __A8R8G8B8_to_R8G8B8( void ){
			u8 * newData = nullptr;

			u32 newSize = dataSize - (dataSize / 4u);

			gtMainSystem::getInstance()->allocateMemory( (void**)&newData, newSize );

			for( u32 i = 0u, count = 0u; i < dataSize; ){
				newData[ count ] = data[ i+1 ];
				newData[ count+1 ] = data[ i+2 ];
				newData[ count+2 ] = data[ i+3 ];

				i += 4u;
				count += 3u;
			}

			dataSize = newSize;
			gtMainSystem::getInstance()->freeMemory( (void**)&data );
			data = newData;
			pitch = width * 3u;
		}


		void __A8R8G8B8_to_R8G8B8A8( void ){
			for( u32 i = 0u; i < dataSize; ){
				u8 a = data[ i ];
				u8 r = data[ i+1u ];
				u8 g = data[ i+2u ];
				u8 b = data[ i+3u ];
				data[ i ] = r;
				data[ i+1u ] = g;
				data[ i+2u ] = b;
				data[ i+3u ] = a;
				i += 4u;
			}
		}

		void __A8R8G8B8_to_X8R8G8B8( void ){
			for( u32 i = 0u; i < dataSize; ){
				data[ i ] = 255;
				i += 4u;
			}
		}

		/*void __X8R8G8B8_to_A8R8G8B8( void ){
			for( u32 i = 0u; i < dataSize; ){
				data[ i ] = 255;
				i += 4u;
			}
		}*/

		void __R8G8B8_to_R8G8B8A8( void ){
			u8 * newData = nullptr;

			u32 newSize = dataSize + (dataSize / 3u);

			gtMainSystem::getInstance()->allocateMemory( (void**)&newData, newSize );

			u32 * data32 = reinterpret_cast<u32*>(newData);

			for( u32 i = 0u; i < dataSize; ++data32 ){
				*data32 = GT_MAKEFOURCC( data[ i ], data[ i+1u ], data[ i+2u ], 255 );
				i += 3u;
			}

			dataSize = newSize;
			gtMainSystem::getInstance()->freeMemory( (void**)&data );
			data = newData;
			pitch = width * 4u;
		}

		void __R8G8B8A8_to_R8G8B8( void ){
			u8 * newData = nullptr;

			u32 newSize = dataSize - (dataSize / 4u);

			gtMainSystem::getInstance()->allocateMemory( (void**)&newData, newSize );

			for( u32 i = 0u, count = 0u; i < dataSize; ){
				newData[ count ] = data[ i ];
				newData[ count+1 ] = data[ i+1 ];
				newData[ count+2 ] = data[ i+2 ];

				i += 4u;
				count += 3u;
			}

			dataSize = newSize;
			gtMainSystem::getInstance()->freeMemory( (void**)&data );
			data = newData;
			pitch = width * 3u;
		}

		void __R8G8B8A8_to_X8R8G8B8( void ){
			for( u32 i = 0u; i < dataSize; ){
				u8 r = data[ i ];
				u8 g = data[ i+1u ];
				u8 b = data[ i+2u ];
				data[ i ] = 255;
				data[ i+1u ] = r;
				data[ i+2u ] = g;
				data[ i+3u ] = b;
				i += 4u;
			}
		}

		void __R8G8B8A8_to_A8R8G8B8( void ){
			for( u32 i = 0u; i < dataSize; ){
				u8 r = data[ i ];
				u8 g = data[ i+1u ];
				u8 b = data[ i+2u ];
				u8 a = data[ i+3u ];
				data[ i ] = a;
				data[ i+1u ] = r;
				data[ i+2u ] = g;
				data[ i+3u ] = b;
				i += 4u;
			}
		}

		void __X4R4G4B4_to_R8G8B8A8( void ){
			u8 * newData = nullptr;

			u32 newSize = dataSize * 2;

			gtMainSystem::getInstance()->allocateMemory( (void**)&newData, newSize );

			const u32 redMask = 3840u;
			const u32 greenMask = 240u;
			const u32 blueMask = 15u;

			u16 * p = reinterpret_cast<u16*>(data);
			u32 sz = dataSize / 2u;

			for( u32 i = 0u, count = 0u; i < sz; ++i ){

				newData[ count ] = (p[ i ]&redMask)>>8;
				newData[ count ] *= 17;

				newData[ count+1u ] = (p[ i ]&greenMask)>>4;
				newData[ count+1u ] *= 17;

				newData[ count+2u ] = (p[ i ]&blueMask);
				newData[ count+2u ] *= 17;

				newData[ count+3u ] = 255;
				count += 4u;
			}

			dataSize = newSize;
			gtMainSystem::getInstance()->freeMemory( (void**)&data );
			data = newData;
			pitch = width * 4u;
		}

		void __X4R4G4B4_to_A8R8G8B8( void ){
			u8 * newData = nullptr;

			u32 newSize = dataSize * 2;

			gtMainSystem::getInstance()->allocateMemory( (void**)&newData, newSize );

			const u32 redMask = 3840u;
			const u32 greenMask = 240u;
			const u32 blueMask = 15u;

			u16 * p = reinterpret_cast<u16*>(data);
			u32 sz = dataSize / 2u;

			for( u32 i = 0u, count = 0u; i < sz; ++i ){

				newData[ count ] = 255;

				newData[ count+1u ] = (p[ i ]&redMask)>>8;
				newData[ count+1u ] *= 17;

				newData[ count+2u ] = (p[ i ]&greenMask)>>4;
				newData[ count+2u ] *= 17;

				newData[ count+3u ] = (p[ i ]&blueMask);
				newData[ count+3u ] *= 17;

				count += 4u;
			}

			dataSize = newSize;
			gtMainSystem::getInstance()->freeMemory( (void**)&data );
			data = newData;
			pitch = width * 4u;
		}

		void __X4R4G4B4_to_R8G8B8( void ){
			u8 * newData = nullptr;

			u32 sz = dataSize / 2u;

			u32 newSize = dataSize + sz;

			gtMainSystem::getInstance()->allocateMemory( (void**)&newData, newSize );

			const u32 redMask = 3840u;
			const u32 greenMask = 240u;
			const u32 blueMask = 15u;

			u16 * p = reinterpret_cast<u16*>(data);

			for( u32 i = 0u, count = 0u; i < sz; ++i ){

				newData[ count ] = (p[ i ]&redMask)>>8;
				newData[ count ] *= 17;

				newData[ count+1u ] = (p[ i ]&greenMask)>>4;
				newData[ count+1u ] *= 17;

				newData[ count+2u ] = (p[ i ]&blueMask);
				newData[ count+2u ] *= 17;

				count += 3u;
			}

			dataSize = newSize;
			gtMainSystem::getInstance()->freeMemory( (void**)&data );
			data = newData;
			pitch = width * 3u;
		}

		void __X1R5G5B5_to_R8G8B8A8( void ){
			u8 * newData = nullptr;

			u32 newSize = dataSize * 2;

			gtMainSystem::getInstance()->allocateMemory( (void**)&newData, newSize );

			const u32 redMask = 31744u;
			const u32 greenMask = 992u;
			const u32 blueMask = 31u;

			u16 * p = reinterpret_cast<u16*>(data);
			u32 sz = dataSize / 2u;

			for( u32 i = 0u, count = 0u; i < sz; ++i ){

				newData[ count ] = (p[ i ]&redMask)>>10;
				newData[ count ] *= 8;

				newData[ count+1u ] = static_cast<u8>((p[ i ]&greenMask)>>5);
				newData[ count+1u ] *= 8;

				newData[ count+2u ] = (p[ i ]&blueMask);
				newData[ count+2u ] *= 8;

				newData[ count+3u ] = 255;
				count += 4u;
			}

			dataSize = newSize;
			gtMainSystem::getInstance()->freeMemory( (void**)&data );
			data = newData;
			pitch = width * 4u;
		}

		void __A1R5G5B5_to_R8G8B8A8( void ){
			u8 * newData = nullptr;

			u32 newSize = dataSize * 2;

			gtMainSystem::getInstance()->allocateMemory( (void**)&newData, newSize );

			const u32 alphaMask = 32768u;
			const u32 redMask = 31744u;
			const u32 greenMask = 992u;
			const u32 blueMask = 31u;

			u16 * p = reinterpret_cast<u16*>(data);
			u32 sz = dataSize / 2u;

			for( u32 i = 0u, count = 0u; i < sz; ++i ){

				newData[ count ] = (p[ i ]&redMask)>>10;
				newData[ count ] *= 8;

				newData[ count+1u ] = static_cast<u8>((p[ i ]&greenMask)>>5);
				newData[ count+1u ] *= 8;

				newData[ count+2u ] = (p[ i ]&blueMask);
				newData[ count+2u ] *= 8;

				newData[ count+3u ] = ((p[ i ]&alphaMask)>>15) ? 255u : 0u;
				count += 4u;
			}

			dataSize = newSize;
			gtMainSystem::getInstance()->freeMemory( (void**)&data );
			data = newData;
			pitch = width * 4u;
		}

		void __R5G6B5_to_R8G8B8A8( void ){
			u8 * newData = nullptr;

			u32 newSize = dataSize * 2;

			gtMainSystem::getInstance()->allocateMemory( (void**)&newData, newSize );

			const u32 redMask = 63488u;
			const u32 greenMask = 2016u;
			const u32 blueMask = 31u;

			u16 * p = reinterpret_cast<u16*>(data);
			u32 sz = dataSize / 2u;

			for( u32 i = 0u, count = 0u; i < sz; ++i ){

				newData[ count ] = (p[ i ]&redMask)>>11;
				newData[ count ] *= 8;

				newData[ count+1u ] = static_cast<u8>((p[ i ]&greenMask)>>6);
				newData[ count+1u ] *= 8;

				newData[ count+2u ] = (p[ i ]&blueMask);
				newData[ count+2u ] *= 8;

				newData[ count+3u ] = 255;
				count += 4u;
			}

			dataSize = newSize;
			gtMainSystem::getInstance()->freeMemory( (void**)&data );
			data = newData;
			pitch = width * 4u;
		}

		void __1_to_R8G8B8A8( void ){

			u8 * newData = nullptr;
			u32 newSize = dataSize * 8u * 4u;
			gtMainSystem::getInstance()->allocateMemory( (void**)&newData, newSize );

			u32 * data32 = reinterpret_cast<u32*>(newData);

			u32 sz = newSize / 4u;
			for( u32 i = 0u, count = 0u; i < sz; ){
				data32[ i+7 ] = ((data[ count ] & 1u))?0xFFFFFFFF:0x0;
				data32[ i+6u ] = ((data[ count ] & 2u)>>1u)?0xFFFFFFFF:0x0;
				data32[ i+5u ] = ((data[ count ] & 4u)>>2u)?0xFFFFFFFF:0x0;
				data32[ i+4u ] = ((data[ count ] & 8u)>>3u)?0xFFFFFFFF:0x0;
				data32[ i+3u ] = ((data[ count ] & 16u)>>4u)?0xFFFFFFFF:0x0;
				data32[ i+2u ] = ((data[ count ] & 32u)>>5u)?0xFFFFFFFF:0x0;
				data32[ i+1u ] = ((data[ count ] & 64u)>>6u)?0xFFFFFFFF:0x0;
				data32[ i+0u ] = ((data[ count ] & 128u)>>7u)?0xFFFFFFFF:0x0;
				count++;
				i += 8u;
			}

			dataSize = newSize;
			gtMainSystem::getInstance()->freeMemory( (void**)&data );
			data = newData;
			pitch = width * 4u;
		}

	};

	//	Helper functions for gtImage
	namespace image{

			// планируется написать функцию изменения масштаба картинки
		GT_FORCE_INLINE void scale( gtImage * /*i*/, f32 /*factor*/ ){
		}

			// \param i: картинка для закрашивания
			// \param saveAlpha: если \b true то альфа не закрасится
			// \param color: цвет
		GT_FORCE_INLINE void fillSolid( gtImage * i, bool saveAlpha, const gtColor& color ){
			if( i->format != gtImage::Format::FMT_R8G8B8 &&
				i->format != gtImage::Format::FMT_R8G8B8A8 )
				return;

			u8 * ptr = i->data;

			bool rgba = false;
			if( i->format == gtImage::Format::FMT_R8G8B8A8 ) rgba = true;

			for( u32 ind = 0u; ind < i->dataSize; ind += 3 ){
				*ptr = color.getAsByteRed();
				ptr++;
				*ptr = color.getAsByteGreen();
				ptr++;
				*ptr = color.getAsByteBlue();
				ptr++;

				if( rgba ){
					if( !saveAlpha )
						*ptr = color.getAsByteAlpha();
					ptr++;
					ind++;
				}
			}
		}

			// \param i: картинка для закрашивания
			// \param saveAlpha: если \b true то альфа не закрасится
			// \param firstColor: первый цвет
			// \param secondColor: второй цвет
		GT_FORCE_INLINE void fillCheckerBoard(
			gtImage * i,
			bool saveAlpha,
			const gtColor& firstColor,
			const gtColor& secondColor)
		{
			if( i->format != gtImage::Format::FMT_R8G8B8 &&
				i->format != gtImage::Format::FMT_R8G8B8A8 )
				return;

			u8 * ptr = i->data;

			bool rgba = false;
			if( i->format == gtImage::Format::FMT_R8G8B8A8 ) rgba = true;

			bool white = true;

			const gtColor * col;

			bool even = i->width % 2 ? false : true;

			u32 pitchCounter = 0u;

			for( u32 ind = 0u; ind < i->dataSize; ind += 3 ){

				white ? white = false : white = true;
				white ? col = &firstColor : col = &secondColor;

				if( even ){
					if( pitchCounter >= i->pitch ){
						pitchCounter = 0u;
						white ? white = false : white = true;
						white ? col = &firstColor : col = &secondColor;
					}
				}


				*ptr = col->getAsByteRed();
				ptr++;
				*ptr = col->getAsByteGreen();
				ptr++;
				*ptr = col->getAsByteBlue();
				ptr++;

				if( rgba ){
					if( !saveAlpha )
						*ptr = firstColor.getAsByteAlpha();
					++ptr;
					++ind;
					++pitchCounter;
				}
				pitchCounter += 3;
			}
		}

	}

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
