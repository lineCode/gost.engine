#pragma once
#ifndef __GT_IMAGE_H__
#define __GT_IMAGE_H__


namespace gost{

	class gtMainSystem;

	enum class gtImageFormat{
		One_bit,		// white or black
			
		R1,
			
		A1R5G5B5,		// 1 11111 11111 11111
		X1R5G5B5,		// x 11111 11111 11111
			
		A4R4G4B4,		// 1111 1111 1111 1111
		X4R4G4B4,		// xxxx 1111 1111 1111
		B4G4R4A4,
			
		R5G6B5,			// 11111 111111 11111
		B5G6R5,
		B5G5R5A1,
			
		A8,
			
		R8,
		R8G8,
		R8G8B8,			// u8 u8 u8
		R8G8B8A8,		// u8 u8 u8 u8
		R8G8B8G8,
		G8R8G8B8,
		B8G8R8A8,
		B8G8R8X8,
		X8R8G8B8,		// u8 u8 u8 u8
		A8R8G8B8,		// u8 u8 u8 u8
			
		R9G9B9E5,
			
		R10G10B10A2,

		R11G11B10,

		R16,
		R16G16,
		R16G16B16A16,
			
		R24G8,
		R24X8,
		X24G8,
			
		R32,
		R32G32,
		R32G8X24,
		R32G32B32,
		R32G32B32A32,	

		//	OpenGL
		COMPRESSED_RGBA_S3TC_DXT1 = 0x83F1, // dds dxt1
		COMPRESSED_RGBA_S3TC_DXT3 = 0x83F2, // dds dxt3
		COMPRESSED_RGBA_S3TC_DXT5 = 0x83F3	// dds dxt5

	};
		// software image
	class gtImage : public gtRefObject{
	public:

			// c-tor
		gtImage():
			format( gtImageFormat::R8G8B8A8 ),
			width( gtConst0U ),
			height( gtConst0U ),
			bits( gtConst32U ),
			mipCount( gtConst1U ),
			data( nullptr ),
			dataSize( gtConst0U ),
			pitch( gtConst0U ),
			frames( gtConst1U )
		{}

			// d-tor
		virtual ~gtImage(){
			if( data ){
				gtMainSystem::getInstance()->freeMemory( (void**)&data );
				data = nullptr;
			}
		}


		gtImageFormat format;

		u32		width;

		u32		height;

		u32		bits;

		u32		mipCount;

		u8	*	data;

		u32		dataSize;

		u32		pitch;

		u32		frames;

		void	convert( gtImageFormat newFormat ){
			if( newFormat == format ) return;

			if( format == gtImageFormat::R8G8B8 ){
				if( newFormat == gtImageFormat::A8R8G8B8 ){
					__R8G8B8_to_A8R8G8B8();
				}else if( newFormat == gtImageFormat::R8G8B8A8 ){
					__R8G8B8_to_R8G8B8A8();
				}else if( newFormat == gtImageFormat::X8R8G8B8 ){
					__R8G8B8_to_A8R8G8B8();
				}

			//	==================================================
			}else if( format == gtImageFormat::A8R8G8B8 ){
				if( newFormat == gtImageFormat::R8G8B8 ){
					__A8R8G8B8_to_R8G8B8();
				}else if( newFormat == gtImageFormat::R8G8B8A8 ){
					__A8R8G8B8_to_R8G8B8A8();
				}else if( newFormat == gtImageFormat::X8R8G8B8 ){
					__A8R8G8B8_to_X8R8G8B8();
				}
			//	==================================================
			}else if( format == gtImageFormat::X8R8G8B8 ){
				if( newFormat == gtImageFormat::R8G8B8 ){
					__A8R8G8B8_to_R8G8B8();
				}else if( newFormat == gtImageFormat::R8G8B8A8 ){
					__A8R8G8B8_to_R8G8B8A8();
				}else if( newFormat == gtImageFormat::A8R8G8B8 ){
					//__X8R8G8B8_to_A8R8G8B8();
				}

			//	==================================================
			}else if( format == gtImageFormat::R8G8B8A8 ){
				if( newFormat == gtImageFormat::R8G8B8 ){
					__R8G8B8A8_to_R8G8B8();
				}else if( newFormat == gtImageFormat::X8R8G8B8 ){
					__R8G8B8A8_to_X8R8G8B8();
				}else if( newFormat == gtImageFormat::A8R8G8B8 ){
					__R8G8B8A8_to_A8R8G8B8();
				}

				//	==================================================
			}else if( format == gtImageFormat::X4R4G4B4 ){
				if( newFormat == gtImageFormat::R8G8B8A8 ){
					__X4R4G4B4_to_R8G8B8A8();
				}else if( newFormat == gtImageFormat::A8R8G8B8 ){
					__X4R4G4B4_to_A8R8G8B8();
				}else if( newFormat == gtImageFormat::X8R8G8B8 ){
					__X4R4G4B4_to_A8R8G8B8();
				}else if( newFormat == gtImageFormat::R8G8B8 ){
					__X4R4G4B4_to_R8G8B8();
				}

				//	==================================================
			}else if( format == gtImageFormat::X1R5G5B5 ){
				if( newFormat == gtImageFormat::R8G8B8A8 ){
					__X1R5G5B5_to_R8G8B8A8();
				}

				//	==================================================
			}else if( format == gtImageFormat::A1R5G5B5 ){
				if( newFormat == gtImageFormat::R8G8B8A8 ){
					__A1R5G5B5_to_R8G8B8A8();
				}

			}else if( format == gtImageFormat::R5G6B5 ){
				if( newFormat == gtImageFormat::R8G8B8A8 ){
					__R5G6B5_to_R8G8B8A8();
				}

			}else if( format == gtImageFormat::One_bit ){
				if( newFormat == gtImageFormat::R8G8B8A8 ){
					__1_to_R8G8B8A8();
				}

					//	==================================================
			}else if( format == gtImageFormat::A4R4G4B4 ){
				if( newFormat == gtImageFormat::R8G8B8A8 ){
					__X4R4G4B4_to_R8G8B8A8();
				}else if( newFormat == gtImageFormat::A8R8G8B8 ){
					__X4R4G4B4_to_A8R8G8B8();
				}else if( newFormat == gtImageFormat::X8R8G8B8 ){
					__X4R4G4B4_to_A8R8G8B8();
				}else if( newFormat == gtImageFormat::R8G8B8 ){
					__X4R4G4B4_to_R8G8B8();
				}
			}

			format = newFormat;
		}

		void	makeAlphaFromBlack(){
			if( format == gtImageFormat::R8G8B8A8){
				for( u32 i = gtConst0U; i < dataSize; ){
					u8 r = data[ i ];
					u8 g = data[ i+gtConst1U ];
					u8 b = data[ i+gtConst2U ];
					data[ i+gtConst3U ] = (u8)std::ceil((f32)(r+g+b)*0.33333f);
					i += gtConst4U;
				}
			}
		}

			// rgb -> bgr, rgba -> abgr, rgbx -> bgrx...
		void	flipPixel(){
			if( !data ) return;
			switch( format ){
			case gtImageFormat::One_bit:
				break;

			case gtImageFormat::A1R5G5B5:
				__flip_pixel_A1R5G5B5();
				break;
			case gtImageFormat::X1R5G5B5:
				__flip_pixel_X1R5G5B5();
				break;
			case gtImageFormat::A4R4G4B4:
				__flip_pixel_X4R4G4B4();
				break;
			case gtImageFormat::X4R4G4B4:
				__flip_pixel_X4R4G4B4();
				break;
			case gtImageFormat::R5G6B5:
				__flip_pixel_R5G6B5();
				break;
			case gtImageFormat::X8R8G8B8:
				__flip_pixel_X8R8G8B8();
				break;
			case gtImageFormat::A8R8G8B8:
				__flip_pixel_A8R8G8B8();
				break;
			case gtImageFormat::R8G8B8A8:
				__flip_pixel_A8R8G8B8();
				break;
			case gtImageFormat::R8G8B8:
				__flip_pixel_R8G8B8();
				break;
			case gtImageFormat::COMPRESSED_RGBA_S3TC_DXT1:
				break;
			case gtImageFormat::COMPRESSED_RGBA_S3TC_DXT3:
				break;
			case gtImageFormat::COMPRESSED_RGBA_S3TC_DXT5:
				break;
			default:
				break;
			}
		}

		void flipVertical(){
			u8 * line = nullptr;
			gtMainSystem::getInstance()->allocateMemory( (void**)&line, pitch );

			u8 * p_Up = &data[ gtConst0U ];
			u8 * p_Down = &data[ pitch * height - pitch ];

			for( u32 i = gtConst0U; i < height / gtConst2U; ++i ){

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
		void __flip_pixel_A8R8G8B8(){
			for( u32 i = gtConst0U; i < dataSize; ){
				u8 a = data[ i ];
				u8 r = data[ i+gtConst1U ];
				u8 g = data[ i+gtConst2U ];
				u8 b = data[ i+gtConst3U ];
				data[ i ] = b;
				data[ i+gtConst1U ] = g;
				data[ i+gtConst2U ] = r;
				data[ i+gtConst3U ] = a;
				i += gtConst4U;
			}
		}

		void __flip_pixel_X8R8G8B8(){
			for( u32 i = gtConst0U; i < dataSize; ){
				u8 r = data[ i+gtConst1U ];
				u8 g = data[ i+gtConst2U ];
				u8 b = data[ i+gtConst3U ];
				data[ i+gtConst1U ] = b;
				data[ i+gtConst2U ] = g;
				data[ i+gtConst3U ] = r;
				i += gtConst4U;
			}
		}

		void __flip_pixel_X4R4G4B4(){
			const u32 redMask = 3840u;
			const u32 greenMask = 240u;
			const u32 blueMask = 15u;

			u16 * p = reinterpret_cast<u16*>(data);
			u32 sz = dataSize / gtConst2U;
			for( u32 i = gtConst0U; i < sz; ++i ){
				u16 pixel	= p[ i ];
				u16 red		= (pixel & redMask)>>gtConst8U;
				u16 green	= (pixel & greenMask)>>gtConst4U;
				u16 blue	= (pixel & blueMask);

				p[ i ] = red;
				p[ i ] |= green<<gtConst4U;
				p[ i ] |= blue<<gtConst8U;
			}
		}

		void __flip_pixel_R5G6B5(){
			const u32 redMask = 63488u;
			const u32 greenMask = 2016u;
			const u32 blueMask = 31u;

			u16 * p = reinterpret_cast<u16*>(data);
			u32 sz = dataSize / gtConst2U;
			for( u32 i = gtConst0U; i < sz; ++i ){
				u16 pixel	= p[ i ];
				u16 red		= (pixel & redMask)>>11;
				u16 green	= (pixel & greenMask)>>gtConst6U;
				u16 blue	= (pixel & blueMask);

				p[ i ] = red;
				p[ i ] |= green<<gtConst6U;
				p[ i ] |= blue<<gtConst11U;
			}
		}

		void __flip_pixel_X1R5G5B5(){
			const u32 redMask = 31744u;
			const u32 greenMask = 992u;
			const u32 blueMask = 31u;

			u16 * p = reinterpret_cast<u16*>(data);
			u32 sz = dataSize / gtConst2U;
			for( u32 i = gtConst0U; i < sz; ++i ){
				u16 pixel	= p[ i ];
				u16 red		= (pixel & redMask)>>gtConst10U;
				u16 green	= (pixel & greenMask)>>gtConst5U;
				u16 blue	= (pixel & blueMask);

				p[ i ] = red;
				p[ i ] |= green<<gtConst5U;
				p[ i ] |= blue<<gtConst10U;
			}
		}

		void __flip_pixel_A1R5G5B5(){
			const u32 alphaMask = 32768u;
			const u32 redMask = 31744u;
			const u32 greenMask = 992u;
			const u32 blueMask = 31u;

			u16 * p = reinterpret_cast<u16*>(data);
			u32 sz = dataSize / gtConst2U;
			for( u32 i = gtConst0U; i < sz; ++i ){
				u16 pixel	= p[ i ];
				u16 alpha	= (pixel & alphaMask)>>gtConst15U;
				u16 red		= (pixel & redMask)>>gtConst10U;
				u16 green	= (pixel & greenMask)>>gtConst5U;
				u16 blue	= (pixel & blueMask);

				p[ i ] = alpha;
				p[ i ] |= red<<gtConst5U;
				p[ i ] |= green<<gtConst10U;
				p[ i ] |= blue<<gtConst15U;
			}
		}

		void __flip_pixel_R8G8B8(){
			for( u32 i = gtConst0U; i < dataSize; ){
				u8 red = data[ i ];
				data[ i ] = data[ i+gtConst2U ];
				data[ i+gtConst2U ] = red;
				i += gtConst3U;
			}
		}

		//	===================		CONVERT		=======================================
		void __R8G8B8_to_A8R8G8B8(){
			u8 * newData = nullptr;

			u32 newSize = dataSize + (dataSize / gtConst3U);

			gtMainSystem::getInstance()->allocateMemory( (void**)&newData, newSize );

			u32 * data32 = reinterpret_cast<u32*>(newData);

			for( u32 i = gtConst0U; i < dataSize; ++data32 ){
				*data32 = GT_MAKEFOURCC( gtConst255, data[ i ], data[ i+gtConst1U ], data[ i+gtConst2U ] );
				i += gtConst3U;
			}

			dataSize = newSize;
			gtMainSystem::getInstance()->freeMemory( (void**)&data );
			data = newData;
			pitch = width * gtConst4U;
		}


		void __A8R8G8B8_to_R8G8B8(){
			u8 * newData = nullptr;

			u32 newSize = dataSize - (dataSize / gtConst4U);

			gtMainSystem::getInstance()->allocateMemory( (void**)&newData, newSize );

			for( u32 i = gtConst0U, count = gtConst0U; i < dataSize; ){
				newData[ count ] = data[ i+gtConst1U ];
				newData[ count+1 ] = data[ i+gtConst2U ];
				newData[ count+2 ] = data[ i+gtConst3U ];

				i += gtConst4U;
				count += gtConst3U;
			}

			dataSize = newSize;
			gtMainSystem::getInstance()->freeMemory( (void**)&data );
			data = newData;
			pitch = width * gtConst3U;
		}


		void __A8R8G8B8_to_R8G8B8A8(){
			for( u32 i = gtConst0U; i < dataSize; ){
				u8 a = data[ i ];
				u8 r = data[ i+gtConst1U ];
				u8 g = data[ i+gtConst2U ];
				u8 b = data[ i+gtConst3U ];
				data[ i ] = r;
				data[ i+gtConst1U ] = g;
				data[ i+gtConst2U ] = b;
				data[ i+gtConst3U ] = a;
				i += gtConst4U;
			}
		}

		void __A8R8G8B8_to_X8R8G8B8(){
			for( u32 i = gtConst0U; i < dataSize; ){
				data[ i ] = gtConst255;
				i += gtConst4U;
			}
		}

		/*void __X8R8G8B8_to_A8R8G8B8(){
			for( u32 i = gtConst0U; i < dataSize; ){
				data[ i ] = 255;
				i += gtConst4U;
			}
		}*/

		void __R8G8B8_to_R8G8B8A8(){
			u8 * newData = nullptr;

			u32 newSize = dataSize + (dataSize / gtConst3U);

			gtMainSystem::getInstance()->allocateMemory( (void**)&newData, newSize );

			u32 * data32 = reinterpret_cast<u32*>(newData);

			for( u32 i = gtConst0U; i < dataSize; ++data32 ){
				*data32 = GT_MAKEFOURCC( data[ i ], data[ i+gtConst1U ], data[ i+gtConst2U ], gtConst255 );
				i += gtConst3U;
			}

			dataSize = newSize;
			gtMainSystem::getInstance()->freeMemory( (void**)&data );
			data = newData;
			pitch = width * gtConst4U;
		}

		void __R8G8B8A8_to_R8G8B8(){
			u8 * newData = nullptr;

			u32 newSize = dataSize - (dataSize / gtConst4U);

			gtMainSystem::getInstance()->allocateMemory( (void**)&newData, newSize );

			for( u32 i = gtConst0U, count = gtConst0U; i < dataSize; ){
				newData[ count ] = data[ i ];
				newData[ count+gtConst1U ] = data[ i+gtConst1U ];
				newData[ count+gtConst2U ] = data[ i+gtConst2U ];

				i += gtConst4U;
				count += gtConst3U;
			}

			dataSize = newSize;
			gtMainSystem::getInstance()->freeMemory( (void**)&data );
			data = newData;
			pitch = width * gtConst3U;
		}

		void __R8G8B8A8_to_X8R8G8B8(){
			for( u32 i = gtConst0U; i < dataSize; ){
				u8 r = data[ i ];
				u8 g = data[ i+gtConst1U ];
				u8 b = data[ i+gtConst2U ];
				data[ i ] = gtConst255;
				data[ i+gtConst1U ] = r;
				data[ i+gtConst2U ] = g;
				data[ i+gtConst3U ] = b;
				i += gtConst4U;
			}
		}

		void __R8G8B8A8_to_A8R8G8B8(){
			for( u32 i = gtConst0U; i < dataSize; ){
				u8 r = data[ i ];
				u8 g = data[ i+gtConst1U ];
				u8 b = data[ i+gtConst2U ];
				u8 a = data[ i+gtConst3U ];
				data[ i ] = a;
				data[ i+gtConst1U ] = r;
				data[ i+gtConst2U ] = g;
				data[ i+gtConst3U ] = b;
				i += gtConst4U;
			}
		}

		void __X4R4G4B4_to_R8G8B8A8(){
			u8 * newData = nullptr;

			u32 newSize = dataSize * gtConst2U;

			gtMainSystem::getInstance()->allocateMemory( (void**)&newData, newSize );

			const u32 redMask = 3840u;
			const u32 greenMask = 240u;
			const u32 blueMask = gtConst15U;

			u16 * p = reinterpret_cast<u16*>(data);
			u32 sz = dataSize / gtConst2U;

			for( u32 i = gtConst0U, count = gtConst0U; i < sz; ++i ){

				newData[ count ] = (p[ i ]&redMask)>>gtConst8U;
				newData[ count ] *= gtConst17U;

				newData[ count+gtConst1U ] = (p[ i ]&greenMask)>>gtConst4U;
				newData[ count+gtConst1U ] *= gtConst17U;

				newData[ count+gtConst2U ] = (p[ i ]&blueMask);
				newData[ count+gtConst2U ] *= gtConst17U;

				newData[ count+gtConst3U ] = gtConst255;
				count += gtConst4U;
			}

			dataSize = newSize;
			gtMainSystem::getInstance()->freeMemory( (void**)&data );
			data = newData;
			pitch = width * gtConst4U;
		}

		void __X4R4G4B4_to_A8R8G8B8(){
			u8 * newData = nullptr;

			u32 newSize = dataSize * gtConst2U;

			gtMainSystem::getInstance()->allocateMemory( (void**)&newData, newSize );

			const u32 redMask = 3840u;
			const u32 greenMask = 240u;
			const u32 blueMask = gtConst15U;

			u16 * p = reinterpret_cast<u16*>(data);
			u32 sz = dataSize / gtConst2U;

			for( u32 i = gtConst0U, count = gtConst0U; i < sz; ++i ){

				newData[ count ] = gtConst255;

				newData[ count+gtConst1U ] = (p[ i ]&redMask)>>gtConst8U;
				newData[ count+gtConst1U ] *= gtConst17U;

				newData[ count+gtConst2U ] = (p[ i ]&greenMask)>>gtConst4U;
				newData[ count+gtConst2U ] *= gtConst17U;

				newData[ count+gtConst3U ] = (p[ i ]&blueMask);
				newData[ count+gtConst3U ] *= gtConst17U;

				count += gtConst4U;
			}

			dataSize = newSize;
			gtMainSystem::getInstance()->freeMemory( (void**)&data );
			data = newData;
			pitch = width * gtConst4U;
		}

		void __X4R4G4B4_to_R8G8B8(){
			u8 * newData = nullptr;

			u32 sz = dataSize / gtConst2U;

			u32 newSize = dataSize + sz;

			gtMainSystem::getInstance()->allocateMemory( (void**)&newData, newSize );

			const u32 redMask = 3840u;
			const u32 greenMask = 240u;
			const u32 blueMask = gtConst15U;

			u16 * p = reinterpret_cast<u16*>(data);

			for( u32 i = gtConst0U, count = gtConst0U; i < sz; ++i ){

				newData[ count ] = (p[ i ]&redMask)>>gtConst8U;
				newData[ count ] *= gtConst17U;

				newData[ count+gtConst1U ] = (p[ i ]&greenMask)>>gtConst4U;
				newData[ count+gtConst1U ] *= gtConst17U;

				newData[ count+gtConst2U ] = (p[ i ]&blueMask);
				newData[ count+gtConst2U ] *= gtConst17U;

				count += gtConst3U;
			}

			dataSize = newSize;
			gtMainSystem::getInstance()->freeMemory( (void**)&data );
			data = newData;
			pitch = width * gtConst3U;
		}

		void __X1R5G5B5_to_R8G8B8A8(){
			u8 * newData = nullptr;

			u32 newSize = dataSize * gtConst2U;

			gtMainSystem::getInstance()->allocateMemory( (void**)&newData, newSize );

			const u32 redMask = 31744u;
			const u32 greenMask = 992u;
			const u32 blueMask = 31u;

			u16 * p = reinterpret_cast<u16*>(data);
			u32 sz = dataSize / gtConst2U;

			for( u32 i = gtConst0U, count = gtConst0U; i < sz; ++i ){

				newData[ count ] = (p[ i ]&redMask)>>gtConst10U;
				newData[ count ] *= gtConst8U;

				newData[ count+gtConst1U ] = static_cast<u8>((p[ i ]&greenMask)>>gtConst5U);
				newData[ count+gtConst1U ] *= gtConst8U;

				newData[ count+gtConst2U ] = (p[ i ]&blueMask);
				newData[ count+gtConst2U ] *= gtConst8U;

				newData[ count+gtConst3U ] = gtConst255;
				count += gtConst4U;
			}

			dataSize = newSize;
			gtMainSystem::getInstance()->freeMemory( (void**)&data );
			data = newData;
			pitch = width * gtConst4U;
		}

		void __A1R5G5B5_to_R8G8B8A8(){
			u8 * newData = nullptr;

			u32 newSize = dataSize * gtConst2U;

			gtMainSystem::getInstance()->allocateMemory( (void**)&newData, newSize );

			const u32 alphaMask = 32768u;
			const u32 redMask = 31744u;
			const u32 greenMask = 992u;
			const u32 blueMask = 31u;

			u16 * p = reinterpret_cast<u16*>(data);
			u32 sz = dataSize / gtConst2U;

			for( u32 i = gtConst0U, count = gtConst0U; i < sz; ++i ){

				newData[ count ] = (p[ i ]&redMask)>>gtConst10U;
				newData[ count ] *= gtConst8U;

				newData[ count+gtConst1U ] = static_cast<u8>((p[ i ]&greenMask)>>gtConst5U);
				newData[ count+gtConst1U ] *= gtConst8U;

				newData[ count+gtConst2U ] = (p[ i ]&blueMask);
				newData[ count+gtConst2U ] *= gtConst8U;

				newData[ count+gtConst3U ] = ((p[ i ]&alphaMask)>>gtConst15U) ? gtConst255 : gtConst0U;
				count += gtConst4U;
			}

			dataSize = newSize;
			gtMainSystem::getInstance()->freeMemory( (void**)&data );
			data = newData;
			pitch = width * gtConst4U;
		}

		void __R5G6B5_to_R8G8B8A8(){
			u8 * newData = nullptr;

			u32 newSize = dataSize * gtConst2U;

			gtMainSystem::getInstance()->allocateMemory( (void**)&newData, newSize );

			const u32 redMask = 63488u;
			const u32 greenMask = 2016u;
			const u32 blueMask = 31u;

			u16 * p = reinterpret_cast<u16*>(data);
			u32 sz = dataSize / gtConst2U;

			for( u32 i = gtConst0U, count = gtConst0U; i < sz; ++i ){

				newData[ count ] = (p[ i ]&redMask)>>gtConst11U;
				newData[ count ] *= gtConst8U;

				newData[ count+gtConst1U ] = static_cast<u8>((p[ i ]&greenMask)>>gtConst6U);
				newData[ count+gtConst1U ] *= gtConst8U;

				newData[ count+gtConst2U ] = (p[ i ]&blueMask);
				newData[ count+gtConst2U ] *= gtConst8U;

				newData[ count+gtConst3U ] = gtConst255;
				count += gtConst4U;
			}

			dataSize = newSize;
			gtMainSystem::getInstance()->freeMemory( (void**)&data );
			data = newData;
			pitch = width * gtConst4U;
		}

		void __1_to_R8G8B8A8(){

			u8 * newData = nullptr;
			u32 newSize = dataSize * gtConst8U * gtConst4U;
			gtMainSystem::getInstance()->allocateMemory( (void**)&newData, newSize );

			u32 * data32 = reinterpret_cast<u32*>(newData);

			u32 sz = newSize / gtConst4U;
			for( u32 i = gtConst0U, count = gtConst0U; i < sz; ){
				data32[ i+gtConst7U ] = ((data[ count ] & gtConst1U))?gtConst0xFFFFFFFF:gtConst0U;
				data32[ i+gtConst6U ] = ((data[ count ] & gtConst2U)>>gtConst1U)?gtConst0xFFFFFFFF:gtConst0U;
				data32[ i+gtConst5U ] = ((data[ count ] & gtConst4U)>>gtConst2U)?gtConst0xFFFFFFFF:gtConst0U;
				data32[ i+gtConst4U ] = ((data[ count ] & gtConst8U)>>gtConst3U)?gtConst0xFFFFFFFF:gtConst0U;
				data32[ i+gtConst3U ] = ((data[ count ] & gtConst16U)>>gtConst4U)?gtConst0xFFFFFFFF:gtConst0U;
				data32[ i+gtConst2U ] = ((data[ count ] & gtConst32U)>>gtConst5U)?gtConst0xFFFFFFFF:gtConst0U;
				data32[ i+gtConst1U ] = ((data[ count ] & gtConst64U)>>gtConst6U)?gtConst0xFFFFFFFF:gtConst0U;
				data32[ i+gtConst0U ] = ((data[ count ] & gtConst128U)>>gtConst7U)?gtConst0xFFFFFFFF:gtConst0U;
				count++;
				i += gtConst8U;
			}

			dataSize = newSize;
			gtMainSystem::getInstance()->freeMemory( (void**)&data );
			data = newData;
			pitch = width * gtConst4U;
		}

	};

	//	Helper functions for gtImage
	namespace image{

		GT_FORCE_INLINE void scale( gtImage * /*i*/, f32 /*factor*/ ){
		}

		GT_FORCE_INLINE void fillSolid( gtImage * i, bool saveAlpha, const gtColor& color ){
			if( i->format != gtImageFormat::R8G8B8 &&
				i->format != gtImageFormat::R8G8B8A8 )
				return;

			u8 * ptr = i->data;

			bool rgba = false;
			if( i->format == gtImageFormat::R8G8B8A8 ) rgba = true;

			for( u32 ind = gtConst0U; ind < i->dataSize; ind += 3 ){
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

		GT_FORCE_INLINE void fillCheckerBoard(
			gtImage * i,
			bool saveAlpha,
			const gtColor& firstColor,
			const gtColor& secondColor)
		{
			if( i->format != gtImageFormat::R8G8B8 &&
				i->format != gtImageFormat::R8G8B8A8 )
				return;

			u8 * ptr = i->data;

			bool rgba = false;
			if( i->format == gtImageFormat::R8G8B8A8 ) rgba = true;

			bool white = true;

			const gtColor * col;

			bool even = i->width % gtConst2U ? false : true;

			u32 pitchCounter = gtConst0U;

			for( u32 ind = gtConst0U; ind < i->dataSize; ind += gtConst3U ){

				white ? white = false : white = true;
				white ? col = &firstColor : col = &secondColor;

				if( even ){
					if( pitchCounter >= i->pitch ){
						pitchCounter = gtConst0U;
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
				pitchCounter += gtConst3U;
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
