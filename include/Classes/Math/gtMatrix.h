#pragma once
#ifndef __GT_MATRIX_H__
#define __GT_MATRIX_H__

namespace gost{
	
	class gtMatrix4{

		v4f m_data[ gtConst4U ]; //< components

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
			m_data[ gtConst0U ] = x;
			m_data[ gtConst1U ] = y;
			m_data[ gtConst2U ] = z;
			m_data[ gtConst3U ] = w;
		}

			//	fill matrix with one value
		void		fill( f32 v ){
			m_data[ gtConst0U ].fill( v );
			m_data[ gtConst1U ].fill( v );
			m_data[ gtConst2U ].fill( v );
			m_data[ gtConst3U ].fill( v );
		}

			// set all components = 0
		void		zero(){
			fill( gtConst0F );
		}

		void		identity(){
			auto * p = this->getPtr();
			p[ gtConst0U ] = gtConst1F;
			p[ gtConst1U ] = gtConst0F;
			p[ gtConst2U ] = gtConst0F;
			p[ gtConst3U ] = gtConst0F;

			p[ gtConst4U ] = gtConst0F;
			p[ gtConst5U ] = gtConst1F;
			p[ gtConst6U ] = gtConst0F;
			p[ gtConst7U ] = gtConst0F;

			p[ gtConst8U ] = gtConst0F;
			p[ gtConst9U ] = gtConst0F;
			p[ gtConst10U ] = gtConst1F;
			p[ gtConst11U ] = gtConst0F;

			p[ gtConst12U ] = gtConst0F;
			p[ gtConst13U ] = gtConst0F;
			p[ gtConst14U ] = gtConst0F;
			p[ gtConst15U ] = gtConst1F;

		}

			// get pointer
			//	\return pointer
		f32 *		getPtr(){
			return reinterpret_cast<f32*>(&m_data);
		}

			//	get/set row
			// \param i: component id
			// \return vector
		v4f& operator[]( u32 i ){
			return m_data[ i ];
		}

			//	get row
			// \param i: component id
			// \return vector
		const v4f& operator[]( u32 i ) const {
			return m_data[ i ];
		}

			//	add
			// \param m: other matrix
			// \return new matrix
		gtMatrix4 operator+( const gtMatrix4& m ) const {
			gtMatrix4 out = *this;

			out[ gtConst0U ] += m[ gtConst0U ];
			out[ gtConst1U ] += m[ gtConst1U ];
			out[ gtConst2U ] += m[ gtConst2U ];
			out[ gtConst3U ] += m[ gtConst3U ];

			return out;
		}

			//	substract
			// \param m: other matrix
			// \return new matrix
		gtMatrix4 operator-( const gtMatrix4& m ) const {
			gtMatrix4 out = *this;

			out[ gtConst0U ] -= m[ gtConst0U ];
			out[ gtConst1U ] -= m[ gtConst1U ];
			out[ gtConst2U ] -= m[ gtConst2U ];
			out[ gtConst3U ] -= m[ gtConst3U ];

			return out;
		}

			//	multiplication
			// \param m: other matrix
			// \return new matrix
		gtMatrix4 operator*( const gtMatrix4& m ) const {
			return gtMatrix4(
				m_data[ gtConst0U ] * m[ gtConst0U ].x + m_data[ gtConst1U ] * m[ gtConst0U ].y + m_data[ gtConst2U ] * m[ gtConst0U ].z + m_data[ gtConst3U ] * m[ gtConst0U ].w,
				m_data[ gtConst0U ] * m[ gtConst1U ].x + m_data[ gtConst1U ] * m[ gtConst1U ].y + m_data[ gtConst2U ] * m[ gtConst1U ].z + m_data[ gtConst3U ] * m[ gtConst1U ].w,
				m_data[ gtConst0U ] * m[ gtConst2U ].x + m_data[ gtConst1U ] * m[ gtConst2U ].y + m_data[ gtConst2U ] * m[ gtConst2U ].z + m_data[ gtConst3U ] * m[ gtConst2U ].w,
				m_data[ gtConst0U ] * m[ gtConst3U ].x + m_data[ gtConst1U ] * m[ gtConst3U ].y + m_data[ gtConst2U ] * m[ gtConst3U ].z + m_data[ gtConst3U ] * m[ gtConst3U ].w
			);
		}

			//	divide
			// \param m: other matrix
			// \return new matrix
		gtMatrix4 operator/( const gtMatrix4& m ) const {
			gtMatrix4 out = *this;

			out[ gtConst0U ] /= m[ gtConst0U ];
			out[ gtConst1U ] /= m[ gtConst1U ];
			out[ gtConst2U ] /= m[ gtConst2U ];
			out[ gtConst3U ] /= m[ gtConst3U ];

			return out;
		}

			//	add
			// \param m: other matrix
			// \return this matrix
		gtMatrix4& operator+=( const gtMatrix4& m ){
			m_data[ gtConst0U ] += m[ gtConst0U ];
			m_data[ gtConst1U ] += m[ gtConst1U ];
			m_data[ gtConst2U ] += m[ gtConst2U ];
			m_data[ gtConst3U ] += m[ gtConst3U ];
			return *this;
		}

			//	substract
			// \param m: other matrix
			// \return this matrix
		gtMatrix4& operator-=( const gtMatrix4& m ){
			m_data[ gtConst0U ] -= m[ gtConst0U ];
			m_data[ gtConst1U ] -= m[ gtConst1U ];
			m_data[ gtConst2U ] -= m[ gtConst2U ];
			m_data[ gtConst3U ] -= m[ gtConst3U ];
			return *this;
		}

			//	multiplication
			// \param m: other matrix
			// \return this matrix
		gtMatrix4& operator*=( const gtMatrix4& m ){
			m_data[ gtConst0U ] *= m[ gtConst0U ];
			m_data[ gtConst1U ] *= m[ gtConst1U ];
			m_data[ gtConst2U ] *= m[ gtConst2U ];
			m_data[ gtConst3U ] *= m[ gtConst3U ];
			return *this;
		}

			//	divide
			// \param m: other matrix
			// \return this matrix
		gtMatrix4& operator/=( const gtMatrix4& m ){
			m_data[ gtConst0U ] /= m[ gtConst0U ];
			m_data[ gtConst1U ] /= m[ gtConst1U ];
			m_data[ gtConst2U ] /= m[ gtConst2U ];
			m_data[ gtConst3U ] /= m[ gtConst3U ];
			return *this;
		}

			//	transpose
		void transpose(){
			gtMatrix4 tmp;
			tmp[ gtConst0U ].x = this->m_data[ gtConst0U ].x; //0
			tmp[ gtConst0U ].y = this->m_data[ gtConst1U ].x; //1
			tmp[ gtConst0U ].z = this->m_data[ gtConst2U ].x; //2
			tmp[ gtConst0U ].w = this->m_data[ gtConst3U ].x; //3

			tmp[ gtConst1U ].x = this->m_data[ gtConst0U ].y; //4
			tmp[ gtConst1U ].y = this->m_data[ gtConst1U ].y; //5
			tmp[ gtConst1U ].z = this->m_data[ gtConst2U ].y; //6
			tmp[ gtConst1U ].w = this->m_data[ gtConst3U ].y; //7

			tmp[ gtConst2U ].x = this->m_data[ gtConst0U ].z; //8
			tmp[ gtConst2U ].y = this->m_data[ gtConst1U ].z; //9
			tmp[ gtConst2U ].z = this->m_data[ gtConst2U ].z; //10
			tmp[ gtConst2U ].w = this->m_data[ gtConst3U ].z; //11

			tmp[ gtConst3U ].x = this->m_data[ gtConst0U ].w; //12
			tmp[ gtConst3U ].y = this->m_data[ gtConst1U ].w; //13
			tmp[ gtConst3U ].z = this->m_data[ gtConst2U ].w; //14
			tmp[ gtConst3U ].w = this->m_data[ gtConst3U ].w; //15
			this->m_data[ gtConst0U ] = tmp[ gtConst0U ];
			this->m_data[ gtConst1U ] = tmp[ gtConst1U ];
			this->m_data[ gtConst2U ] = tmp[ gtConst2U ];
			this->m_data[ gtConst3U ] = tmp[ gtConst3U ];
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
			out[ gtConst0U ] = v4f( W, gtConst0F, gtConst0F, gtConst0F );
			out[ gtConst1U ] = v4f( gtConst0F, H, gtConst0F, gtConst0F );
			out[ gtConst2U ] = v4f( gtConst0F, gtConst0F, Far / (Near - Far), gtConst1F );
			out[ gtConst3U ] = v4f( gtConst0F, gtConst0F, -out[ gtConst2U ].z * Near, gtConst0F );
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
			out[ gtConst0U ] = v4f( W, gtConst0F, gtConst0F, gtConst0F );
			out[ gtConst1U ] = v4f( gtConst0F, H, gtConst0F, gtConst0F );
			out[ gtConst2U ] = v4f( gtConst0F, gtConst0F, Far / (Near - Far), -gtConst1F );
			out[ gtConst3U ] = v4f( gtConst0F, gtConst0F, out[ gtConst2U ].z * Near, gtConst0F );
		}

			//	create orthogonal matrix for right hand coordinate system
			// \param in_out: projection matrix
			// \param width: width of viewport
			// \param height: height of viewport
			// \param Near: near clip plane
			// \param Far: far clip plane
		GT_FORCE_INLINE void  makeOrthoRHMatrix( gtMatrix4& out, f32 width, f32 height,
			f32& Near, f32 Far ){
			out[ gtConst0U ] = v4f( gtConst2F / width, gtConst0F, gtConst0F, gtConst0F );
			out[ gtConst1U ] = v4f( gtConst0F, gtConst2F / height, gtConst0F, gtConst0F );
			out[ gtConst2U ] = v4f( gtConst0F, gtConst0F, gtConst1F / (Near - Far), gtConst0F );
			out[ gtConst3U ] = v4f( gtConst0F, gtConst0F, out[ gtConst2U ].z * Near, gtConst1F );
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

			out[ gtConst0U ].x = s.x;
			out[ gtConst1U ].x = s.y;
			out[ gtConst2U ].x = s.z;
			out[ gtConst0U ].y = u.x;
			out[ gtConst1U ].y = u.y;
			out[ gtConst2U ].y = u.z;
			out[ gtConst0U ].z =-f.x;
			out[ gtConst1U ].z =-f.y;
			out[ gtConst2U ].z =-f.z;
			out[ gtConst3U ].x =-math::dot( s, eye );
			out[ gtConst3U ].y =-math::dot( u, eye );
			out[ gtConst3U ].z = math::dot( f, eye );
		}

		template<typename vector_type>
		void  makeLookAtLHMatrix( const vector_type& eye, const vector_type& center,const vector_type& up, gtMatrix4& out){
			vector_type f( center - eye );
			f.normalize();
			
			vector_type s( math::cross( up, f ) );
			s.normalize();

			vector_type u( math::cross( f, s ) );

			out.identity();

			out[ gtConst0U ].x = s.x;
			out[ gtConst1U ].x = s.y;
			out[ gtConst2U ].x = s.z;
			out[ gtConst0U ].y = u.x;
			out[ gtConst1U ].y = u.y;
			out[ gtConst2U ].y = u.z;
			out[ gtConst0U ].z = f.x;
			out[ gtConst1U ].z = f.y;
			out[ gtConst2U ].z = f.z;
			out[ gtConst3U ].x =-math::dot( s, eye );
			out[ gtConst3U ].y =-math::dot( u, eye );
			out[ gtConst3U ].z =-math::dot( f, eye );
		}

		template<typename vector_type>
		void makeTranslationMatrix( const vector_type& position, gtMatrix4& out ){
			out[ gtConst3U ].x = position.x;
			out[ gtConst3U ].y = position.y;
			out[ gtConst3U ].z = position.z;
		}

		template<typename vector_type>
		void makeScaleMatrix( const vector_type& scale, gtMatrix4& out ){
			out[ gtConst0U ].x = scale.x;
			out[ gtConst1U ].y = scale.y;
			out[ gtConst2U ].z = scale.z;
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

			out[ gtConst0U ].x = gtConst1F - ( yy + zz );
			out[ gtConst0U ].y = xy - wz;
			out[ gtConst0U ].z = xz + wy;

			out[ gtConst1U ].x = xy + wz;
			out[ gtConst1U ].y = gtConst1F - ( xx + zz );
			out[ gtConst1U ].z = yz - wx;

			out[ gtConst2U ].x = xz - wy;
			out[ gtConst2U ].y = yz + wx;
			out[ gtConst2U ].z = gtConst1F - ( xx + yy );

			out[ gtConst0U ].w	= out[ gtConst1U ].w = out[ gtConst2U ].w = out[ gtConst3U ].x = out[ gtConst3U ].y = out[ gtConst3U ].z = gtConst0F;
			out[ gtConst3U ].w  = gtConst1F;
		}

			//Vector-matrix product
		template<typename vector_type>
		vector_type mul( const vector_type& vec, const gtMatrix4& mat ){
			return vector_type(
				mat[ gtConst0U ].x * vec.x + mat[ gtConst1U ].x * vec.y + mat[ gtConst2U ].x * vec.z,
				mat[ gtConst0U ].y * vec.x + mat[ gtConst1U ].y * vec.y + mat[ gtConst2U ].y * vec.z,
				mat[ gtConst0U ].z * vec.x + mat[ gtConst1U ].z * vec.y + mat[ gtConst2U ].z * vec.z
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