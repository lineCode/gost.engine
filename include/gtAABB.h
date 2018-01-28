/*!	GOST
	\file gtAABB.h
	\brief Axis-Aligned Bounding Box
*/

#pragma once
#ifndef __GT_AABB_H__
#define __GT_AABB_H__ ///< include guard

namespace gost{

		/// Axis-Aligned Bounding Box
	class gtAabb{
	public:

			///	default constructor
		gtAabb( void ){ }

			///	Установить по значениям
			/// \param min: minimum
			/// \param max: maximum
		gtAabb( const v4f& min, const v4f& max ):
			m_min( min ), m_max( max ){ }

			/// is empty
			/// \return \b true if empty
		bool isEmpty( void ) const {
			return (m_min == m_max);
		}

			/// reset
		void reset( void ){
			m_max = v4f();
			m_min = m_max;
		}

			///	Если точка удовлетворяет условию, aabb растёт
			///	\param point: vertex position for add
		void add( const v3f& point ){
			if( point.x_ < m_min.x_ ) m_min.x_ = point.x_;
			if( point.y_ < m_min.y_ ) m_min.y_ = point.y_;
			if( point.z_ < m_min.z_ ) m_min.z_ = point.z_;

			if( point.x_ > m_max.x_ ) m_max.x_ = point.x_;
			if( point.y_ > m_max.y_ ) m_max.y_ = point.y_;
			if( point.z_ > m_max.z_ ) m_max.z_ = point.z_;
		}

			///	Если размер коробки удовлетворяет условию, aabb растёт
			///	\param box: other aabb
		void add( const gtAabb& box ){
			if( box.m_min.x_ < m_min.x_ ) m_min.x_ = box.m_min.x_;
			if( box.m_min.y_ < m_min.y_ ) m_min.y_ = box.m_min.y_;
			if( box.m_min.z_ < m_min.z_ ) m_min.z_ = box.m_min.z_;

			if( box.m_max.x_ > m_max.x_ ) m_max.x_ = box.m_max.x_;
			if( box.m_max.y_ > m_max.y_ ) m_max.y_ = box.m_max.y_;
			if( box.m_max.z_ > m_max.z_ ) m_max.z_ = box.m_max.z_;
		}

			///	размер коробки
			/// \param v: [\b in/out] vector for size
		void extent( v4f& v ){
			v = v4f( m_max - m_min );
		}

			/// центр коробки
			/// \param v: [\b in/out] center
		void center( v4f& v ){
			v = v4f( m_min + m_max );
			v *= 0.5f;
		}

		v4f m_min, m_max; ///< components

	};

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