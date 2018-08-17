#pragma once
#ifndef __GT_MATRIX_H__
#define __GT_MATRIX_H__

namespace gost{
	
	class gtMatrix4{

		v4f m_data[ 4u ]; //< components

	public:

			// c-tor
		gtMatrix4(){
			identity();
		}

			// c-tor with other matrix
		gtMatrix4( const gtMatrix4& m ){
			*this = m;
		}

			//	fill matrix with one value
		gtMatrix4( f32 v ){
			fill( v );
		}

			//	set matrix 4 vectors
		gtMatrix4( const v4f& x, const v4f& y, const v4f& z, const v4f& w ){
			m_data[ 0u ] = x;
			m_data[ 1u ] = y;
			m_data[ 2u ] = z;
			m_data[ 3u ] = w;
		}

			//	fill matrix with one value
		void		fill( f32 v ){
			m_data[ 0u ].fill( v );
			m_data[ 1u ].fill( v );
			m_data[ 2u ].fill( v );
			m_data[ 3u ].fill( v );
		}

			// set all components = 0
		void		zero(){
			fill( 0.f );
		}

		void		identity(){
			auto * p = this->getPtr();
			p[ 0u ] = 1.f;
			p[ 1u ] = 0.f;
			p[ 2u ] = 0.f;
			p[ 3u ] = 0.f;

			p[ 4u ] = 0.f;
			p[ gtConst5U ] = 1.f;
			p[ gtConst6U ] = 0.f;
			p[ gtConst7U ] = 0.f;

			p[ gtConst8U ] = 0.f;
			p[ gtConst9U ] = 0.f;
			p[ gtConst10U ] = 1.f;
			p[ gtConst11U ] = 0.f;

			p[ gtConst12U ] = 0.f;
			p[ gtConst13U ] = 0.f;
			p[ gtConst14U ] = 0.f;
			p[ gtConst15U ] = 1.f;

		}

		f32 * getPtr(){ return reinterpret_cast<f32*>(&m_data); }
		
		v4f&       operator[]( u32 i ){ return m_data[ i ]; }
		const v4f& operator[]( u32 i ) const { return m_data[ i ]; }

			//	add
			// \param m: other matrix
			// \return new matrix
		gtMatrix4 operator+( const gtMatrix4& m ) const {
			gtMatrix4 out = *this;

			out[ 0u ] += m[ 0u ];
			out[ 1u ] += m[ 1u ];
			out[ 2u ] += m[ 2u ];
			out[ 3u ] += m[ 3u ];

			return out;
		}

			//	substract
			// \param m: other matrix
			// \return new matrix
		gtMatrix4 operator-( const gtMatrix4& m ) const {
			gtMatrix4 out = *this;

			out[ 0u ] -= m[ 0u ];
			out[ 1u ] -= m[ 1u ];
			out[ 2u ] -= m[ 2u ];
			out[ 3u ] -= m[ 3u ];

			return out;
		}

			//	multiplication
			// \param m: other matrix
			// \return new matrix
		gtMatrix4 operator*( const gtMatrix4& m ) const {
			return gtMatrix4(
				m_data[ 0u ] * m[ 0u ].x + m_data[ 1u ] * m[ 0u ].y + m_data[ 2u ] * m[ 0u ].z + m_data[ 3u ] * m[ 0u ].w,
				m_data[ 0u ] * m[ 1u ].x + m_data[ 1u ] * m[ 1u ].y + m_data[ 2u ] * m[ 1u ].z + m_data[ 3u ] * m[ 1u ].w,
				m_data[ 0u ] * m[ 2u ].x + m_data[ 1u ] * m[ 2u ].y + m_data[ 2u ] * m[ 2u ].z + m_data[ 3u ] * m[ 2u ].w,
				m_data[ 0u ] * m[ 3u ].x + m_data[ 1u ] * m[ 3u ].y + m_data[ 2u ] * m[ 3u ].z + m_data[ 3u ] * m[ 3u ].w
			);
		}

			//	divide
			// \param m: other matrix
			// \return new matrix
		gtMatrix4 operator/( const gtMatrix4& m ) const {
			gtMatrix4 out = *this;

			out[ 0u ] /= m[ 0u ];
			out[ 1u ] /= m[ 1u ];
			out[ 2u ] /= m[ 2u ];
			out[ 3u ] /= m[ 3u ];

			return out;
		}

			//	add
			// \param m: other matrix
			// \return this matrix
		gtMatrix4& operator+=( const gtMatrix4& m ){
			m_data[ 0u ] += m[ 0u ];
			m_data[ 1u ] += m[ 1u ];
			m_data[ 2u ] += m[ 2u ];
			m_data[ 3u ] += m[ 3u ];
			return *this;
		}

			//	substract
			// \param m: other matrix
			// \return this matrix
		gtMatrix4& operator-=( const gtMatrix4& m ){
			m_data[ 0u ] -= m[ 0u ];
			m_data[ 1u ] -= m[ 1u ];
			m_data[ 2u ] -= m[ 2u ];
			m_data[ 3u ] -= m[ 3u ];
			return *this;
		}

			//	multiplication
			// \param m: other matrix
			// \return this matrix
		gtMatrix4& operator*=( const gtMatrix4& m ){
			m_data[ 0u ] *= m[ 0u ];
			m_data[ 1u ] *= m[ 1u ];
			m_data[ 2u ] *= m[ 2u ];
			m_data[ 3u ] *= m[ 3u ];
			return *this;
		}

			//	divide
			// \param m: other matrix
			// \return this matrix
		gtMatrix4& operator/=( const gtMatrix4& m ){
			m_data[ 0u ] /= m[ 0u ];
			m_data[ 1u ] /= m[ 1u ];
			m_data[ 2u ] /= m[ 2u ];
			m_data[ 3u ] /= m[ 3u ];
			return *this;
		}

			//	transpose
		void transpose(){
			gtMatrix4 tmp;
			tmp[ 0u ].x = this->m_data[ 0u ].x; //0
			tmp[ 0u ].y = this->m_data[ 1u ].x; //1
			tmp[ 0u ].z = this->m_data[ 2u ].x; //2
			tmp[ 0u ].w = this->m_data[ 3u ].x; //3

			tmp[ 1u ].x = this->m_data[ 0u ].y; //4
			tmp[ 1u ].y = this->m_data[ 1u ].y; //5
			tmp[ 1u ].z = this->m_data[ 2u ].y; //6
			tmp[ 1u ].w = this->m_data[ 3u ].y; //7

			tmp[ 2u ].x = this->m_data[ 0u ].z; //8
			tmp[ 2u ].y = this->m_data[ 1u ].z; //9
			tmp[ 2u ].z = this->m_data[ 2u ].z; //10
			tmp[ 2u ].w = this->m_data[ 3u ].z; //11

			tmp[ 3u ].x = this->m_data[ 0u ].w; //12
			tmp[ 3u ].y = this->m_data[ 1u ].w; //13
			tmp[ 3u ].z = this->m_data[ 2u ].w; //14
			tmp[ 3u ].w = this->m_data[ 3u ].w; //15
			this->m_data[ 0u ] = tmp[ 0u ];
			this->m_data[ 1u ] = tmp[ 1u ];
			this->m_data[ 2u ] = tmp[ 2u ];
			this->m_data[ 3u ] = tmp[ 3u ];
		}

		bool invert(){
			gtMatrix4 r0, r1, r2, r3;
			f32 a, det, invDet;
			f32* mat = reinterpret_cast<f32 *>(this);

			det = mat[0*4+0] * mat[1*4+1] - mat[0*4+1] * mat[1*4+0];
			if ( std::abs( det ) < 1e-14 ) {
				return false;
			}
			v4f dd;
			
			invDet = 1.0f / det;

			r0[0][0] =   mat[1*4+1] * invDet;
			r0[0][1] = - mat[0*4+1] * invDet;
			r0[1][0] = - mat[1*4+0] * invDet;
			r0[1][1] =   mat[0*4+0] * invDet;
			r1[0][0] = r0[0][0] * mat[0*4+2] + r0[0][1] * mat[1*4+2];
			r1[0][1] = r0[0][0] * mat[0*4+3] + r0[0][1] * mat[1*4+3];
			r1[1][0] = r0[1][0] * mat[0*4+2] + r0[1][1] * mat[1*4+2];
			r1[1][1] = r0[1][0] * mat[0*4+3] + r0[1][1] * mat[1*4+3];
			r2[0][0] = mat[2*4+0] * r1[0][0] + mat[2*4+1] * r1[1][0];
			r2[0][1] = mat[2*4+0] * r1[0][1] + mat[2*4+1] * r1[1][1];
			r2[1][0] = mat[3*4+0] * r1[0][0] + mat[3*4+1] * r1[1][0];
			r2[1][1] = mat[3*4+0] * r1[0][1] + mat[3*4+1] * r1[1][1];
			r3[0][0] = r2[0][0] - mat[2*4+2];
			r3[0][1] = r2[0][1] - mat[2*4+3];
			r3[1][0] = r2[1][0] - mat[3*4+2];
			r3[1][1] = r2[1][1] - mat[3*4+3];
		
			det = r3[0][0] * r3[1][1] - r3[0][1] * r3[1][0];
			if ( std::abs( det ) < 1e-14 ) {
				return false;
			}

			invDet = 1.0f / det;

			a = r3[0][0];
			r3[0][0] =   r3[1][1] * invDet;
			r3[0][1] = - r3[0][1] * invDet;
			r3[1][0] = - r3[1][0] * invDet;
			r3[1][1] =   a * invDet;
			r2[0][0] = mat[2*4+0] * r0[0][0] + mat[2*4+1] * r0[1][0];
			r2[0][1] = mat[2*4+0] * r0[0][1] + mat[2*4+1] * r0[1][1];
			r2[1][0] = mat[3*4+0] * r0[0][0] + mat[3*4+1] * r0[1][0];
			r2[1][1] = mat[3*4+0] * r0[0][1] + mat[3*4+1] * r0[1][1];
			mat[2*4+0] = r3[0][0] * r2[0][0] + r3[0][1] * r2[1][0];
			mat[2*4+1] = r3[0][0] * r2[0][1] + r3[0][1] * r2[1][1];
			mat[3*4+0] = r3[1][0] * r2[0][0] + r3[1][1] * r2[1][0];
			mat[3*4+1] = r3[1][0] * r2[0][1] + r3[1][1] * r2[1][1];
			mat[0*4+0] = r0[0][0] - r1[0][0] * mat[2*4+0] - r1[0][1] * mat[3*4+0];
			mat[0*4+1] = r0[0][1] - r1[0][0] * mat[2*4+1] - r1[0][1] * mat[3*4+1];
			mat[1*4+0] = r0[1][0] - r1[1][0] * mat[2*4+0] - r1[1][1] * mat[3*4+0];
			mat[1*4+1] = r0[1][1] - r1[1][0] * mat[2*4+1] - r1[1][1] * mat[3*4+1];
			mat[0*4+2] = r1[0][0] * r3[0][0] + r1[0][1] * r3[1][0];
			mat[0*4+3] = r1[0][0] * r3[0][1] + r1[0][1] * r3[1][1];
			mat[1*4+2] = r1[1][0] * r3[0][0] + r1[1][1] * r3[1][0];
			mat[1*4+3] = r1[1][0] * r3[0][1] + r1[1][1] * r3[1][1];
			mat[2*4+2] = -r3[0][0];
			mat[2*4+3] = -r3[0][1];
			mat[3*4+2] = -r3[1][0];
			mat[3*4+3] = -r3[1][1];

			return true;
		}

	};

	namespace math{

			//	create perspective matrix for left hand coordinate system
			// \param in_out: projection matrix
			// \param FOV: field of view
			// \param aspect: aspect ratio
			// \param Near: near clip plane
			// \param Far: far clip plane
		GT_FORCE_INLINE void  makePerspectiveLHMatrix( gtMatrix4& out, f32 FOV, f32 aspect,
			f32 Near, f32 Far){
			f32 S	=	std::sin( gtConst05F * FOV );
			f32 C	=	std::cos( gtConst05F * FOV );
			f32 H = C / S;
			f32 W = H / aspect;
			out[ 0 ] = v4f( W, 0.f, 0.f, 0.f );
			out[ 1 ] = v4f( 0.f, H, 0.f, 0.f );
			out[ 2 ] = v4f( 0.f, 0.f, Far/(Far-Near), 1.f );
			out[ 3 ] = v4f( 0.f, 0.f, -out[2].z * Near, 0.f );
		}

			//	create perspective matrix for right hand coordinate system
			// \param in_out: projection matrix
			// \param FOV: field of view
			// \param aspect: aspect ratio
			// \param Near: near clip plane
			// \param Far: far clip plane
		GT_FORCE_INLINE void  makePerspectiveRHMatrix( gtMatrix4& out, f32 FOV, f32 aspect,
			f32 Near, f32 Far){
			f32 S	=	std::sin( gtConst05F * FOV );
			f32 C	=	std::cos( gtConst05F * FOV );
			f32 H = C / S;
			f32 W = H / aspect;
			out[ 0u ] = v4f( W, 0.f, 0.f, 0.f );
			out[ 1u ] = v4f( 0.f, H, 0.f, 0.f );
			out[ 2u ] = v4f( 0.f, 0.f, Far / (Near - Far), -1.f );
			out[ 3u ] = v4f( 0.f, 0.f, out[ 2u ].z * Near, 0.f );
		}

			//	create orthogonal matrix for right hand coordinate system
			// \param in_out: projection matrix
			// \param width: width of viewport
			// \param height: height of viewport
			// \param Near: near clip plane
			// \param Far: far clip plane
		GT_FORCE_INLINE void  makeOrthoRHMatrix( gtMatrix4& out, f32 width, f32 height,
			f32& Near, f32 Far ){
			out[ 0u ] = v4f( 2.f / width, 0.f, 0.f, 0.f );
			out[ 1u ] = v4f( 0.f, 2.f / height, 0.f, 0.f );
			out[ 2u ] = v4f( 0.f, 0.f, 1.f / (Near - Far), 0.f );
			out[ 3u ] = v4f( 0.f, 0.f, out[ 2u ].z * Near, 1.f );
		}
		
			//	create \a look \a at matrix for right hand coordinate system
			// \param in_out: view matrix
			// \param eye: camera position
			// \param center: camera target
			// \param up: up vector
		template<typename vector_type>
		void  makeLookAtRHMatrix( const vector_type& eye, const vector_type& center, const vector_type& up, gtMatrix4& out ){
			vector_type f( center - eye );
			f.normalize();
			
			vector_type s( math::cross( f, up ) );
			s.normalize();

			vector_type u( math::cross( s, f ) );

			out.identity();

			out[ 0u ].x = s.x;
			out[ 1u ].x = s.y;
			out[ 2u ].x = s.z;
			out[ 0u ].y = u.x;
			out[ 1u ].y = u.y;
			out[ 2u ].y = u.z;
			out[ 0u ].z =-f.x;
			out[ 1u ].z =-f.y;
			out[ 2u ].z =-f.z;
			out[ 3u ].x =-math::dot( s, eye );
			out[ 3u ].y =-math::dot( u, eye );
			out[ 3u ].z = math::dot( f, eye );
		}

		template<typename vector_type>
		void  makeLookAtLHMatrix( const vector_type& eye, const vector_type& center,const vector_type& up, gtMatrix4& out){
			vector_type f( center - eye );
			f.normalize();
			
			vector_type s( math::cross( up, f ) );
			s.normalize();

			vector_type u( math::cross( f, s ) );

			out.identity();

			out[ 0u ].x = s.x;
			out[ 1u ].x = s.y;
			out[ 2u ].x = s.z;
			out[ 0u ].y = u.x;
			out[ 1u ].y = u.y;
			out[ 2u ].y = u.z;
			out[ 0u ].z = f.x;
			out[ 1u ].z = f.y;
			out[ 2u ].z = f.z;
			out[ 3u ].x =-math::dot( s, eye );
			out[ 3u ].y =-math::dot( u, eye );
			out[ 3u ].z =-math::dot( f, eye );
		}

		template<typename vector_type>
		void makeTranslationMatrix( const vector_type& position, gtMatrix4& out ){
			out[ 3u ].x = position.x;
			out[ 3u ].y = position.y;
			out[ 3u ].z = position.z;
		}

		template<typename vector_type>
		void makeScaleMatrix( const vector_type& scale, gtMatrix4& out ){
			out[ 0u ].x = scale.x;
			out[ 1u ].y = scale.y;
			out[ 2u ].z = scale.z;
		}

		GT_FORCE_INLINE void makeRotationMatrix( gtMatrix4& out, const gtQuaternion& p ){
			
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

			out[ 0u ].x = 1.f - ( yy + zz );
			out[ 0u ].y = xy - wz;
			out[ 0u ].z = xz + wy;

			out[ 1u ].x = xy + wz;
			out[ 1u ].y = 1.f - ( xx + zz );
			out[ 1u ].z = yz - wx;

			out[ 2u ].x = xz - wy;
			out[ 2u ].y = yz + wx;
			out[ 2u ].z = 1.f - ( xx + yy );

			out[ 0u ].w	= out[ 1u ].w = out[ 2u ].w = out[ 3u ].x = out[ 3u ].y = out[ 3u ].z = 0.f;
			out[ 3u ].w  = 1.f;
		}

			//Vector-matrix product
		template<typename vector_type>
		vector_type mul( const vector_type& vec, const gtMatrix4& mat ){
			return vector_type(
				mat[ 0u ].x * vec.x + mat[ 1u ].x * vec.y + mat[ 2u ].x * vec.z,
				mat[ 0u ].y * vec.x + mat[ 1u ].y * vec.y + mat[ 2u ].y * vec.z,
				mat[ 0u ].z * vec.x + mat[ 1u ].z * vec.y + mat[ 2u ].z * vec.z
			);
		}

		template<typename vector_type>
		vector_type mul4( const vector_type& vec, const gtMatrix4& mat ){
			return vector_type(
				mat[ 0u ].x * vec.x + mat[ 1u ].x * vec.y + mat[ 2u ].x * vec.z,
				mat[ 0u ].y * vec.x + mat[ 1u ].y * vec.y + mat[ 2u ].y * vec.z,
				mat[ 0u ].z * vec.x + mat[ 1u ].z * vec.y + mat[ 2u ].z * vec.z,
				mat[ 0u ].w * vec.x + mat[ 1u ].w * vec.y + mat[ 2u ].w * vec.z
			);
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