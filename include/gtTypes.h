#pragma once
#ifndef __GT_TYPES_H__
#define __GT_TYPES_H__

namespace gost{

	enum class gtSide : unsigned int{
	  /*POSX, NEGX, POSY,NEGY, POSZ	, NEGZ*/
		LEFT, RIGHT, UP, DOWN, FRONT, BACK
	};

	enum class gtBoundingVolumeType : unsigned int{
		sphere,
		obb,
		aabb,
		convex
	};

    #if defined(GT_PLATFORM_WIN32)
    #	define	GT_IMPORT __declspec(dllimport)
    #	define	GT_EXPORT __declspec(dllexport)
    #if (_MSC_VER > 1700 )
    #   define	GT_TYPE( x, y ) using x = y
    #	define	GT_SPRINTF sprintf_s
    #	define	GT_SNPRINTF _snprintf_s
    #	define	GT_CDECL __cdecl
    #else
    #   define	GT_TYPE( x, y ) typedef (y) (x)
    #	define	GT_SPRINTF sprintf
    #	define	GT_SNPRINTF snprintf
    #	define	GT_CDECL
    #endif
    #elif defined(__linux__)
    #   define	GT_TYPE(x,y) typedef y x
    #	define	GT_IMPORT
    #	define	GT_EXPORT
    #	define	GT_SPRINTF sprintf
    #	define	GT_SNPRINTF snprintf
    #	define	GT_CDECL
    #endif


    #if (_MSC_VER > 1700 || __cplusplus >= 201103L )
    #undef GT_TYPE
    #   define	GT_TYPE( x, y ) using x = y
    #endif

	#ifdef GT_EXPORTS
    #	define GT_API GT_EXPORT
    #else
    #	define GT_API GT_IMPORT
    #endif

	#if defined(_MSC_VER) || ((__BORLANDC__ >= 0x530) && !defined(__STRICT_ANSI__))
    #   define GT_UCHAR		 unsigned	__int8
    #   define GT_CHAR					__int8
    #   define GT_USHORT	 unsigned	__int16
    #   define GT_SHORT					__int16
    #   define GT_UINT		 unsigned	__int32
    #   define GT_INT					__int32
    #   define GT_ULONGLONG  unsigned	__int64
    #   define GT_LONGLONG				__int64
	 #else
	#   define GT_UCHAR		 unsigned	char
    #   define GT_CHAR					char
    #   define GT_USHORT	 unsigned	short
    #   define GT_SHORT					short
    #   define GT_UINT		 unsigned	int
    #   define GT_INT					int
    #   define GT_ULONGLONG  unsigned	long long
    #   define GT_LONGLONG				long long
    #endif

	GT_TYPE( c8,	char );				//< See \ref _GT_TYPE
	GT_TYPE( word,	unsigned short );	//< See \ref _GT_TYPE
	GT_TYPE( dword, unsigned int );		//< See \ref _GT_TYPE
	GT_TYPE( uint,	unsigned int );		//< See \ref _GT_TYPE
	GT_TYPE( ulong, unsigned long );	//< See \ref _GT_TYPE

	GT_TYPE( u8,  GT_UCHAR );			//< See \ref _GT_TYPE
	GT_TYPE( s8,  GT_CHAR );			//< See \ref _GT_TYPE
	GT_TYPE( u16, GT_USHORT );			//< See \ref _GT_TYPE
	GT_TYPE( s16, GT_SHORT );			//< See \ref _GT_TYPE
	GT_TYPE( u32, GT_UINT );			//< See \ref _GT_TYPE
	GT_TYPE( s32, GT_INT );				//< See \ref _GT_TYPE
	GT_TYPE( u64, GT_ULONGLONG );		//< See \ref _GT_TYPE
	GT_TYPE( s64, GT_LONGLONG );		//< See \ref _GT_TYPE
	GT_TYPE( f32, float );				//< See \ref _GT_TYPE
	GT_TYPE( f64, double );				//< See \ref _GT_TYPE

	#if defined(GT_32)
	GT_TYPE( gtAddressType, u32 );
	#else
	GT_TYPE( gtAddressType, u64 );
	#endif


	#if defined(GT_REAL_64)
	GT_TYPE( gtReal, f64 );				//< See \ref _GT_TYPE
	#else
	GT_TYPE( gtReal, f32 );				//< See \ref _GT_TYPE
	#endif



	#define GT_FUNCTION __FUNCTION__
	#if defined(GT_PLATFORM_LINUX)
	#define GT_FILE    __FILE__
	#else
	#define GT_FILE    __FILEW__
	#endif
	#define GT_LINE    __LINE__

	#if defined(DEBUG) || defined(_DEBUG)
	#	define	GT_DEBUG
	#endif

	#if defined(__GNUC__) && (__GNUC__ >= 3)
	GT_TYPE( gt_va_list, __builtin_va_list );
	#elif defined(GT_COMPILER_ARMCC)
	#include <stdarg.h>
	GT_TYPE( gt_va_list, va_list );
	#else
	GT_TYPE( gt_va_list, va_list );
	#endif

	#if defined(GT_PLATFORM_PS4)
	#	define GT_FORCE_INLINE inline __attribute__((always_inline))
	#elif defined(GT_COMPILER_MWERKS)
	#	define GT_FORCE_INLINE inline
	#elil defined(GT_COMPILER_INTEL)
	#	define GT_FORCE_INLINE inline
	#elif defined(GT_COMPILER_MSVC)
	#	define GT_FORCE_INLINE __forceinline
	#else
	#	define GT_FORCE_INLINE inline
	#endif


	struct gtBits{
		bool b0:1, b1:1, b2:1, b3:1, b4:1, b5:1, b6:1, b7:1;
	};

	union gtByte{
		gtByte():byte( 0u ){}
		gtBits bits;
		u8 byte;
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
