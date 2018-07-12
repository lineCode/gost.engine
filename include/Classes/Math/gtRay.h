﻿#pragma once
#ifndef __GT_RAY_H__
#define __GT_RAY_H__

namespace gost{

	template<typename T>
	class gtRay{
	public:

		using reference = gtRay<T>&;
		using const_reference = const gtRay<T>&;

		gtRay():m_begin(),m_end(){}
		gtRay( const gtVector3<T>& begin, const gtVector3<T>& end ):
			m_begin( begin ), m_end( end )
		{}

		gtVector3<T> m_begin;
		gtVector3<T> m_end;

		reference operator=( const_reference o ){
			m_begin = o.m_begin;
			m_end = o.m_end;
			return *this;
		}

		bool operator<=( const_reference o ) const {
			return length() <= o.length();
		}
		
		bool operator>=( const_reference o ) const {
			return length() >= o.length();
		}

		bool operator>( const_reference o ) const {
			return length() > o.length();
		}

		bool operator<( const_reference o ) const {
			return length() < o.length();
		}

		T length(){
			return m_begin.distance( m_end );
		}

	};

}

#endif

/*
Copyright (c) 2018 532235

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
