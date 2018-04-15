/*!	GOST
	\file gtString.h
	\brief string
*/

#pragma once
#ifndef __GT_STRING_H__
#define __GT_STRING_H__


namespace gost{

		// чтобы постоянно не выделять память при добавлении символов
	const u32 StringWordSize = 16u;

		//String class
	template<typename char_type>
	class gtString_base{

		typedef char_type*			pointer;
		typedef const char_type*	const_pointer;
		typedef char_type&			reference;
		typedef const char_type&	const_reference;
		typedef gtString_base		this_type;
		typedef const gtString_base& this_const_reference;

			// buffer
		pointer m_data;

			// num of allocated chars
		u32 m_allocated;

		u32 m_size;

		void reallocate( u32 new_allocated ){

			char_type * new_data = new char_type[ new_allocated ];

			if( m_data ){
				memcpy( new_data, m_data, m_size * sizeof( char_type ) );
				delete []m_data;
			}else{
				memset( new_data, 0,  new_allocated * sizeof( char_type ) );
			}

			m_data = new_data;

			m_allocated = new_allocated;

		}

		template<typename other_type>
		u32 getlen( const other_type* str ){
			unsigned int len = 0u;
			const other_type* p = &str[ 0u ];
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
		gtString_base( void ):
			m_data( nullptr ),
			m_allocated( StringWordSize ),
			m_size( 0u )
		{
			reallocate( m_allocated );
		}

			// Construct from 'c' string
			// \param str: 'c' string
		gtString_base( const_pointer str ):
			m_size( 0u ),
			m_allocated( StringWordSize ),
			m_data( nullptr )
		{
			reallocate( m_allocated );
			assign( str );
		}

			// Construct from other (copy c-tor)
			// \param str: Other gtString
		gtString_base( this_const_reference str ):
			m_allocated(StringWordSize),
			m_data(nullptr),
			m_size(0u)
		{
			reallocate( m_allocated );
			assign( str );
		}

			// move c-tor
		gtString_base( this_type&& str ):
			m_size( 0u ),
			m_allocated( StringWordSize ),
			m_data( nullptr )
		{
			reallocate( m_allocated );
			assign( str );
		}

		gtString_base( char_type c ):
			m_size( 0u ),
			m_allocated( StringWordSize ),
			m_data( nullptr )
		{
			u32 new_size = 1u;
			reallocate( (new_size + 1u) + StringWordSize );
			m_data[ 0u ] = c;
			m_size = new_size;
			m_data[ m_size ] = 0x0;
		}

			// Destructor
		~gtString_base( void ){
			if( m_data )
				delete []m_data;
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
			m_size = 0u;
			m_data[ m_size ] = 0x0;
			append( str );
		}

			//	Assign other
			// \param str: Other gtString
		void assign( this_const_reference str ){
			m_size = 0u;
			m_data[ m_size ] = 0x0;
			append( str );
		}

			// Add 'c' string to back
			// \param str: 'c' string
		template<typename other_type>
		void append( const other_type * str ){
			u32 new_size = getlen( str ) + m_size;

			if( (new_size + 1u) > m_allocated )
				reallocate( (new_size + 1u) + StringWordSize );

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
			u32 new_size = m_size + 1u;
			if( (new_size + 1u) > m_allocated )
				reallocate( (new_size + 1u) + StringWordSize );
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

		gtString_base<char> to_ansiString( void ){
			gtString_base<char> ret;
			ret.reserve(m_size);
			this->copy(ret.data(),m_data);
			return ret;
		}

		gtString_base<char16_t> to_utf16String( void ){
			gtString_base<char16_t> ret;
			ret.reserve(m_size);
			this->copy(ret.data(),m_data);
			return ret;
		}

			// Get 'c' string
			// \return Pointer to first char of string
		const_pointer c_str( void ) const {
			return m_data;
		}

			// Get string buffer
			// \return Pointer to first char of string
		pointer data( void ) const {
			return m_data;
		}

			// Get size
			// \return Size of string
		const u32 size( void ) const {
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
			for( u32 i = 0u; i < sz; ++i )
				if( ptr[ i ] != m_data[ i ]  ) return false;

			return true;
		}

		bool operator!=( this_const_reference other ) const {
			if( other.size() == m_size ) return false;

			const u32 sz = other.size();
			const auto * ptr = other.data();
			for( u32 i = 0u; i < sz; ++i )
				if( ptr[ i ] == m_data[ i ]  ) return false;

			return true;
		}

		void clear( void ){
			m_size = 0u;
			m_data[ m_size ] = static_cast<char_type>(m_size);
		}

		void pop_back( void ){
			if( m_size ){
				--m_size;
				m_data[ m_size ] = 0u;
			}
		}

		void shrink_to_fit( void ){
			if( m_size ){
				if( m_allocated > (m_size + StringWordSize) ){
					reallocate( m_size + 1u );
					m_data[ m_size ] = 0x0;
				}
			}
		}

		void setSize( u32 size ){
			m_size = size;
		}

		void erase( u32 begin, u32 end ){

			u32 numCharsToDelete = end - begin + 1u; // example, delete first char: 0 - 0 + 1

			u32 next = end + 1u;

			for( u32 i = begin; i < m_size; ++i ){

				if( next < m_size ){

					m_data[ i ] = m_data[ next ];

					++next;
				}

			}

			m_size -= numCharsToDelete;
			m_data[ m_size ] = 0;
		}

		void pop_front( void ){
			erase( 0u, 0u );
		}

		bool isSpace( u32 index ){
			return (bool)std::isspace( (int)m_data[ index ] );
		}
	};

	//! \brief Строка с utf-16. gtString(u"asdasd")
	GT_TYPE( gtString, gtString_base<char16_t>); //< See \ref _GT_TYPE

	//! \brief Строка с wide char. gtString(L"asdasd")
	GT_TYPE( gtStringW, gtString_base<wchar_t>); //< See \ref _GT_TYPE

	//! \brief Строка с utf-32. gtString(U"asdasd")
	GT_TYPE( gtString32, gtString_base<char32_t>); //< See \ref _GT_TYPE

	//! \brief Строка с ANSI текстом
	GT_TYPE( gtStringA, gtString_base<char>); //< See \ref _GT_TYPE

	namespace util{

		GT_FORCE_INLINE void changeEndian( gtString_base<char16_t>& string ){
			u32 sz = string.size();
			for( u32 i = 0u; i < sz; ++i ){
				u8* p1 = reinterpret_cast<u8*>( &string.data()[ i ]);
				u8* p2 = p1 + 1u;
				u8 c1 = *p1;
				*p1 = *p2;
				*p2 = c1;
			}
		}

		GT_FORCE_INLINE void utf16_to_utf8( gtString_base<char16_t>& utf16, gtString_base<char>& utf8 ){
			u32 sz = utf16.size();
			for( u32 i = 0u; i < sz; ++i ){
				char16_t ch16 = utf16[ i ];
				if( ch16 < 0x80 ){
					utf8 += (char)ch16;
				}else if( ch16 < 0x800 ){
					utf8 += (char)((ch16>>6)|0xc0);
					utf8 += (char)((ch16&0x3f)|0x80);
				}
			}
		}

		GT_FORCE_INLINE void utf8_to_utf16( gtString_base<char16_t>& utf16, gtString_base<char>& utf8 ){
			gtArray<u32> unicode;
			u32 i = 0u;
			auto sz = utf8.size();
			while( i < sz ){
				u32 uni = 0u;
				u32 todo = 0u;
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
			for( u32 o = 0u; o < sz2; ++o ){
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
