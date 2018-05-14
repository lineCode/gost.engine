﻿#pragma once
#ifndef __GT_VECTOR_H__
#define __GT_VECTOR_H__

namespace gost{


	template< typename Type > struct gtVector3;
	template< typename Type > struct gtVector4;

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
				break;
			}
		}

		gtVector2():
			x( static_cast< Type >( 0u ) ),
			y( static_cast< Type >( 0u ) ){
		}

		gtVector2( const gtVector2< Type >& v ){
			*this = v;
		}

		gtVector2( Type X, Type Y ):
			x( static_cast< Type >( X ) ),
			y( static_cast< Type >( Y ) ){
		}

		gtVector2( Type v ):
			x( static_cast< Type >( v ) ),
			y( static_cast< Type >( v ) ){
		}

		void zero(){
			x = y = static_cast< Type >( 0u );
		}

		void fill( Type val ){
			x = y = static_cast< Type >( val );
		}

		void setX( Type value ){
			x = value;
		}

		void setY( Type value ){
			y = value;
		}

		void operator+=( const gtVector2< Type >& v ){
			x += v.x;
			y += v.y;
		}

		void operator-=( const gtVector2< Type >& v ){
			x -= v.x;
			y -= v.y;
		}

		void operator*=( const gtVector2< Type >& v ){
			x *= v.x;
			y *= v.y;
		}

		void operator*=( Type v ){
			x *= v;
			y *= v;
		}

		void operator/=( const gtVector2< Type >& v ){
			x /= v.x;
			y /= v.y;
		}

		Type summ(){
			return x + y;
		}

		gtVector2< Type > operator+( const gtVector2< Type >& v ) const {
			return gtVector2( x + v.x, y + v.y );
		}

		gtVector2< Type > operator-( const gtVector2< Type >& v ) const {
			return gtVector2( x - v.x, y - v.y );
		}

		gtVector2< Type > operator-() const {
			return gtVector2( -x, -y );
		}

		gtVector2< Type > operator*( const gtVector2< Type >& v ) const {
			return gtVector2( x * v.x, y * v.y );
		}

		gtVector2< Type > operator/( const gtVector2< Type >& v ) const {
			return gtVector2( x / v.x, y / v.y );
		}

		const Type	getX() const {
			return x;
		}

		const Type	getY() const {
			return y;
		}

		void	set( Type X, Type Y ){
			x = X;
			y = Y;
		}

		bool	operator==( const gtVector2< Type >& v ) const {
			if( x != v.x ) return false;
			if( y != v.y ) return false;
			return true;
		}

		bool	operator!=( const gtVector2< Type >& v ) const {
			if( x != v.x ) return true;
			if( y != v.y ) return true;
			return false;
		}



		Type	lengthSqrt(){
			return ( x * x ) + ( y * y );
		}

		void	normalize(){
			f32 sqLen, invLen;
			sqLen = lengthSqrt();
			invLen = math::invSqrt( sqLen );
			x *= invLen;
			y *= invLen;
		}


		gtVector3< Type > getV3(){
			return gtVector3< Type >( x, y, static_cast< Type >( 0u ) );
		}

		gtVector4< Type > getV4(){
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
				break;
			}
		}

		gtVector3():
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

		void zero(){
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

		Type summ(){
			return x + y + z;
		}

		gtVector3< Type > operator+( const gtVector3< Type >& v ) const {
			return gtVector3( x + v.x, y + v.y, z + v.z );
		}

		gtVector3< Type > operator-( const gtVector3< Type >& v ) const {
			return gtVector3( x - v.x, y - v.y, z - v.z );
		}

		gtVector3< Type > operator-() const {
			return gtVector3( -x, -y, -z );
		}

		gtVector3< Type > operator*( const gtVector3< Type >& v ) const {
			return gtVector3( x * v.x, y * v.y, z * v.z );
		}

		gtVector3< Type > operator/( const gtVector3< Type >& v ) const {
			return gtVector3( x / v.x, y / v.y, z / v.z );
		}

		const Type	getZ() const {
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

		Type	lengthSqrt(){
			return ( x * x ) + ( y * y ) + ( z * z );
		}

		Type	length(){
			return static_cast<Type>( std::sqrt( lengthSqrt() ) );
		}

		Type	distance( const gtVector3< Type >& from ){
			return gtVector3< Type >( x - from.x, y - from.y, z - from.z ).length();
		}

		void	normalize(){
			f32 sqLen, invLen;
			sqLen = lengthSqrt();
			invLen = math::invSqrt( sqLen );
			x *= invLen;
			y *= invLen;
			z *= invLen;
		}

		gtVector2< Type > getV2(){
			return gtVector2< Type >( x, y );
		}

		gtVector4< Type > getV4(){
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
			}
			GT_BREAKPOINT(0);
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
			}
			return 0;
		}

		gtVector4():
			x( static_cast< Type >( 0u ) ),
			y( static_cast< Type >( 0u ) ),
			z( static_cast< Type >( 0u ) ),
			w( static_cast< Type >( 0u ) ){
		}

		gtVector4( const gtVector4< Type >& v ){
			*this = v;
		}

		template<typename other_type>
		gtVector4( const gtVector4< other_type >& v ){
			this->x = static_cast<Type>(v.x);
			this->y = static_cast<Type>(v.y);
			this->z = static_cast<Type>(v.z);
			this->w = static_cast<Type>(v.w);
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

		void zero(){
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

		Type summ(){
			return x + y + z + w;
		}

		gtVector4< Type > operator+( const gtVector4< Type >& v ) const {
			return gtVector4( x + v.x, y + v.y, z + v.z, w + v.w );
		}

		gtVector4< Type > operator-( const gtVector4< Type >& v ) const {
			return gtVector4( x - v.x, y - v.y, z - v.z, w - v.w );
		}

		gtVector4< Type > operator-() const {
			return gtVector4( -x, -y, -z, -w );
		}

		gtVector4< Type > operator*( const gtVector4< Type >& v ) const {
			return gtVector4( x * v.x, y * v.y, z * v.z, w * v.w );
		}

		gtVector4< Type > operator/( const gtVector4< Type >& v ) const {
			return gtVector4( x / v.x, y / v.y, z / v.z, w / v.w );
		}

		const Type	getW() const {
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

		Type	lengthSqrt(){
			return ( x * x ) + ( y * y ) + ( z * z ) + ( w * w );
		}

		void	normalize(){
			f32 sqLen, invLen;
			sqLen = lengthSqrt();
			invLen = math::invSqrt( sqLen );
			x *= invLen;
			y *= invLen;
			z *= invLen;
			w *= invLen;
		}

		Type getWidth(){
			return z - x;
		}

		Type	getHeight(){
			return w - y;
		}

		gtVector2< Type > getV2(){ return gtVector2< Type >( x, y ); }
		gtVector3< Type > getV3(){ return gtVector3< Type >( x, y, z ); }
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
			}
			return 0;
		}

		gtVector5():
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
			}
			return 0;
		}

		gtVector6():
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
			}
			return 0;
		}

		gtVector7():
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
			}
			return 0;
		}

		gtVector8():
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

	using v2f = gtVector2<f32>;
	using v2i = gtVector2<s32>;
	using v2u = gtVector2<u32>;

	using v3f = gtVector3<f32>;
	using v3i = gtVector3<s32>;
	using v3u = gtVector3<u32>;

	using v4f = gtVector4<f32>;
	using v4i = gtVector4<s32>;
	using v4u = gtVector4<u32>;

	using v5f = gtVector5<f32>;
	using v5i = gtVector5<s32>;
	using v5u = gtVector5<u32>;

	using v6f = gtVector6<f32>;
	using v6i = gtVector6<s32>;
	using v6u = gtVector6<u32>;

	using v7f = gtVector7<f32>;
	using v7i = gtVector7<s32>;
	using v7u = gtVector7<u32>;

	using v8f = gtVector8<f32>;
	using v8i = gtVector8<s32>;
	using v8u = gtVector8<u32>;

	namespace math{

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
