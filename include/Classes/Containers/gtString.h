#pragma once
#ifndef __GT_STRING_H__
#define __GT_STRING_H__

#include "Common/gtTypes.h"

namespace gost{

	const u32 StringWordSize = 16u;

		//String class
	template<typename char_type>
	class gtString_base{

		using pointer = char_type*;
		using const_pointer = const char_type*;
		using reference = char_type&;
		using const_reference = const char_type&;
		using this_type = gtString_base;
		using this_const_reference = const gtString_base&;

			// buffer
		pointer m_data;

			// num of allocated chars
		u32 m_allocated;

		u32 m_size;

		void reallocate( u32 new_allocated ){

			//char_type * new_data = new char_type[ new_allocated ];
			char_type * new_data = (char_type*)gtMemAlloc( new_allocated * sizeof(char_type) );

			if( m_data ){
				memcpy( new_data, m_data, m_size * sizeof( char_type ) );

				//delete []m_data;
				gtMemFree( m_data );

			}else{
				//memset( new_data, 0,  new_allocated * sizeof( char_type ) );
				gtMemSet((char_type)0,new_data,new_allocated);
			}

			m_data = new_data;

			m_allocated = new_allocated;

		}

		template<typename other_type>
		u32 getlen( const other_type* str ){
			unsigned int len = gtConst0U;
			const other_type* p = &str[ gtConst0U ];
			while( *p++ ) len++;
			return len;
		}

		template<typename dst_type, typename src_type>
		void copy( dst_type * dst, src_type* src ){
			while( *src ){
				*dst = static_cast<dst_type>(*src);
				dst++;
				src++;
			}
		}


		public:

			// Default constructor
		gtString_base():
			m_data( nullptr ),
			m_allocated( StringWordSize ),
			m_size( gtConst0U )
		{
			reallocate( m_allocated );
		}

			// Construct from 'c' string
			// \param str: 'c' string
		gtString_base( const_pointer str ):
			m_data( nullptr ),
			m_allocated( StringWordSize ),
			m_size( gtConst0U )
		{
			reallocate( m_allocated );
			assign( str );
		}

			// Construct from other (copy c-tor)
			// \param str: Other gtString
		gtString_base( this_const_reference str ):
			m_data(nullptr),
			m_allocated(StringWordSize),
			m_size(gtConst0U)
		{
			reallocate( m_allocated );
			assign( str );
		}

			// move c-tor
		gtString_base( this_type&& str ):
			m_data( nullptr ),
			m_allocated( StringWordSize ),
			m_size( gtConst0U )
		{
			reallocate( m_allocated );
			assign( str );
		}

		gtString_base( char_type c ):
			m_data( nullptr ),
			m_allocated( StringWordSize ),
			m_size( gtConst0U )
		{
			u32 new_size = gtConst1U;
			reallocate( (new_size + gtConst1U) + StringWordSize );
			m_data[ gtConst0U ] = c;
			m_size = new_size;
			m_data[ m_size ] = 0x0;
		}

			// Destructor
		~gtString_base(){
			if( m_data )
				//delete []m_data;
				gtMemFree( m_data );
		}

			// Allocate memory for chars
			// \param size: size in chars
		void reserve( u32 size ){
			if( size > m_allocated ){
				reallocate( size );
				m_data[ m_size ] = 0;
			}
		}

			// Assign 'c' string
			// \param str: 'c' string
		template<typename other_type>
		void assign( other_type str ){
			m_size = gtConst0U;
			m_data[ m_size ] = 0x0;
			append( str );
		}

			//	Assign other
			// \param str: Other gtString
		void assign( this_const_reference str ){
			m_size = gtConst0U;
			m_data[ m_size ] = 0x0;
			append( str );
		}

			// Add 'c' string to back
			// \param str: 'c' string
		template<typename other_type>
		void append( const other_type * str ){
			u32 new_size = getlen( str ) + m_size;

			if( (new_size + gtConst1U) > m_allocated )
				reallocate( (new_size + gtConst1U) + StringWordSize );

			copy( &m_data[m_size], str );

			m_size = new_size;
			m_data[ m_size ] = 0x0;
		}

			// Add other
			// \param str: Other gtString
		void append( this_const_reference str ){
			append( str.data() );
		}

			// Add char
			// \param c: char
		void append( char_type c ){
			u32 new_size = m_size + gtConst1U;
			if( (new_size + gtConst1U) > m_allocated )
				reallocate( (new_size + gtConst1U) + StringWordSize );
			m_data[ m_size ] = c;
			m_size = new_size;
			m_data[ m_size ] = 0x0;
		}

		void append( s32 c ){
			char buf[32u];
			GT_SPRINTF(buf,"%i",c);
			append( buf );
		}

		void append( u32 c ){
			char buf[32u];
			GT_SPRINTF(buf,"%u",c);
			append( buf );
		}

		void append( f64 c ){
			char buf[32u];
			GT_SPRINTF(buf,"%g",c);
			append( buf );
		}

		void append( f32 c ){
			char buf[32u];
			GT_SPRINTF(buf,"%g",c);
			append( buf );
		}

		gtString_base<char> to_ansiString(){
			gtString_base<char> ret;
			ret.reserve(m_size);
			this->copy(ret.data(),m_data);
			return ret;
		}

		gtString_base<char16_t> to_utf16String(){
			gtString_base<char16_t> ret;
			ret.reserve(m_size);
			this->copy(ret.data(),m_data);
			return ret;
		}

			// Get 'c' string
			// \return Pointer to first char of string
		const_pointer c_str() const {
			return m_data;
		}

			// Get string buffer
			// \return Pointer to first char of string
		pointer data() const {
			return m_data;
		}

			// Get size
			// \return Size of string
		const u32 size() const {
			return m_size;
		}

			//	Assign
		this_type& operator=( this_const_reference str ){
			assign( str );
			return *this;
		}

		this_type& operator=( this_type&& str ){
			assign( str );
			return *this;
		}

		template<typename other_type>
		this_type& operator=( other_type * str ){
			assign( str );
			return *this;
		}

		this_type operator+( const_pointer str ){
			this_type ret( *this );
			ret.append( str );
			return ret;
		}

		this_type operator+( this_const_reference str ){
			return operator+(str.data());
		}

		this_type operator+( u32 num ){
			this_type r( *this );
			r.append( num );
			return r;
		}

		pointer begin() const {
			return m_data;
		}

		pointer end() const {
			return (m_data+(m_size));
		}

		const_reference operator[]( u32 i ) const {
			return m_data[ i ];
		}

		reference operator[]( u32 i ){
			return m_data[ i ];
		}

		void operator+=( s32 i ){
			append( i );
		}

		void operator+=( u32 i ){
			append( i );
		}
		void operator+=( f32 i ){
			append( i );
		}
		void operator+=( f64 i ){
			append( i );
		}

		void operator+=( char_type c ){
			append( c );
		}

		/*void operator+=( pointer str ){
			append( str );
		}*/

		template<typename other_type>
		void operator+=( other_type * str ){
			append( str );
		}

		void operator+=( this_const_reference str ){
			append( str );
		}

		bool operator==( this_const_reference other ) const {
			if( other.size() != m_size ) return false;

			const u32 sz = other.size();
			const auto * ptr = other.data();
			for( u32 i = gtConst0U; i < sz; ++i )
				if( ptr[ i ] != m_data[ i ]  ) return false;

			return true;
		}

		bool operator!=( this_const_reference other ) const {
			if( other.size() == m_size ) return false;

			const u32 sz = other.size();
			const auto * ptr = other.data();
			for( u32 i = gtConst0U; i < sz; ++i )
				if( ptr[ i ] == m_data[ i ]  ) return false;

			return true;
		}

		void clear(){
			m_size = gtConst0U;
			m_data[ m_size ] = static_cast<char_type>(m_size);
		}

		void pop_back(){
			if( m_size ){
				--m_size;
				m_data[ m_size ] = gtConst0U;
			}
		}

		void shrink_to_fit(){
			if( m_size ){
				if( m_allocated > (m_size + StringWordSize) ){
					reallocate( m_size + gtConst1U );
					m_data[ m_size ] = 0x0;
				}
			}
		}

		void setSize( u32 size ){
			m_size = size;
		}

		void erase( u32 begin, u32 end ){

			u32 numCharsToDelete = end - begin + gtConst1U; // example, delete first char: 0 - 0 + 1

			u32 next = end + gtConst1U;

			for( u32 i = begin; i < m_size; ++i ){

				if( next < m_size ){

					m_data[ i ] = m_data[ next ];

					++next;
				}

			}

			m_size -= numCharsToDelete;
			m_data[ m_size ] = 0;
		}

		void pop_front(){
			erase( gtConst0U, gtConst0U );
		}

		bool isSpace( u32 index ){
			if( m_data[ index ] == (char_type)' ' ) return true;
			if( m_data[ index ] == (char_type)'\r' ) return true;
			if( m_data[ index ] == (char_type)'\n' ) return true;
			if( m_data[ index ] == (char_type)'\t' ) return true;
			return false;
		}
	};

	GT_TYPE( gtString, gtString_base<char16_t>); //< See \ref _GT_TYPE

	GT_TYPE( gtStringW, gtString_base<wchar_t>); //< See \ref _GT_TYPE

	GT_TYPE( gtString32, gtString_base<char32_t>); //< See \ref _GT_TYPE

	GT_TYPE( gtStringA, gtString_base<char>); //< See \ref _GT_TYPE

	namespace util{
		
		GT_FORCE_INLINE void stringChangeEndian( gtString_base<char16_t>& string ){
			u32 sz = string.size();
			for( u32 i = gtConst0U; i < sz; ++i ){
				u8* p1 = reinterpret_cast<u8*>( &string.data()[ i ]);
				u8* p2 = p1 + gtConst1U;
				u8 c1 = *p1;
				*p1 = *p2;
				*p2 = c1;
			}
		}

		GT_FORCE_INLINE void stringUTF16_to_UTF8( gtString_base<char16_t>& utf16, gtString_base<char>& utf8 ){
			u32 sz = utf16.size();
			for( u32 i = gtConst0U; i < sz; ++i ){
				char16_t ch16 = utf16[ i ];
				if( ch16 < 0x80 ){
					utf8 += (char)ch16;
				}else if( ch16 < 0x800 ){
					utf8 += (char)((ch16>>6)|0xc0);
					utf8 += (char)((ch16&0x3f)|0x80);
				}
			}
		}

		GT_FORCE_INLINE void stringUTF8_to_UTF16( gtString_base<char16_t>& utf16, gtString_base<char>& utf8 ){
			gtArray<u32> unicode;
			u32 i = gtConst0U;
			auto sz = utf8.size();
			while( i < sz ){
				u32 uni = gtConst0U;
				u32 todo = gtConst0U;
//				bool error = false;
				u8 ch = utf8[i++];
				if( ch <= 0x7F ){
					uni = ch;
					todo = 0;
				}else if( ch <= 0xBF ){
					//throw std::logic_error("not a UTF-8 string");
				}else if ( ch <= 0xDF ){
					uni = ch&0x1F;
					todo = 1;
				}else if( ch <= 0xEF ){
					uni = ch&0x0F;
					todo = 2;
				}else if( ch <= 0xF7 ){
					uni = ch&0x07;
					todo = 3;
				}else{
					//throw std::logic_error("not a UTF-8 string");
				}
				for( u32 j = 0; j < todo; ++j ){
					//if( i == utf8.size() )
						//throw std::logic_error("not a UTF-8 string");
					u8 ch2 = utf8[i++];
					//if( ch < 0x80 || ch > 0xBF )
						//throw std::logic_error("not a UTF-8 string");
					uni <<= 6;
					uni += ch2 & 0x3F;
				}
				//if( uni >= 0xD800 && uni <= 0xDFFF )
					//throw std::logic_error("not a UTF-8 string");
				//if( uni > 0x10FFFF )
					//throw std::logic_error("not a UTF-8 string");
				unicode.push_back(uni);
			}
			auto sz2 = unicode.size();
			for( u32 o = gtConst0U; o < sz2; ++o ){
				u32 uni = unicode[o];
				if( uni <= 0xFFFF ){
					utf16 += (char16_t)uni;
				}else{
					uni -= 0x10000;
					utf16 += (wchar_t)((uni >> 10) + 0xD800);
					utf16 += (wchar_t)((uni & 0x3FF) + 0xDC00);
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
