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
				m_data[ 0 ] * m[0].x + m_data[ 1 ] * m[0].y + m_data[ 2 ] * m[0].z + m_data[ 3 ] * m[0].w,
				m_data[ 0 ] * m[1].x + m_data[ 1 ] * m[1].y + m_data[ 2 ] * m[1].z + m_data[ 3 ] * m[1].w,
				m_data[ 0 ] * m[2].x + m_data[ 1 ] * m[2].y + m_data[ 2 ] * m[2].z + m_data[ 3 ] * m[2].w,
				m_data[ 0 ] * m[3].x + m_data[ 1 ] * m[3].y + m_data[ 2 ] * m[3].z + m_data[ 3 ] * m[3].w
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
			tmp[ 0 ].x = this->m_data[ 0 ].x; //0
			tmp[ 0 ].y = this->m_data[ 1 ].x; //1
			tmp[ 0 ].z = this->m_data[ 2 ].x; //2
			tmp[ 0 ].w = this->m_data[ 3 ].x; //3

			tmp[ 1 ].x = this->m_data[ 0 ].y; //4
			tmp[ 1 ].y = this->m_data[ 1 ].y; //5
			tmp[ 1 ].z = this->m_data[ 2 ].y; //6
			tmp[ 1 ].w = this->m_data[ 3 ].y; //7

			tmp[ 2 ].x = this->m_data[ 0 ].z; //8
			tmp[ 2 ].y = this->m_data[ 1 ].z; //9
			tmp[ 2 ].z = this->m_data[ 2 ].z; //10
			tmp[ 2 ].w = this->m_data[ 3 ].z; //11

			tmp[ 3 ].x = this->m_data[ 0 ].w; //12
			tmp[ 3 ].y = this->m_data[ 1 ].w; //13
			tmp[ 3 ].z = this->m_data[ 2 ].w; //14
			tmp[ 3 ].w = this->m_data[ 3 ].w; //15
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
			in_out[0] = v4f( W, 0.f, 0.f, 0.f );
			in_out[1] = v4f( 0.f, H, 0.f, 0.f );
			in_out[2] = v4f( 0.f, 0.f, Far / (Near - Far), 1.f );
			in_out[3] = v4f( 0.f, 0.f, -in_out[2].z * Near, 0.f );
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
			in_out[0] = v4f( W, 0.f, 0.f, 0.f );
			in_out[1] = v4f( 0.f, H, 0.f, 0.f );
			in_out[2] = v4f( 0.f, 0.f, Far / (Near - Far), -1.f );
			in_out[3] = v4f( 0.f, 0.f, in_out[2].z * Near, 0.f );
		}

			//	create orthogonal matrix for right hand coordinate system
			// \param in_out: projection matrix
			// \param width: width of viewport
			// \param height: height of viewport
			// \param Near: near clip plane
			// \param Far: far clip plane
		GT_FORCE_INLINE void  makeOrthoRHMatrix( gtMatrix4& in_out, f32 width, f32 height,
			f32& Near, f32 Far ){
			in_out[0] = v4f( 2.f / width, 0.f, 0.f, 0.f );
			in_out[1] = v4f( 0.f, 2.f / height, 0.f, 0.f );
			in_out[2] = v4f( 0.f, 0.f, 1.f / (Near - Far), 0.f );
			in_out[3] = v4f( 0.f, 0.f, in_out[2].z * Near, 1.f );
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

			in_out[0].x = s.x;
			in_out[1].x = s.y;
			in_out[2].x = s.z;
			in_out[0].y = u.x;
			in_out[1].y = u.y;
			in_out[2].y = u.z;
			in_out[0].z =-f.x;
			in_out[1].z =-f.y;
			in_out[2].z =-f.z;
			in_out[3].x =-math::dot( s, eye );
			in_out[3].y =-math::dot( u, eye );
			in_out[3].z = math::dot( f, eye );
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

			in_out[0].x = s.x;
			in_out[1].x = s.y;
			in_out[2].x = s.z;
			in_out[0].y = u.x;
			in_out[1].y = u.y;
			in_out[2].y = u.z;
			in_out[0].z = f.x;
			in_out[1].z = f.y;
			in_out[2].z = f.z;
			in_out[3].x =-math::dot( s, eye );
			in_out[3].y =-math::dot( u, eye );
			in_out[3].z =-math::dot( f, eye );
		}

			//	create translation matrix
			// \param in_out: translation matrix
			// \param position: position
			//	нужно чтобы у матрицы была главная диагональ = 1
		GT_FORCE_INLINE void makeTranslationMatrix( gtMatrix4& in_out, const v3f& position ){
			in_out[ 3 ].x = position.x;
			in_out[ 3 ].y = position.y;
			in_out[ 3 ].z = position.z;
		}

		GT_FORCE_INLINE void makeScaleMatrix( gtMatrix4& in_out, const v3f& scale ){
			in_out[ 0 ].x = scale.x;
			in_out[ 1 ].y = scale.y;
			in_out[ 2 ].z = scale.z;
		}

			//	create rotation matrix
			// \param in_out: rotation matrix
			// \param p: orientation
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

			in_out[ 0 ].x = 1.0f - ( yy + zz );
			in_out[ 0 ].y = xy - wz;
			in_out[ 0 ].z = xz + wy;

			in_out[ 1 ].x = xy + wz;
			in_out[ 1 ].y = 1.0f - ( xx + zz );
			in_out[ 1 ].z = yz - wx;

			in_out[ 2 ].x = xz - wy;
			in_out[ 2 ].y = yz + wx;
			in_out[ 2 ].z = 1.0f - ( xx + yy );


			in_out[0].w	=	in_out[1].w = in_out[2].w = in_out[3].x = in_out[3].y = in_out[3].z = 0.f;
			in_out[3].w  = 1.f;
		}

			//Vector-matrix product
		GT_FORCE_INLINE v3f mul( const v3f& vector, const gtMatrix4& matrix ){
			v3f result;
			
			result.x = math::dot(matrix[ 0u ], vector);
			result.y = math::dot(matrix[ 1u ], vector);
			result.z = math::dot(matrix[ 2u ], vector);

			return result;
		}


	}//end math::

}//end gost::

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