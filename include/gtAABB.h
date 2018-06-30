#pragma once
#ifndef __GT_AABB_H__
#define __GT_AABB_H__ //< include guard

namespace gost{

		// Axis-Aligned Bounding Box
	class gtAabb{
	public:

		gtAabb(){ }

		gtAabb( const v3f& min, const v3f& max ):
			m_min( min ), m_max( max ){ }

		void add( const v3f& point ){
			if( point.x < m_min.x ) m_min.x = point.x;
			if( point.y < m_min.y ) m_min.y = point.y;
			if( point.z < m_min.z ) m_min.z = point.z;

			if( point.x > m_max.x ) m_max.x = point.x;
			if( point.y > m_max.y ) m_max.y = point.y;
			if( point.z > m_max.z ) m_max.z = point.z;
		}

		void add( const gtAabb& box ){
			if( box.m_min.x < m_min.x ) m_min.x = box.m_min.x;
			if( box.m_min.y < m_min.y ) m_min.y = box.m_min.y;
			if( box.m_min.z < m_min.z ) m_min.z = box.m_min.z;

			if( box.m_max.x > m_max.x ) m_max.x = box.m_max.x;
			if( box.m_max.y > m_max.y ) m_max.y = box.m_max.y;
			if( box.m_max.z > m_max.z ) m_max.z = box.m_max.z;
		}

		void center( v3f& v ){
			v = v3f( m_min + m_max );
			v *= 0.5f;
		}

		void extent( v3f& v ){
			v = v3f( m_max - m_min );
		}

		bool isEmpty() const {
			return (m_min == m_max);
		}

		void reset(){
			m_min = m_max = v3f();
		}

		v3f m_min, m_max;
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