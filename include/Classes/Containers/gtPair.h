#pragma once
#ifndef __GT_PAIR_H__
#define __GT_PAIR_H__

namespace gost{

	template<typename Type1, typename Type2>
	class gtPair{
	public:

		typedef gtPair<Type1,Type2>& reference;
		typedef const gtPair<Type1,Type2>& const_reference;

		gtPair():m_first(),m_second(),compareFunc(nullptr){}
		gtPair( Type1 first, Type2 second ):
			m_first( first ), m_second( second ),compareFunc(nullptr)
		{}
		gtPair( Type1 first, Type2 second, bool(*pred)(const_reference This, const_reference Other) ):
			m_first( first ), m_second( second ),compareFunc(pred)
		{}

		Type1 m_first;
		Type2 m_second;

		reference operator=( const_reference o ){
			m_first = o.m_first;
			m_second = o.m_second;
			return *this;
		}

		bool operator<=( const_reference o ) const {
			if( compareFunc ) return compareFunc( *this, o );
			return false;
		}
		
		bool operator>=( const_reference o ) const {
			if( compareFunc ) return compareFunc( *this, o );
			return false;
		}

		bool operator>( const_reference o ) const {
			if( compareFunc ) return compareFunc( *this, o );
			return false;
		}

		bool operator<( const_reference o ) const {
			if( compareFunc ) return compareFunc( *this, o );
			return false;
		}

		bool(*compareFunc)(const_reference This, const_reference Other);
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
