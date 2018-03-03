﻿#pragma once
#ifndef __GT_OBB_H__
#define __GT_OBB_H__ ///< include guard

namespace gost{

		// Oriented Bounding Box
		//	Сначала создаётся как старый AABB, но, если включено отображение, то вычисляется поворот, в соответствии с ориентацией объекта.
		//	Если объект повернулся, нужно обновить OBB. На основве нового OBB можно делать AABB.
	class gtObb{
	public:

			//	default constructor
		gtObb( void ){ }

			// reset
		void reset( void ){
			m_max = v3f();
			m_min = m_center = m_extent = m_max;
		}

			//	Если точка удовлетворяет условию, aabb растёт
			//	\param point: vertex position for add
		void add( const v3f& point ){
			if( point.x < m_min.x ) m_min.x = point.x;
			if( point.y < m_min.y ) m_min.y = point.y;
			if( point.z < m_min.z ) m_min.z = point.z;

			if( point.x > m_max.x ) m_max.x = point.x;
			if( point.y > m_max.y ) m_max.y = point.y;
			if( point.z > m_max.z ) m_max.z = point.z;
		}

			///	Если размер коробки удовлетворяет условию, aabb растёт
			///	\param box: other aabb
		void add( const gtObb& box ){
			if( box.m_min.x < m_min.x ) m_min.x = box.m_min.x;
			if( box.m_min.y < m_min.y ) m_min.y = box.m_min.y;
			if( box.m_min.z < m_min.z ) m_min.z = box.m_min.z;

			if( box.m_max.x > m_max.x ) m_max.x = box.m_max.x;
			if( box.m_max.y > m_max.y ) m_max.y = box.m_max.y;
			if( box.m_max.z > m_max.z ) m_max.z = box.m_max.z;
		}

			//	размер коробки
			// \param v: [\b in/out] vector for size
		void calculateExtentAndCenter( void ){
			m_extent = m_max - m_min;
			m_center = m_min + m_max;
			m_center *= 0.5f;
		}

		v3f m_min, m_max, m_extent, m_center; ///< components

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