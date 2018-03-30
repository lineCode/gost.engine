﻿/*!	GOST
	\file gtUtilities.h
	\brief Usefull functions

	Тут будут лежать вспомогательные функции, которые могут пригодиться в любом месте.
*/

#pragma once
#ifndef __GT_UTILITIES_H__
#define __GT_UTILITIES_H__ ///< include guard


namespace gost{
	
	//	Some helper functions here
	namespace util{
		
			///	изменяет символ \ на /
			///	\param str: строка в котором нужно изменить символ
		template<typename Type>
		inline void stringFlipSlash( Type& str ){
			u32 sz = str.size();
			for( u32 i = 0u; i < sz; ++i ){
				if( str[ i ] == '\\' ) 
					str[ i ] = '/'; // стандартный набор символов подходит для всех версий char
			}
		}

			///	переворачивает строку (abc -> cba)
			///	\param str: строка для переворачивания
		template<typename Type>
		inline void stringFlip( Type& str ){
			Type flippedStr;
			for( u32 i = str.size() - 1u; i >= 0u; --i ){
				flippedStr += str[ i ];
				if( !i ) break;
			}
			str = flippedStr;
		}

			///	удаляет последний символ пока не встретится указанный
			///	\param str: строка в которой нужно удалять символы
			///	\param с: символ, который остановит удаление
		template<typename Type>
		inline void stringPopBackBefore( Type& str, s8 c ){
			if( str.size() )
				str.pop_back();
			if( str.size() )
				for( u32 i = str.size() - 1u; i >= 0u; --i ){
					if( str[ i ] == c ) break;
					else str.pop_back();
					if( !i ) break;
				}
		}

			//	возвратит расширение - последнюю часть строки после точки (слэши должны быть такими /)
			//	\param str: строка содержащая часть с точкой
			//	\return Строка содержащая расширение
		template<typename Type>
		inline Type stringGetExtension( const Type& str ){
			
			// по сути нужно всего лишь добавлять символы с конца
			//	пока не будет встречена точка либо slash
			Type ret;

			for( u32 i = str.size() - 1u; i >= 0u; --i ){
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
			//	\param str: строка для изменения
		template<typename Type>
		inline void stringToLower( Type& str ){
			
			u32 sz = str.size();

			for( u32 i = 0u; i < sz; ++i ){
				auto c = str[ i ];
				if( c <= 'Z' && c >= 'A' )
					str[ i ] += 32;
			}

		}

			///	"AbC" -> "ABC"
			///	\param str: строка для изменения
		template<typename Type>
		inline void stringToUpper( Type& str ){
			
			u32 sz = str.size();

			for( u32 i = 0u; i < sz; ++i ){
				auto c = str[ i ];
				if( c >= 'a' && c <= 'z' )
					str[ i ] -= 32;
			}

		}

			///	" asd " -> "asd"
		template<typename Type>
		inline void stringTrimSpace( Type& str ){
			
			while( true ){
				if( str.isSpace( 0u ) )
					str.pop_front();
				else break;
			}

			while( true ){
				if( str.isSpace( str.size() - 1u ) )
					str.pop_back();
				else break;
			}

		}

		template<typename Type>
		inline void stringTrimFrontSpace( Type& str ){
			
			while( true ){
				if( str.isSpace( 0u ) )
					str.pop_front();
				else break;
			}

		}

			///	добаляет к str строку иного типа
			///	\param str: строка в которую нужно добавить
			///	\param other: строка которую нужно добавить
			///	\param c: указывается для приведения типа
		template<typename Type, typename AnotherType, typename charType>
		inline void stringAppend( Type& str, AnotherType& other, charType /*c*/ ){
			u32 sz = 0u;
			auto * p = &other[0];
			while( *p++ ) sz++;
			for( u32 i = 0u; i < sz; ++i )
				str += static_cast<charType>( other[ i ] );
		}



		template<typename array = gtArray<gtString>>
		inline void getSupportedImportImageFormats( array& _array ){
			auto main = gtMainSystem::getInstance();
			auto ps = main->getPluginSystem();
			auto nop = ps->getNumOfPlugins();
			for( u32 i = 0u; i < nop; ++i ){
				auto plg = ps->getPlugin( i );
				if( plg->getInfo().m_info.m_type == gtPluginType::import_image ){
					auto iipl = ps->getAsPluginImportImage( plg );
					iipl->load();
					auto isz = iipl->m_extensions.size();
					for( u32 o = 0u; o < isz; ++o ){
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
			for( u32 i = 0u; i < nop; ++i ){
				auto plg = ps->getPlugin( i );
				if( plg->getInfo().m_info.m_type == gtPluginType::import_model ){
					auto iipl = ps->getAsPluginImportModel( plg );
					iipl->load();
					auto isz = iipl->m_extensions.size();
					for( u32 o = 0u; o < isz; ++o ){
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
			if( sz < 4u ){
				gtLogWriter::printWarning( u"Bad file %s.", fileName.data() );
				return false;
			}

			u8 bom[ 3u ];
			file->read( bom, 3u );
			file->seek( 0u, gtFile::SeekPos::ESP_BEGIN );

			bool isUTF8 = false;
			bool isBE = false;

			if( bom[ 0u ] == 0xEF ){
				file->seek( 3u, gtFile::SeekPos::ESP_BEGIN );
				isUTF8 = true;
				sz -= 3u;
			}else if( bom[ 0u ] == 0xFE ){ // utf16 BE
				file->seek( 2u, gtFile::SeekPos::ESP_BEGIN );
				isBE = true;
				sz -= 2u;
			}else if( bom[ 0u ] == 0xFF ){
				file->seek( 2u, gtFile::SeekPos::ESP_BEGIN );
				sz -= 2u;
			}// else - utf8 w/o bom

			gtStringA textBytes;
			textBytes.reserve( (u32)sz );
			textBytes.setSize( (u32)sz );
			file->read( (u8*)textBytes.data(), sz );

			if( !isUTF8 ){

				for( u32 i = 0u; i < sz; i += 2u ){
					char16_t ch16 = textBytes[ i ];

					if( isBE ){
						ch16 <<= 8u;
						ch16 |= textBytes[ i + 1u ];
					}else{
						char16_t ch16_2 = textBytes[ i + 1u ];
						ch16_2 <<= 8u;
						ch16 |= ch16_2;
					}

					utf16 += ch16;
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

			for( u32 i = 0u; i < source_sz; ++i ){
				if( (source_sz - i) < target_sz ){
					for( u32 i2 = i; i2 < source_sz; ++i2 ){
						result += source[ i2 ];
					}
					break;
				}

				bool comp = false;
				for( u32 o = 0u; o < target_sz; ++o ){
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
					for( u32 o = 0u; o < text_sz; ++o ){
						result += text[ o ];
					}
					i += target_sz - 1u;
				}else{
					result += source[ i ];
				}
			}

			if( result.size() ){
				source.clear();
				source.assign( result );
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