/*!	GOST
	\file gtMatrix.h
	\brief matrix
*/

#pragma once
#ifndef __GT_MATRIX_H__
#define __GT_MATRIX_H__ ///< include guard

namespace gost{
	
		///	matrix with 4 components
	class gtMatrix4{

		v4f m_data[ 4u ]; ///< components

	public:

			/// c-tor
		gtMatrix4( void ){
			identity();
		}

			/// c-tor with other matrix
		gtMatrix4( const gtMatrix4& m ){
			*this = m;
		}

			///	fill matrix with one value
		gtMatrix4( f32 v ){
			fill( v );
		}

			///	set matrix 4 vectors
		gtMatrix4( const v4f& x, const v4f& y, const v4f& z, const v4f& w ){
			m_data[ 0 ] = x;
			m_data[ 1 ] = y;
			m_data[ 2 ] = z;
			m_data[ 3 ] = w;
		}

			///	fill matrix with one value
		void		fill( f32 v ){
			m_data[ 0u ].fill( v );
			m_data[ 1u ].fill( v );
			m_data[ 2u ].fill( v );
			m_data[ 3u ].fill( v );
		}

			/// set all components = 0
		void		zero( void ){
			fill( 0.f );
		}

			///	main diagonal - главная диагональ = 1
		void		identity( void ){
			auto * p = this->getPtr();
			p[ 0u ] = 1.f;
			p[ 1u ] = 0.f;
			p[ 2u ] = 0.f;
			p[ 3u ] = 0.f;

			p[ 4u ] = 0.f;
			p[ 5u ] = 1.f;
			p[ 6u ] = 0.f;
			p[ 7u ] = 0.f;

			p[ 8u ] = 0.f;
			p[ 9u ] = 0.f;
			p[ 10u ] = 1.f;
			p[ 11u ] = 0.f;

			p[ 12u ] = 0.f;
			p[ 13u ] = 0.f;
			p[ 14u ] = 0.f;
			p[ 15u ] = 1.f;

		}

			/// get pointer
			///	\return pointer
		f32 *		getPtr( void ){
			return reinterpret_cast<f32*>(&m_data);
		}

			///	get/set row
			/// \param i: component id
			/// \return vector
		v4f& operator[]( u32 i ){
			return m_data[ i ];
		}

			///	get row
			/// \param i: component id
			/// \return vector
		const v4f& operator[]( u32 i ) const {
			return m_data[ i ];
		}

			///	add
			/// \param m: other matrix
			/// \return new matrix
		gtMatrix4 operator+( const gtMatrix4& m ) const {
			gtMatrix4 out = *this;

			out[ 0u ] += m[ 0u ];
			out[ 1u ] += m[ 1u ];
			out[ 2u ] += m[ 2u ];
			out[ 3u ] += m[ 3u ];

			return out;
		}

			///	substract
			/// \param m: other matrix
			/// \return new matrix
		gtMatrix4 operator-( const gtMatrix4& m ) const {
			gtMatrix4 out = *this;

			out[ 0u ] -= m[ 0u ];
			out[ 1u ] -= m[ 1u ];
			out[ 2u ] -= m[ 2u ];
			out[ 3u ] -= m[ 3u ];

			return out;
		}

			///	multiplication
			/// \param m: other matrix
			/// \return new matrix
		gtMatrix4 operator*( const gtMatrix4& m ) const {
			return gtMatrix4(
				m_data[ 0 ] * m[0][0] + m_data[ 1 ] * m[0][1] + m_data[ 2 ] * m[0][2] + m_data[ 3 ] * m[0][3],
				m_data[ 0 ] * m[1][0] + m_data[ 1 ] * m[1][1] + m_data[ 2 ] * m[1][2] + m_data[ 3 ] * m[1][3],
				m_data[ 0 ] * m[2][0] + m_data[ 1 ] * m[2][1] + m_data[ 2 ] * m[2][2] + m_data[ 3 ] * m[2][3],
				m_data[ 0 ] * m[3][0] + m_data[ 1 ] * m[3][1] + m_data[ 2 ] * m[3][2] + m_data[ 3 ] * m[3][3]
			);
		}

			///	divide
			/// \param m: other matrix
			/// \return new matrix
		gtMatrix4 operator/( const gtMatrix4& m ) const {
			gtMatrix4 out = *this;

			out[ 0u ] /= m[ 0u ];
			out[ 1u ] /= m[ 1u ];
			out[ 2u ] /= m[ 2u ];
			out[ 3u ] /= m[ 3u ];

			return out;
		}

			///	add
			/// \param m: other matrix
			/// \return this matrix
		gtMatrix4& operator+=( const gtMatrix4& m ){
			m_data[ 0u ] += m[ 0u ];
			m_data[ 1u ] += m[ 1u ];
			m_data[ 2u ] += m[ 2u ];
			m_data[ 3u ] += m[ 3u ];
			return *this;
		}

			///	substract
			/// \param m: other matrix
			/// \return this matrix
		gtMatrix4& operator-=( const gtMatrix4& m ){
			m_data[ 0u ] -= m[ 0u ];
			m_data[ 1u ] -= m[ 1u ];
			m_data[ 2u ] -= m[ 2u ];
			m_data[ 3u ] -= m[ 3u ];
			return *this;
		}

			///	multiplication
			/// \param m: other matrix
			/// \return this matrix
		gtMatrix4& operator*=( const gtMatrix4& m ){
			m_data[ 0u ] *= m[ 0u ];
			m_data[ 1u ] *= m[ 1u ];
			m_data[ 2u ] *= m[ 2u ];
			m_data[ 3u ] *= m[ 3u ];
			return *this;
		}

			///	divide
			/// \param m: other matrix
			/// \return this matrix
		gtMatrix4& operator/=( const gtMatrix4& m ){
			m_data[ 0u ] /= m[ 0u ];
			m_data[ 1u ] /= m[ 1u ];
			m_data[ 2u ] /= m[ 2u ];
			m_data[ 3u ] /= m[ 3u ];
			return *this;
		}

			///	transpose
		void transpose( void ){
			gtMatrix4 tmp;
			tmp[ 0 ][ 0 ] = this->m_data[ 0 ][ 0 ]; //0
			tmp[ 0 ][ 1 ] = this->m_data[ 1 ][ 0 ]; //1
			tmp[ 0 ][ 2 ] = this->m_data[ 2 ][ 0 ]; //2
			tmp[ 0 ][ 3 ] = this->m_data[ 3 ][ 0 ]; //3

			tmp[ 1 ][ 0 ] = this->m_data[ 0 ][ 1 ]; //4
			tmp[ 1 ][ 1 ] = this->m_data[ 1 ][ 1 ]; //5
			tmp[ 1 ][ 2 ] = this->m_data[ 2 ][ 1 ]; //6
			tmp[ 1 ][ 3 ] = this->m_data[ 3 ][ 1 ]; //7

			tmp[ 2 ][ 0 ] = this->m_data[ 0 ][ 2 ]; //8
			tmp[ 2 ][ 1 ] = this->m_data[ 1 ][ 2 ]; //9
			tmp[ 2 ][ 2 ] = this->m_data[ 2 ][ 2 ]; //10
			tmp[ 2 ][ 3 ] = this->m_data[ 3 ][ 2 ]; //11

			tmp[ 3 ][ 0 ] = this->m_data[ 0 ][ 3 ]; //12
			tmp[ 3 ][ 1 ] = this->m_data[ 1 ][ 3 ]; //13
			tmp[ 3 ][ 2 ] = this->m_data[ 2 ][ 3 ]; //14
			tmp[ 3 ][ 3 ] = this->m_data[ 3 ][ 3 ]; //15
			this->m_data[ 0 ] = tmp[ 0 ];
			this->m_data[ 1 ] = tmp[ 1 ];
			this->m_data[ 2 ] = tmp[ 2 ];
			this->m_data[ 3 ] = tmp[ 3 ];
		}
	};

	namespace math{

			///	create perspective matrix for left hand coordinate system
			/// \param in_out: projection matrix
			/// \param FOV: field of view
			/// \param aspect: aspect ratio
			/// \param Near: near clip plane
			/// \param Far: far clip plane
		GT_FORCE_INLINE void  makePerspectiveLHMatrix( gtMatrix4& in_out, f32 FOV, f32 aspect,
			f32 Near, f32 Far){
			f32 S	=	std::sin( 0.5f * FOV );
			f32 C	=	std::cos( 0.5f * FOV );
			f32 H = C / S;
			f32 W = H / aspect;
			in_out[0] = v4f(IL_BEGIN W, 0.f, 0.f, 0.f IL_END);
			in_out[1] = v4f(IL_BEGIN 0.f, H, 0.f, 0.f IL_END);
			in_out[2] = v4f(IL_BEGIN 0.f, 0.f, Far / (Near - Far), 1.f IL_END);
			in_out[3] = v4f(IL_BEGIN 0.f, 0.f, -in_out[2][2] * Near, 0.f IL_END);
		}

			///	create perspective matrix for right hand coordinate system
			/// \param in_out: projection matrix
			/// \param FOV: field of view
			/// \param aspect: aspect ratio
			/// \param Near: near clip plane
			/// \param Far: far clip plane
		GT_FORCE_INLINE void  makePerspectiveRHMatrix( gtMatrix4& in_out, f32 FOV, f32 aspect,
			f32 Near, f32 Far){
			f32 S	=	std::sin( 0.5f * FOV );
			f32 C	=	std::cos( 0.5f * FOV );
			f32 H = C / S;
			f32 W = H / aspect;
			in_out[0] = v4f(IL_BEGIN W, 0.f, 0.f, 0.f IL_END);
			in_out[1] = v4f(IL_BEGIN 0.f, H, 0.f, 0.f IL_END);
			in_out[2] = v4f(IL_BEGIN 0.f, 0.f, Far / (Near - Far), -1.f IL_END);
			in_out[3] = v4f(IL_BEGIN 0.f, 0.f, in_out[2][2] * Near, 0.f IL_END);
		}

			///	create orthogonal matrix for right hand coordinate system
			/// \param in_out: projection matrix
			/// \param width: width of viewport
			/// \param height: height of viewport
			/// \param Near: near clip plane
			/// \param Far: far clip plane
		GT_FORCE_INLINE void  makeOrthoRHMatrix( gtMatrix4& in_out, f32 width, f32 height,
			f32& Near, f32 Far ){
			in_out[0] = v4f(IL_BEGIN 2.f / width, 0.f, 0.f, 0.f IL_END);
			in_out[1] = v4f(IL_BEGIN 0.f, 2.f / height, 0.f, 0.f IL_END);
			in_out[2] = v4f(IL_BEGIN 0.f, 0.f, 1.f / (Near - Far), 0.f IL_END);
			in_out[3] = v4f(IL_BEGIN 0.f, 0.f, in_out[2][2] * Near, 1.f IL_END);
		}
		
			///	create \a look \a at matrix for right hand coordinate system
			/// \param in_out: view matrix
			/// \param eye: camera position
			/// \param center: camera target
			/// \param up: up vector
		GT_FORCE_INLINE void  makeLookAtRHMatrix( gtMatrix4& in_out, const v3f& eye, const v3f& center, const v3f& up ){
			v3f f( center - eye );
			f.normalize();
			
			v3f s( math::cross( f, up ) );
			s.normalize();

			v3f u( math::cross( s, f ) );

			in_out.identity();

			in_out[0][0] = s.x_;
			in_out[1][0] = s.y_;
			in_out[2][0] = s.z_;
			in_out[0][1] = u.x_;
			in_out[1][1] = u.y_;
			in_out[2][1] = u.z_;
			in_out[0][2] =-f.x_;
			in_out[1][2] =-f.y_;
			in_out[2][2] =-f.z_;
			in_out[3][0] =-math::dot( s, eye );
			in_out[3][1] =-math::dot( u, eye );
			in_out[3][2] = math::dot( f, eye );
		}

			///	create \a look \a at matrix for left hand coordinate system
			/// \param in_out: view matrix
			/// \param eye: camera position
			/// \param center: camera target
			/// \param up: up vector
		GT_FORCE_INLINE void  makeLookAtLHMatrix( gtMatrix4& in_out, const v3f& eye, const v3f& center,const v3f& up){
			v3f f( center - eye );
			f.normalize();
			
			v3f s( math::cross( up, f ) );
			s.normalize();

			v3f u( math::cross( f, s ) );

			in_out.identity();

			in_out[0][0] = s.x_;
			in_out[1][0] = s.y_;
			in_out[2][0] = s.z_;
			in_out[0][1] = u.x_;
			in_out[1][1] = u.y_;
			in_out[2][1] = u.z_;
			in_out[0][2] = f.x_;
			in_out[1][2] = f.y_;
			in_out[2][2] = f.z_;
			in_out[3][0] =-math::dot( s, eye );
			in_out[3][1] =-math::dot( u, eye );
			in_out[3][2] =-math::dot( f, eye );
		}

			///	create translation matrix
			/// \param in_out: translation matrix
			/// \param position: position
			///	нужно чтобы у матрицы была главная диагональ = 1
		GT_FORCE_INLINE void makeTranslationMatrix( gtMatrix4& in_out, const v3f& position ){
			in_out[ 3 ][ 0 ] = position.x_;
			in_out[ 3 ][ 1 ] = position.y_;
			in_out[ 3 ][ 2 ] = position.z_;
		}

			///	create rotation matrix
			/// \param in_out: rotation matrix
			/// \param p: orientation
		GT_FORCE_INLINE void makeRotationMatrix( gtMatrix4& in_out, const gtQuaternion& p ){
			
			f32	wx, wy, wz;
			f32	xx, yy, yz;
			f32	xy, xz, zz;
			f32	x2, y2, z2;

			x2 = p.x + p.x;
			y2 = p.y + p.y;
			z2 = p.z + p.z;

			xx = p.x * x2;
			xy = p.x * y2;
			xz = p.x * z2;

			yy = p.y * y2;
			yz = p.y * z2;
			zz = p.z * z2;

			wx = p.w * x2;
			wy = p.w * y2;
			wz = p.w * z2;

			in_out[ 0 ][ 0 ] = 1.0f - ( yy + zz );
			in_out[ 0 ][ 1 ] = xy - wz;
			in_out[ 0 ][ 2 ] = xz + wy;

			in_out[ 1 ][ 0 ] = xy + wz;
			in_out[ 1 ][ 1 ] = 1.0f - ( xx + zz );
			in_out[ 1 ][ 2 ] = yz - wx;

			in_out[ 2 ][ 0 ] = xz - wy;
			in_out[ 2 ][ 1 ] = yz + wx;
			in_out[ 2 ][ 2 ] = 1.0f - ( xx + yy );


			in_out[0][3]	=	in_out[1][3] = in_out[2][3] = in_out[3][0] = in_out[3][1] = in_out[3][2] = 0;
			in_out[3][3]  = 1;
		}

			///Vector-matrix product
		GT_FORCE_INLINE v3f mul( const v3f& vector, const gtMatrix4& matrix ){
			v3f result;

			//auto * pm = &matrix[ 0u ][ 0u ];
			//auto * pv = &vector[ 0u ];

			/*result[ 0u ] = (pm[ 0u ]*pv[ 0u ])+(pm[ 1u ]*pv[ 1u ])+(pm[ 2u ]*pv[ 2u ]);
			result[ 1u ] = (pm[ 4u ]*pv[ 0u ])+(pm[ 5u ]*pv[ 1u ])+(pm[ 6u ]*pv[ 2u ]);
			result[ 2u ] = (pm[ 8u ]*pv[ 0u ])+(pm[ 9u ]*pv[ 1u ])+(pm[ 10u ]*pv[ 2u ]);*/
			
			result[ 0u ] = math::dot(matrix[ 0u ], vector);
			result[ 1u ] = math::dot(matrix[ 1u ], vector);
			result[ 2u ] = math::dot(matrix[ 2u ], vector);

			return result;
		}


	}//end math::

}//end gost::

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