#pragma once
#ifndef __GT_QUATERNION_H__
#define __GT_QUATERNION_H__

namespace gost{

		//	quaternion
	class gtQuaternion{
	public:

			//	component
		f32 x, y, z, w;

			//	c-tor
		gtQuaternion(){
			identity();
		}

			//	c-tor with other quaternion
			//	\param o: other quaternion
		gtQuaternion( const gtQuaternion& o ):
			x(o.x),
			y(o.y),
			z(o.z),
			w(o.w)
		{}

			//	c-tor with components
			//	\param i1: imagine1, first component
			//	\param i2: imagine2, second component
			//	\param i3: imagine3, third component
			//	\param real: real part, fourth component
		gtQuaternion( f32 i1, f32 i2, f32 i3, f32 real ):
			x(i1),
			y(i2),
			z(i3),
			w(real)
		{}

			//	create quaternion from Euler angles
			// \param Euler_x: x angle
			// \param Euler_y: y angle
			// \param Euler_z: z angle
		gtQuaternion( f32 Euler_x, f32 Euler_y, f32 Euler_z ){
			set( Euler_x, Euler_y, Euler_z );
		}

			//	create quaternion from Euler angles
			// \param v: vector with angles
		gtQuaternion( const v3f& v ){
			set( v );
		}

			//	get quaternion in gtVector4f form
			//	\return vector
		v4f	get() const {
			return v4f( x, y, z, w );
		}

			//	get real part
			//	\return real part
		f32	getReal() const {
			return w;
		}

			//	set quaternion with components
			//	\param i1: imagine1, first component
			//	\param i2: imagine2, second component
			//	\param i3: imagine3, third component
			//	\param real: real part, fourth component
		void set( f32 i1, f32 i2, f32 i3, f32 real ){
			x = i1;
			y = i2;
			z = i3;
			w = real;
		}

			//	set quaternion from Euler angles
			// \param v: vector with angles
		void set( const v3f& v ){
			set( v.x, v.y, v.z );
		}

			//	set quaternion from Euler angles
			// \param Euler_x: x angle
			// \param Euler_y: y angle
			// \param Euler_z: z angle
		void set( f32 Euler_x, f32 Euler_y, f32 Euler_z ){
			f32 c1 = cos( Euler_x * gtConst05F );
			f32 c2 = cos( Euler_y * gtConst05F );
			f32 c3 = cos( Euler_z * gtConst05F );
			f32 s1 = sin( Euler_x * gtConst05F );
			f32 s2 = sin( Euler_y * gtConst05F );
			f32 s3 = sin( Euler_z * gtConst05F );
			w	=	(c1 * c2 * c3) + (s1 * s2 * s3);
			x	=	(s1 * c2 * c3) - (c1 * s2 * s3);
			y	=	(c1 * s2 * c3) + (s1 * c2 * s3);
			z	=	(c1 * c2 * s3) - (s1 * s2 * c3);
		}

			//	set real part
			//	\param r: real part
		void setReal( f32 r ){
			w = r;
		}

			//	reset quaternion
		void identity(){
			x = gtConst0F;
			y = gtConst0F;
			z = gtConst0F;
			w = gtConst1F;
		}

			//	assing other
			//	\return this quaternion
		gtQuaternion& operator=( const gtQuaternion& o ){
			x = o.x;
			y = o.y;
			z = o.z;
			w = o.w;
			return *this;
		}

			//	multiplication with other
			//	\param q: other quaternion
			//	\return new quaternion
		gtQuaternion operator*( const gtQuaternion& q )const{
			return gtQuaternion(
				w * q.x + x * q.w + y * q.z - z * q.y,
				w * q.y + y * q.w + z * q.x - x * q.z,
				w * q.z + z * q.w + x * q.y - y * q.x,
				w * q.w - x * q.x - y * q.y - z * q.z);
		}

			//	multiplication with other
			//	\param q: other quaternion
		void operator*=( const gtQuaternion& q ){
			x = w * q.x + x * q.w + y * q.z - z * q.y;
			y = w * q.y + y * q.w + z * q.x - x * q.z;
			z = w * q.z + z * q.w + x * q.y - y * q.x;
			w = w * q.w - x * q.x - y * q.y - z * q.z;
		}

			//	compare
			//	\param q: other quaternion
			// \return \b true if not equal
		bool operator!=( const gtQuaternion& q )const{
			if( x != q.x ) return true;
			if( y != q.y ) return true;
			if( z != q.z ) return true;
			if( w != q.w ) return true;
			return false;
		}

			//	compare
			//	\param q: other quaternion
			// \return \b true if equal
		bool operator==( const gtQuaternion& q )const{
			if( x != q.x ) return false;
			if( y != q.y ) return false;
			if( z != q.z ) return false;
			if( w != q.w ) return false;
			return true;
		}

			//	add other
			//	\param q: other quaternion
			// \return new quaternion
		gtQuaternion operator+( const gtQuaternion& o ) const {
			return gtQuaternion(
				x + o.x,
				y + o.y,
				z + o.z,
				w + o.w );
		}

			//	substract other
			//	\param q: other quaternion
			// \return new quaternion
		gtQuaternion operator-( const gtQuaternion& o ) const {
			return gtQuaternion(
				x - o.x,
				y - o.y,
				z - o.z,
				w - o.w );
		}

			//	invert
			// \return new quaternion
		gtQuaternion operator-(){
			x = -x;
			y = -y;
			z = -z;
			return gtQuaternion( x, y, z, w );
		}

			//	length
			// \return length
		f32	getLength(){
			return std::sqrt( x*x+y*y+z*z+w*w );
		}

			//	normalize
			// \return normalized quaternion
		gtQuaternion& normalize(){
			f32 len = this->getLength();
			if( len ){
				f32 len2 = gtConst1F / len;
				x *= len2;
				y *= len2;
				z *= len2;
				w *= len2;
			}
			return *this;
		}

			//	invert
		void invert(){
			x = -x;
			y = -y;
			z = -z;
		}

	};

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
