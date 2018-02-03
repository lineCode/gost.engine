/*!	GOST
	\file gtUtilities.h
	\brief Usefull functions

	Тут будут лежать вспомогательные функции, которые могут пригодиться в любом месте.
*/

#pragma once
#ifndef __GT_UTILITIES_H__
#define __GT_UTILITIES_H__ ///< include guard


namespace gost{
	
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

			///	возвратит расширение - последнюю часть строки после точки (слэши должны быть такими /)
			///	\param str: строка содержащая часть с точкой
			///	\return Строка содержащая расширение
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

			///	"AbC" -> "abc"
			///	\param str: строка для изменения
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