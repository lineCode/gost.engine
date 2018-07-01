#pragma once
#ifndef __GT_MATH_H__
#define __GT_MATH_H__

/*Doom 3*/
enum{
	LOOKUP_BITS				= 8,							
	EXP_POS					= 23,							
	EXP_BIAS				= 127,							
	LOOKUP_POS				= (EXP_POS-LOOKUP_BITS),
	SEED_POS				= (EXP_POS-8),
	SQRT_TABLE_SIZE			= (2<<LOOKUP_BITS),
	LOOKUP_MASK				= (SQRT_TABLE_SIZE-1)
};

#include "iSqrt_table.hpp"


namespace gost{

	// Here math functions and declarations
	namespace math{

constexpr f32 PI = 3.14159265358979323846f;

		/*Doom 3*/
		union _flint{
			s32 i;
			f32 f;
		};

			//Doom 3
		GT_FORCE_INLINE f32 invSqrt( f32 x ){
			s32 a = ((union _flint*)(&x))->i;
			union _flint seed;
			f64 y = x * 0.5f;
			seed.i = (( ( (3*EXP_BIAS-1) - ( (a >> EXP_POS) & 0xFF) ) >> 1)<<EXP_POS) | iSqrt[(a >> (EXP_POS-LOOKUP_BITS)) & LOOKUP_MASK];
			f64 r = seed.f;
			r = r * ( 1.5f - r * r * y );
			r = r * ( 1.5f - r * r * y );
			return (f32) r;
		}

		GT_FORCE_INLINE f32 degToRad( f32 degrees ){
			return degrees * ( PI / 180.f );
		}

		GT_FORCE_INLINE f32 radToDeg( f32 radians ){
			return radians * ( 180.f / PI );
		}
			
		GT_FORCE_INLINE f32 radians( f32 degrees ){
			return degrees * static_cast<f32>(0.01745329251994329576923690768489f);
		}

		GT_FORCE_INLINE f64 radians( f64 degrees ){
			return degrees * static_cast<f64>(0.01745329251994329576923690768489);
		}

		GT_FORCE_INLINE f32 degrees( f32 radians ){
			return radians * static_cast<f32>(57.295779513082320876798154814105);
		}

		GT_FORCE_INLINE f64 degrees( f64 radians ){
			return radians * static_cast<f64>(57.295779513082320876798154814105);
		}

		template<typename T>
		GT_FORCE_INLINE T lerp( const T& a, const T& b, const f32 t ){
			return ( T )( a * ( 1.f - t ) ) + ( b * t );
		}

	}

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