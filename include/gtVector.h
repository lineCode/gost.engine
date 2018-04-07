/*!	GOST
	\file gtVector.h
	\brief vector

	\attention Для ускоренной разработки используется шаблонный вариант, с указанием типа и размера. Из за неизвестной длинны приходится использовать массив для инициализации и т.д. Возможно он будет переписан.
*/

#pragma once
#ifndef __GT_VECTOR_H__
#define __GT_VECTOR_H__ //< include guard

namespace gost{
		

	template< typename Type > struct gtVector3;
	template< typename Type > struct gtVector4;
	
		//	Вектор
	template< typename Type >
	struct gtVector2{

		Type x, y;

		Type getComponent( u32 index ){
			switch( index ){
			case 0u:
				return x;
			case 1u:
				return y;
			default:
				gtStackTrace::dumpStackTrace();
				break;
			}
		}

			//	ctor по умолчанию
		gtVector2( void ):
			x( static_cast< Type >( 0u ) ),
			y( static_cast< Type >( 0u ) ){
		}

			// Создать из готового вектора
			// \param v: вектор для копирования
		gtVector2( const gtVector2< Type >& v ){
			*this = v;
		}

			// \param l: список значений
		gtVector2( Type X, Type Y ):
			x( static_cast< Type >( X ) ),
			y( static_cast< Type >( Y ) ){
		}

			//	Создать с указанием одной величины (все компоненты будут равнятся этой величине)
			// \param v: значение которым должен инициализироваться вектор
		gtVector2( Type v ):
			x( static_cast< Type >( v ) ),
			y( static_cast< Type >( v ) ){
		}

			//	Установить каждый компонент равным 0
		void zero( void ){
			x = y = static_cast< Type >( 0u );
		}

			//	Заполнить вектор указанным значением
			// \param val: значение которым должен инициализироваться вектор
		void fill( Type val ){
			x = y = static_cast< Type >( val );
		}

			//	установить значение компонента X
			// \param value: новое значение компонента
		void setX( Type value ){
			x = value;
		}

		void setY( Type value ){
			y = value;
		}

			//	Сложение векторов
			// \param v: вектор
		void operator+=( const gtVector2< Type >& v ){
			x += v.x;
			y += v.y;
		}

			//	Вычитание векторов
			// \param v: вектор
		void operator-=( const gtVector2< Type >& v ){
			x -= v.x;
			y -= v.y;
		}

			//	Умножение векторов
			// \param v: вектор
		void operator*=( const gtVector2< Type >& v ){
			x *= v.x;
			y *= v.y;
		}

			//	Умножить компоненты вектора на заданное число
			// \param v: число
		void operator*=( Type v ){
			x *= v;
			y *= v;
		}

			//	Разделить на вектор
			// \param v: вектор
		void operator/=( const gtVector2< Type >& v ){
			x /= v.x;
			y /= v.y;
		}

		Type summ( void ){
			return x + y;
		}

			//	Сложение векторов
			// \param v: вектор
			//	\return сумма двух векторов
		gtVector2< Type > operator+( const gtVector2< Type >& v ) const {
			return gtVector2( x + v.x, y + v.y );
		}

			//	Вычитание векторов
			// \param v: вектор
			//	\return разность двух векторов
		gtVector2< Type > operator-( const gtVector2< Type >& v ) const {
			return gtVector2( x - v.x, y - v.y );
		}

			//	Изменение знака векта
			//	\return Новый вектор с противоположным знаком
		gtVector2< Type > operator-( void ) const {
			return gtVector2( -x, -y );
		}

			//	Умножение векторов
			// \param v: вектор
			//	\return произведение двух векторов
		gtVector2< Type > operator*( const gtVector2< Type >& v ) const {
			return gtVector2( x * v.x, y * v.y );
		}

			//	Отношение векторов
			// \param v: вектор
			//	\return отношение двух векторов
		gtVector2< Type > operator/( const gtVector2< Type >& v ) const {
			return gtVector2( x / v.x, y / v.y );
		}

			//	получить значение компонента
			//	\param id: номер компонента
			// \return значение компонента
		const Type	getX( void ) const {
			return x;
		}

		const Type	getY( void ) const {
			return y;
		}
		
			//	установка компонентов
			//	\param l: список компонентов
		void	set( Type X, Type Y ){
			x = X;
			y = Y;
		}

			//	сравнит вектор
			//	\param v: вектор для сравнение
			// \return \b true если вектора равны
		bool	operator==( const gtVector2< Type >& v ) const {
			if( x != v.x ) return false;
			if( y != v.y ) return false;
			return true;
		}

			//	сравнит вектор
			//	\param v: вектор для сравнение
			// \return \b true если вектора \b не равны
		bool	operator!=( const gtVector2< Type >& v ) const {
			if( x != v.x ) return true;
			if( y != v.y ) return true;
			return false;
		}

		

			//	получить длину вектора
			// \return длина вектора (от центра координат)
		Type	lengthSqrt( void ){
			return ( x * x ) + ( y * y );
		}

			//	нормализовать вектор - нормализация означает приведение значений в формат от 0 до 1, или от -1 до 1
		void	normalize( void ){
			f32 sqLen, invLen;
			sqLen = lengthSqrt();
			invLen = math::invSqrt( sqLen );
			x *= invLen;
			y *= invLen;
		}


		gtVector3< Type > getV3( void ){
			return gtVector3< Type >( x, y, static_cast< Type >( 0u ) );
		}

		gtVector4< Type > getV4( void ){
			return gtVector4< Type >( x, y, static_cast< Type >( 0u ), static_cast< Type >( 0u ) );
		}

	};

	template< typename Type >
	struct gtVector3{

		Type x, y, z;

		Type getComponent( u32 index ){
			switch( index ){
			case 0u:
				return x;
			case 1u:
				return y;
			case 2u:
				return z;
			default:
				gtStackTrace::dumpStackTrace();
				break;
			}
		}

		gtVector3( void ):
			x( static_cast< Type >( 0u ) ),
			y( static_cast< Type >( 0u ) ),
			z( static_cast< Type >( 0u ) ){
		}

		gtVector3( const gtVector3< Type >& v ){
			*this = v;
		}

		gtVector3( const gtVector2< Type >& v ):
			x( v.x ),
			y( v.y ),
			z( static_cast< Type >( 0u ) ){ 
		}

		gtVector3( Type X, Type Y, Type Z ):
			x( X ),
			y( Y ),
			z( Z ){
		}

		gtVector3( Type v ):
			x( v ),
			y( v ),
			z( v ){
		}

		void zero( void ){
			x = y = z = static_cast< Type >( 0u ); 
		}

		void fill( Type val ){
			x = y = z = static_cast< Type >( val );
		}

		void setZ( Type value ){ 
			z = value; 
		}

		void operator+=( const gtVector3< Type >& v ){ 
			x += v.x; 
			y += v.y; 
			z += v.z; 
		}

		void operator-=( const gtVector3< Type >& v ){ 
			x -= v.x; 
			y -= v.y; 
			z -= v.z; 
		}

		void operator*=( const gtVector3< Type >& v ){ 
			x *= v.x; 
			y *= v.y; 
			z *= v.z; 
		}

		void operator*=( Type v ){ 
			x *= v; 
			y *= v; 
			z *= v; 
		}

		void operator/=( const gtVector3< Type >& v ){ 
			x /= v.x; 
			y /= v.y; 
			z /= v.z;
		}

		Type summ( void ){ 
			return x + y + z; 
		}

		gtVector3< Type > operator+( const gtVector3< Type >& v ) const { 
			return gtVector3( x + v.x, y + v.y, z + v.z );
		}

		gtVector3< Type > operator-( const gtVector3< Type >& v ) const { 
			return gtVector3( x - v.x, y - v.y, z - v.z ); 
		}

		gtVector3< Type > operator-( void ) const { 
			return gtVector3( -x, -y, -z ); 
		}

		gtVector3< Type > operator*( const gtVector3< Type >& v ) const { 
			return gtVector3( x * v.x, y * v.y, z * v.z ); 
		}

		gtVector3< Type > operator/( const gtVector3< Type >& v ) const { 
			return gtVector3( x / v.x, y / v.y, z / v.z ); 
		}

		const Type	getZ( void ) const { 
			return z; 
		}

		void	set( Type X, Type Y, Type Z ){ 
			x = X; 
			y = Y; 
			z = Z; 
		}

		bool	operator==( const gtVector3< Type >& v ) const { 
			if( x != v.x ) 
				return false; 
			if( y != v.y ) 
				return false; 
			if( z != v.z ) 
				return false; 
			return true; 
		}

		bool	operator!=( const gtVector3< Type >& v ) const { 
			if( x != v.x ) 
				return true; 
			if( y != v.y ) 
				return true; 
			if( z != v.z ) 
				return true; 
			return false; 
		}

		Type	lengthSqrt( void ){ 
			return ( x * x ) + ( y * y ) + ( z * z );
		}

		Type	length( void ){
			return std::sqrtf( lengthSqrt() );
		}

		Type	distance( const gtVector3< Type >& from ){
			return gtVector3< Type >( x - from.x, y - from.y, z - from.z ).length();
		}

		void	normalize( void ){
			f32 sqLen, invLen;
			sqLen = lengthSqrt();
			invLen = math::invSqrt( sqLen );
			x *= invLen;
			y *= invLen;
			z *= invLen;
		}

		gtVector2< Type > getV2( void ){ 
			return gtVector2< Type >( x, y );
		}

		gtVector4< Type > getV4( void ){ 
			return gtVector4< Type >( x, y, z, static_cast< Type >( 0u ) );
		}

		Type& operator[]( u32 i ){
			switch( i ){
			case 0u:
				return x;
			case 1u:
				return y;
			case 2u:
				return z;
			default:
				gtStackTrace::dumpStackTrace();
			}
		}

	};

	template< typename Type >
	struct gtVector4{

		Type x, y, z, w;

		Type getComponent( u32 index ){
			switch( index ){
			case 0u:
				return x;
			case 1u:
				return y;
			case 2u:
				return z;
			case 3u:
				return w;
			default:
				gtStackTrace::dumpStackTrace();
				break;
			}
		}

		gtVector4( void ):
			x( static_cast< Type >( 0u ) ),
			y( static_cast< Type >( 0u ) ),
			z( static_cast< Type >( 0u ) ),
			w( static_cast< Type >( 0u ) ){
		}

		gtVector4( const gtVector4< Type >& v ){
			*this = v; 
		}

		gtVector4( const gtVector3< Type >& v ):
			x( v.x ), 
			y( v.y ),
			z( v.z ),
			w( static_cast< Type >( 0u ) ){
		}

		gtVector4( Type X, Type Y, Type Z, Type W ):
			x( X ),
			y( Y ),
			z( Z ),
			w( W ){
		}

		gtVector4( Type v ):
			x( v ),
			y( v ),
			z( v ),
			w( v ){
		}

		void zero( void ){ 
			x = y = z = w = static_cast< Type >( 0u ); 
		}

		void fill( Type val ){ 
			x = y = z = w = static_cast< Type >( val ); 
		}

		void setW( Type value ){
			w = value; 
		}

		void operator+=( const gtVector4< Type >& v ){ 
			x += v.x;
			y += v.y; 
			z += v.z; 
			w += v.w; 
		}

		void operator-=( const gtVector4< Type >& v ){ 
			x -= v.x;
			y -= v.y; 
			z -= v.z; 
			w -= v.w; 
		}

		void operator*=( const gtVector4< Type >& v ){ 
			x *= v.x; 
			y *= v.y; 
			z *= v.z; 
			w *= v.w; 
		}

		void operator*=( Type v ){ 
			x *= v; 
			y *= v; 
			z *= v; 
			w *= v; 
		}

		void operator/=( const gtVector4< Type >& v ){
			x /= v.x;
			y /= v.y; 
			z /= v.z; 
			w /= v.w; 
		}

		Type summ( void ){ 
			return x + y + z + w; 
		}

		gtVector4< Type > operator+( const gtVector4< Type >& v ) const { 
			return gtVector4( x + v.x, y + v.y, z + v.z, w + v.w ); 
		}

		gtVector4< Type > operator-( const gtVector4< Type >& v ) const { 
			return gtVector4( x - v.x, y - v.y, z - v.z, w - v.w );
		}

		gtVector4< Type > operator-( void ) const { 
			return gtVector4( -x, -y, -z, -w ); 
		}

		gtVector4< Type > operator*( const gtVector4< Type >& v ) const { 
			return gtVector4( x * v.x, y * v.y, z * v.z, w * v.w ); 
		}

		gtVector4< Type > operator/( const gtVector4< Type >& v ) const { 
			return gtVector4( x / v.x, y / v.y, z / v.z, w / v.w );
		}

		const Type	getW( void ) const { 
			return w; 
		}

		void	set( Type X, Type Y, Type Z, Type W ){ 
			x = X; 
			y = Y; 
			z = Z; 
			w = W; 
		}

		void	set( Type val ){ 
			x = y = z = w = val;
		}

		void	set( const gtVector3< Type >& v ){ 
			x = v.x;
			y = v.y;
			z = v.z;
			w = static_cast< Type >( 0u );
		}

		void	setXYZ( Type val ){ 
			x = y = z = val;
		}

		bool	operator==( const gtVector4< Type >& v ) const {
			if( x != v.x ) 
				return false;
			if( y != v.y ) 
				return false;
			if( z != v.z ) 
				return false;
			if( w != v.w ) 
				return false;
			return true;
		}

		bool	operator!=( const gtVector4< Type >& v ) const {
			if( x != v.x ) 
				return true;
			if( y != v.y ) 
				return true;
			if( z != v.z ) 
				return true;
			if( w != v.w ) 
				return true;
			return false;
		}

		Type	lengthSqrt( void ){
			return ( x * x ) + ( y * y ) + ( z * z ) + ( w * w );
		}

		void	normalize( void ){
			f32 sqLen, invLen;
			sqLen = lengthSqrt();
			invLen = math::invSqrt( sqLen );
			x *= invLen;
			y *= invLen;
			z *= invLen;
			w *= invLen;
		}

		gtVector2< Type > getV2( void ){ return gtVector2< Type >( x, y ); }
		gtVector3< Type > getV3( void ){ return gtVector3< Type >( x, y, z ); }
	};

	template< typename Type >
	struct gtVector5{

		Type x, y, z, w, a;

		Type getComponent( u32 index ){
			switch( index ){
			case 0u:
				return x;
			case 1u:
				return y;
			case 2u:
				return z;
			case 3u:
				return w;
			case 4u:
				return a;
			default:
				gtStackTrace::dumpStackTrace();
				break;
			}
		}

		gtVector5( void ):
			x( static_cast< Type >( 0u ) ),
			y( static_cast< Type >( 0u ) ),
			z( static_cast< Type >( 0u ) ),
			w( static_cast< Type >( 0u ) ),
			a( static_cast< Type >( 0u ) ){
		}

		gtVector5( Type X, Type Y, Type Z, Type W, Type A ):
			x( X ),
			y( Y ),
			z( Z ),
			w( W ),
			a( A ){
		}

	};

	template< typename Type >
	struct gtVector6{

		Type x, y, z, w, a, b;

		Type getComponent( u32 index ){
			switch( index ){
			case 0u:
				return x;
			case 1u:
				return y;
			case 2u:
				return z;
			case 3u:
				return w;
			case 4u:
				return a;
			case 5u:
				return b;
			default:
				gtStackTrace::dumpStackTrace();
				break;
			}
		}

		gtVector6( void ):
			x( static_cast< Type >( 0u ) ),
			y( static_cast< Type >( 0u ) ),
			z( static_cast< Type >( 0u ) ),
			w( static_cast< Type >( 0u ) ),
			a( static_cast< Type >( 0u ) ),
			b( static_cast< Type >( 0u ) ){
		}

		gtVector6( Type X, Type Y, Type Z, Type W, Type A, Type B ):
			x( X ),
			y( Y ),
			z( Z ),
			w( W ),
			a( A ),
			b( B ){
		}

	};

	template< typename Type >
	struct gtVector7{

		Type x, y, z, w, a, b, c;

		Type getComponent( u32 index ){
			switch( index ){
			case 0u:
				return x;
			case 1u:
				return y;
			case 2u:
				return z;
			case 3u:
				return w;
			case 4u:
				return a;
			case 5u:
				return b;
			case 6u:
				return c;
			default:
				gtStackTrace::dumpStackTrace();
				break;
			}
		}

		gtVector7( void ):
			x( static_cast< Type >( 0u ) ),
			y( static_cast< Type >( 0u ) ),
			z( static_cast< Type >( 0u ) ),
			w( static_cast< Type >( 0u ) ),
			a( static_cast< Type >( 0u ) ),
			b( static_cast< Type >( 0u ) ),
			c( static_cast< Type >( 0u ) ){
		}

		gtVector7( Type X, Type Y, Type Z, Type W, Type A, Type B, Type C ):
			x( X ),
			y( Y ),
			z( Z ),
			w( W ),
			a( A ),
			b( B ),
			c( C ){
		}

	};

	template< typename Type >
	struct gtVector8{

		Type x, y, z, w, a, b, c, d;

		Type getComponent( u32 index ){
			switch( index ){
			case 0u:
				return x;
			case 1u:
				return y;
			case 2u:
				return z;
			case 3u:
				return w;
			case 4u:
				return a;
			case 5u:
				return b;
			case 6u:
				return c;
			case 7u:
				return d;
			default:
				gtStackTrace::dumpStackTrace();
				break;
			}
		}

		gtVector8( void ):
			x( static_cast< Type >( 0u ) ),
			y( static_cast< Type >( 0u ) ),
			z( static_cast< Type >( 0u ) ),
			w( static_cast< Type >( 0u ) ),
			a( static_cast< Type >( 0u ) ),
			b( static_cast< Type >( 0u ) ),
			c( static_cast< Type >( 0u ) ),
			d( static_cast< Type >( 0u ) ){
		}

		gtVector8( Type X, Type Y, Type Z, Type W, Type A, Type B, Type C, Type D ){
			x = X;
			y = Y;
			z = Z;
			w = W;
			a = A;
			b = B;
			c = C;
			d = D;
		}

	};

	using v2f = gtVector2<f32>;	//< синоним для gtVector3<f32>
	using v2i = gtVector2<s32>;	//< синоним для gtVector3<s32>
	using v2u = gtVector2<u32>;	//< синоним для gtVector3<u32>

	using v3f = gtVector3<f32>;	//< синоним для gtVector3<f32>
	using v3i = gtVector3<s32>;	//< синоним для gtVector3<s32>
	using v3u = gtVector3<u32>;	//< синоним для gtVector3<u32>

	using v4f = gtVector4<f32>;	//< синоним для gtVector4<f32>
	using v4i = gtVector4<s32>;	//< синоним для gtVector4<s32>
	using v4u = gtVector4<u32>;	//< синоним для gtVector4<u32>

	using v5f = gtVector5<f32>;	//< синоним для gtVector5<f32>
	using v5i = gtVector5<s32>;	//< синоним для gtVector5<s32>
	using v5u = gtVector5<u32>;	//< синоним для gtVector5<u32>

	using v6f = gtVector6<f32>;	//< синоним для gtVector6<f32>
	using v6i = gtVector6<s32>;	//< синоним для gtVector6<s32>
	using v6u = gtVector6<u32>;	//< синоним для gtVector6<u32>

	using v7f = gtVector7<f32>;	//< синоним для gtVector7<f32>
	using v7i = gtVector7<s32>;	//< синоним для gtVector7<s32>
	using v7u = gtVector7<u32>;	//< синоним для gtVector7<u32>

	using v8f = gtVector8<f32>;	//< синоним для gtVector8<f32>
	using v8i = gtVector8<s32>;	//< синоним для gtVector8<s32>
	using v8u = gtVector8<u32>;	//< синоним для gtVector8<u32>

	namespace math{

			//	cross product of s with a /векторное произедение
			//	\param s: first vector
			//	\param a: second vector
			//	\return cross product
		GT_FORCE_INLINE v3f cross( const v3f& s, const v3f& a ){
			v3f r;
			r.x = (s.y * a.z) - (s.z * a.y);
			r.y = (s.z * a.x) - (s.x * a.z);
			r.z = (s.x * a.y) - (s.y * a.x);
			return r;
		}

			//	dot product of s with a /скалярное произедение
			//	\param s: first vector
			//	\param a: second vector
			//	\return dot product
		GT_FORCE_INLINE f32 dot( const v3f& s, const v3f& a ){
			f32 r;
			r =  (s.x * a.x);
			r += (s.y * a.y);
			r += (s.z * a.z);
			return r;
		}

			//	dot product of s with a /скалярное произедение
			//	\param s: first vector
			//	\param a: second vector
			//	\return dot product
		GT_FORCE_INLINE f32 dot( const v4f& s, const v3f& a ){
			f32 r;
			r =  (s.x * a.x);
			r += (s.y * a.y);
			r += (s.z * a.z);
			return r;
		}

			//	dot product of s with a /скалярное произедение
			//	\param s: first vector
			//	\param a: second vector
			//	\return dot product
		GT_FORCE_INLINE f32 dot( const v4f& s, const v4f& a ){
			f32 r;
			r =  (s.x * a.x);
			r += (s.y * a.y);
			r += (s.z * a.z);
			return r;
		}

		GT_FORCE_INLINE v3f planeDotCoord( const v4f& plane, const v3f& coord ){
			return v3f( plane.x * coord.x + plane.y * coord.y + plane.z * coord.z + plane.w );
		}
	}

	namespace util{

			//	print vector in output window
		GT_FORCE_INLINE void printVector( const v2f& v ){
			gtLogWriter::printInfo( u"Vector:" );
			gtLogWriter::printInfo( u"\t\t%f %f", v.x, v.y );
		}

			//	print vector in output window
		GT_FORCE_INLINE void printVector( const v3f& v ){
			gtLogWriter::printInfo( u"Vector:" );
			gtLogWriter::printInfo( u"\t\t%f %f %f", v.x, v.y, v.z );
		}

	}




}

#endif

/*
Copyright (c) 2017, 2018 532235

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