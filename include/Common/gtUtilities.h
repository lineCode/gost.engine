#pragma once
#ifndef __GT_UTILITIES_H__
#define __GT_UTILITIES_H__


namespace gost{
	
	//	Some helper functions here
	namespace util{
		
		GT_FORCE_INLINE bool pointInRect( const gtVector2<s16>& coord, const v4i& rect ){
			if( coord.x >= rect.x ){
				if( coord.x <= rect.z ){
					if( coord.y >= rect.y ){
						if( coord.y <= rect.w ){
							return true;
						}
					}
				}
			}
			return false;
		}

		template<typename char_type>
		bool isDigit( char_type c ){
			if( c < 0x7B ){
				if( c >= (char_type)'0' && c <= (char_type)'9' )
					return true;
			}
			return false;
		}

		template<typename char_type>
		bool isAlpha( char_type c ){
			if( c < 0x7B ){
				if( c >= (char_type)'a' && c <= (char_type)'z'
					|| c >= (char_type)'A' && c <= (char_type)'Z' )
					return true;
			}else if( c >= 0xC0 && c <= 0x2AF ){
				return true;
			}else if( c >= 0x370 && c < 0x374 ){
				return true;
			}else if( c >= 0x376 && c < 0x378 ){
				return true;
			}else if( c >= 0x376 && c < 0x378 ){
				return true;
			}else if( c == 0x37F || c == 0x386 ){
				return true;
			}else if( c > 0x387 && c < 0x38B ){
				return true;
			}else if( c == 0x38C ){
				return true;
			}else if( c > 0x38D && c < 0x3A2 ){
				return true;
			}else if( c > 0x3A2 && c < 0x482 ){
				return true;
			}else if( c > 0x489 && c < 0x530 ){
				return true;
			}else if( c > 0x530 && c < 0x557 ){
				return true;
			}else if( c > 0x560 && c < 0x588 ){
				return true;
			}else if( c >= 0x5D0 && c < 0x5EB ){
				return true;
			}/*
			 Mey be for isAlphaUnicode( c )
			 else if( c >= 0x630 && c < 0x64B ){
				return true;
			}else if( c > 0x66D && c < 0x6D4 ){
				return true;
			}else if( c > 0x6F9 && c < 0x6FD ){
				return true;
			}else if( c >= 0x710 && c < 0x730 ){
				return true;
			}else if( c > 0x74C && c < 0x7A6 ){
				return true;
			}else if( c == 0x7B0 || c == 0x7B1 ){
				return true;
			}else if( c > 0x7C9 && c < 0x7EB ){
				return true;
			}else if( c > 0x7FF && c < 0x816 ){
				return true;
			}else if( c > 0x83F && c < 0x859 ){
				return true;
			}else if( c > 0x85F && c < 0x86B ){
				return true;
			}else if( c > 0x89F && c < 0x8B5 ){
				return true;
			}else if( c > 0x8B5 && c < 0x8BE ){
				return true;
			}else if( c > 0x903 && c < 0x93A ){
				return true;
			}else if( c > 0x957 && c < 0x962 ){
				return true;
			}else if( c > 0x971 && c < 0x980 ){
				return true;
			}else if( c > 0x984 && c < 0x98D ){
				return true;
			}else if( c > 0x984 && c < 0x98D ){
				return true;
			}else if( c == 0x98F || c == 0x990 || c == 0x9B2 || c == 0x9DC || c == 0x9DD ){
				return true;
			}else if( c > 0x992 && c < 0x9A9 ){
				return true;
			}else if( c > 0x9A9 && c < 0x9B0 ){
				return true;
			}else if( c > 0x9B5 && c < 0x9BA ){
				return true;
			}else if( c > 0x9DE && c < 0x9E2 ){
				return true;
			}else if( c == 0x9F0 || c == 0x9F1 || c == 0xA0F || c == 0xA10 ){
				return true;
			}else if( c > 0xA04 && c < 0xA0B ){
				return true;
			}else if( c > 0xA13 && c < 0xA29 ){
				return true;
			}else if( c > 0xA29 && c < 0xA31 ){
				return true;
			}else if( c == 0xA32 || c == 0xA33 || c == 0xA35 || c == 0xA36 || c == 0xA38 || c == 0xA39 ){
				return true;
			}*/
			return false;
		}

		template<typename Type>
		void stringGetWords( gtArray<gtString_base<Type>> * out_array,
			const gtString_base<Type>& string,
			bool add_space = false, 
			bool add_tab = false, 
			bool add_newLine = false ){

			gtString_base<Type> word;
			u32 sz = string.size();
			for( u32 i = 0; i < sz; ++i ){
				auto ch = string[ i ];
				if( ch < 256 && (u8)ch == ' ' ){
					if( word.size() ){
						out_array->push_back( word );
						word.clear();
					}
					if( add_space ){
						out_array->push_back( gtString_base<Type>( (Type)ch ) );
					}
				}else if( ch < 256 && (u8)ch == '\t' ){
					if( word.size() ){
						out_array->push_back( word );
						word.clear();
					}
					if( add_tab ){
						out_array->push_back( gtString_base<Type>( (Type)ch ) );
					}
				}else if( ch < 256 && (u8)ch == '\n' ){
					if( word.size() ){
						out_array->push_back( word );
						word.clear();
					}
					if( add_newLine ){
						out_array->push_back( gtString_base<Type>( (Type)ch ) );
					}
				}else{
					word += ch;
				}
			}
			if( word.size() ){
				out_array->push_back( word );
				word.clear();
			}
		}

		template<typename Type>
		inline void stringFlipSlash( Type& str ){
			u32 sz = str.size();
			for( u32 i = gtConst0U; i < sz; ++i ){
				if( str[ i ] == '\\' ) 
					str[ i ] = '/';
			}
		}

			//	abc -> cba
		template<typename Type>
		inline void stringFlip( Type& str ){
			Type flippedStr;
			for( u32 i = str.size() - gtConst1U; i >= gtConst0U; --i ){
				flippedStr += str[ i ];
				if( !i ) break;
			}
			str = flippedStr;
		}

		template<typename Type>
		inline void stringPopBackBefore( Type& str, s8 c ){
			if( str.size() )
				str.pop_back();
			if( str.size() )
				for( u32 i = str.size() - gtConst1U; i >= gtConst0U; --i ){
					if( str[ i ] == c ) break;
					else str.pop_back();
					if( !i ) break;
				}
		}

		template<typename Type>
		inline Type stringGetExtension( const Type& str ){
			
			Type ret;

			for( u32 i = str.size() - gtConst1U; i >= gtConst0U; --i ){
				auto c = str[ i ];
				if( c == '/' || c == '.' )
					break;
				else ret += c;
				if( !i ) break;
			}

			stringFlip<Type>( ret );

			return ret;
		}

			//	"AbC" -> "abc"
		template<typename Type>
		inline void stringToLower( Type& str ){
			
			u32 sz = str.size();

			for( u32 i = gtConst0U; i < sz; ++i ){
				auto c = str[ i ];
				if( c <= 'Z' && c >= 'A' )
					str[ i ] += 32;
			}

		}

			//	"AbC" -> "ABC"
		template<typename Type>
		inline void stringToUpper( Type& str ){
			
			u32 sz = str.size();

			for( u32 i = gtConst0U; i < sz; ++i ){
				auto c = str[ i ];
				if( c >= 'a' && c <= 'z' )
					str[ i ] -= 32;
			}

		}

			//	" asd " -> "asd"
		template<typename Type>
		inline void stringTrimSpace( Type& str ){
			
			while( true ){
				if( str.isSpace( gtConst0U ) )
					str.pop_front();
				else break;
			}

			while( true ){
				if( str.isSpace( str.size() - gtConst1U ) )
					str.pop_back();
				else break;
			}

		}

		template<typename Type>
		inline void stringTrimFrontSpace( Type& str ){
			
			while( true ){
				if( str.isSpace( gtConst0U ) )
					str.pop_front();
				else break;
			}

		}

		template<typename Type, typename AnotherType, typename charType>
		inline void stringAppend( Type& str, AnotherType& other, charType /*c*/ ){
			u32 sz = gtConst0U;
			auto * p = &other[0];
			while( *p++ ) sz++;
			for( u32 i = gtConst0U; i < sz; ++i )
				str += static_cast<charType>( other[ i ] );
		}



		template<typename array = gtArray<gtString>>
		inline void getSupportedImportImageFormats( array& _array ){
			auto main = gtMainSystem::getInstance();
			auto ps = main->getPluginSystem();
			auto nop = ps->getNumOfPlugins();
			for( u32 i = gtConst0U; i < nop; ++i ){
				auto plg = ps->getPlugin( i );
				if( plg->getInfo().m_info.m_type == gtPluginType::Import_image ){
					auto iipl = ps->getAsPluginImportImage( plg );
					iipl->load();
					auto isz = iipl->m_extensions.size();
					for( u32 o = gtConst0U; o < isz; ++o ){
						_array.push_back( iipl->m_extensions[ o ] );
					}
					iipl->unload();
				}
			}
		}

		template<typename array = gtArray<gtString>>
		inline void getSupportedImportModelFormats( array& _array ){
			auto main = gtMainSystem::getInstance();
			auto ps = main->getPluginSystem();
			auto nop = ps->getNumOfPlugins();
			for( u32 i = gtConst0U; i < nop; ++i ){
				auto plg = ps->getPlugin( i );
				if( plg->getInfo().m_info.m_type == gtPluginType::Import_model ){
					auto iipl = ps->getAsPluginImportModel( plg );
					iipl->load();
					auto isz = iipl->m_extensions.size();
					for( u32 o = gtConst0U; o < isz; ++o ){
						_array.push_back( iipl->m_extensions[ o ] );
					}
					iipl->unload();
				}
			}
		}

		GT_FORCE_INLINE bool readTextFromFileForUnicode( const gtString& fileName, gtString& utf16 ){
			gtFile_t file = util::openFileForReadBin( fileName );
			if( !file.data() ){
				gtLogWriter::printWarning( u"Can not open file %s.", fileName.data() );
				return false;
			}

			u64 sz = file->size();
			if( sz < gtConst4U ){
				gtLogWriter::printWarning( u"Bad file %s.", fileName.data() );
				return false;
			}

			u8 bom[ gtConst3U ];
			file->read( bom, gtConst3U );
			file->seek( gtConst0U, gtFileSeekPos::Begin );

			bool isUTF8 = false;
			bool isBE = false;

			if( bom[ gtConst0U ] == 0xEF ){
				file->seek( gtConst3U, gtFileSeekPos::Begin );
				isUTF8 = true;
				sz -= gtConst3U;
			}else if( bom[ gtConst0U ] == 0xFE ){ // utf16 BE
				file->seek( gtConst2U, gtFileSeekPos::Begin );
				isBE = true;
				sz -= gtConst2U;
			}else if( bom[ gtConst0U ] == 0xFF ){
				file->seek( gtConst2U, gtFileSeekPos::Begin );
				sz -= gtConst2U;
			}else{
				// else - utf8 w/o bom
				isUTF8 = true;
			}

			gtStringA textBytes;
			textBytes.reserve( (u32)sz );
			textBytes.setSize( (u32)sz );
			file->read( (u8*)textBytes.data(), sz );

			if( !isUTF8 ){
				union{
					char16_t unicode;
					char b[ gtConst2U ];
				}un;
				for( u32 i = gtConst0U; i < sz; i += gtConst2U ){
					/*char16_t ch16 = textBytes[ i ];

					if( isBE ){
						ch16 <<= gtConst8U;
						ch16 |= textBytes[ i + gtConst1U ];
					}else{
						char16_t ch16_2 = textBytes[ i + gtConst1U ];
						ch16_2 <<= gtConst8U;
						ch16 |= ch16_2;
					}*/

					if( isBE ){
						un.b[ gtConst0U ] = textBytes[ i + gtConst1U ];
						un.b[ gtConst1U ] = textBytes[ i ];
					}else{
						un.b[ gtConst0U ] = textBytes[ i ];
						un.b[ gtConst1U ] = textBytes[ i + gtConst1U ];
					}

					utf16 += un.unicode;
				}

			}else{
				util::utf8_to_utf16( utf16, textBytes );
			}
			return true;
		}

		template<typename T>
		void stringReplaseSubString( gtString_base<T>& source, const gtString_base<T>& target, const gtString_base<T>& text ){

			gtString_base<T> result;

			u32 source_sz = source.size();
			u32 target_sz = target.size();
			u32 text_sz   = text.size();

			for( u32 i = gtConst0U; i < source_sz; ++i ){
				if( (source_sz - i) < target_sz ){
					for( u32 i2 = i; i2 < source_sz; ++i2 ){
						result += source[ i2 ];
					}
					break;
				}

				bool comp = false;
				for( u32 o = gtConst0U; o < target_sz; ++o ){
					if( source[ i + o ] == target[ o ] ){
						if( !comp ){
							comp = true;
						}
					}else{
						comp = false;
						break;
					}
				}

				if( comp ){
					for( u32 o = gtConst0U; o < text_sz; ++o ){
						result += text[ o ];
					}
					i += target_sz - gtConst1U;
				}else{
					result += source[ i ];
				}
			}

			if( result.size() ){
				source.clear();
				source.assign( result );
			}
		}
	
		template<typename type>
		type getIntFromString( const gtString& str ){
			
			type Integer = gtConst0U;

			gtString s = str;
			util::stringFlip( s );
			
			u32 mul = gtConst1U;

			auto * ptr = s.data();

			while( *ptr ){
				u32 code = *ptr - 48u;

				if( code >= gtConst0U && code <= gtConst9U ){
					Integer += mul * code;
					mul *= 10u;
				}
				++ptr;
			}

			return Integer;
		}


		template<typename char_type>
		bool isSpace( char_type c ){
			if( c == (char_type)' ' ) return true;
			if( c == (char_type)'\r' ) return true;
			if( c == (char_type)'\n' ) return true;
			if( c == (char_type)'\t' ) return true;
			return false;
		}

		template<typename SourceType, typename TargetType>
		SourceType* getWordFromString( SourceType* source, gtString_base<TargetType>* target ){

			while( *source ){
				if( isSpace( *source ) || *source == (SourceType)',' ){
					++source;
				}else break;
			}

			while( *source ){

				if( isSpace( *source ) || *source == (SourceType)',' ){
					++source;
					break;
				}

				*target += (TargetType)*source;

				++source;
			}
			return source;
		}

		template<typename type>
		void getVec4iFromString( const gtString& str, type* vec ){
			auto * ptr = str.data();

			gtString word;

			u32 i = gtConst0U;

			while( *ptr ){

				word.clear();

				ptr = getWordFromString( ptr, &word );

				if( word.size() ){
					switch( i ){
					case gtConst0U:
						vec->x = (u16)getIntFromString<u32>( word );
						break;
					case gtConst1U:
						vec->y = (u16)getIntFromString<u32>( word );
						break;
					case gtConst2U:
						vec->z = (u16)getIntFromString<u32>( word );
						break;
					case gtConst3U:
						vec->w = (u16)getIntFromString<u32>( word );
						break;
					}
					++i;
				}
			}
		}

		GT_FORCE_INLINE void getFilesFromDir( gtArray<gtString>* out, const gtString& dir ){
			if( !gtFileSystem::existDir( dir ) ){
				gtLogWriter::printWarning( u"Can not scan dir. Dir not exist." );
				return;
			}
			gtFileSystem::scanDirBegin( dir );
			gtFileSystem::DirObject obj;
			while( gtFileSystem::getDirObject(&obj)){
				if( obj.type == gtFileSystem::DirObjectType::file )
					out->push_back( gtString( (char16_t*)obj.path ) );
			}
			gtFileSystem::scanDirEnd();
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