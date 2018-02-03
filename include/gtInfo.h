/*!	GOST
	\file gtInfo.h
	\brief	get some information…
			
	Тут получаем информацию о компиляторах, архитектуре и прочее
*/

#pragma once
#ifndef __GT_INFO_H__
#define __GT_INFO_H__ ///< include guard




	/*! \brief для того чтобы в макрос вставлялись кавычки
	
		например
		#define PRINT(x) printf(QUOTE(x))
		...
		PRINT(int); // вывод "int"
	*/
	#define Q(x) #x
	/*! \brief \sa Q */
	#define QUOTE(x) Q(x)

	/*! \brief создаёт из 4х \b unsigned \b char целое значение

		Например:
		\code{.cpp}
			unsigned int val = GT_MAKEFOURCC(0xFF,0xFF,0x00,0xFF);
		\endcode

		\param ch0: первый байт
		\param ch1: второй байт
		\param ch2: третий байт
		\param ch3: четвёртый байт
	*/
	#define GT_MAKEFOURCC( ch0, ch1, ch2, ch3 )\
            ((u32)(u8)(ch0)|((u32)(u8)(ch1)<<8)|\
            ((u32)(u8)(ch2)<<16)|((u32)(u8)(ch3)<<24))

	//! \brief Для быстрого создания объекта \b gtPtr
	#define gtPtr_t(Type,Name,exp) gtPtr<Type> Name(gtPtrNew<Type>(exp))

	//	Компилятор
	//! \cond
	#if defined(__clang__)
	//! \endcond
	#	define GT_COMPILER_CLANG
	//! \cond
	#elif defined(__SNC__)
	//! \endcond
	#	define GT_COMPILER_SNC
	//! \cond
	#elif defined(__GNUC__) || defined(__CC_ARM) || defined(__ARMCC__) || defined(__ghs__)
	//! \endcond
	#	define GT_COMPILER_GCC
	//! \cond
	#if defined(__CC_ARM) || defined(__ARMCC__)
	//! \endcond
	#	define GT_COMPILER_ARMCC
	//! \cond
	#elif defined(__ghs__)
	//! \endcond
	#	define GT_COMPILER_GHS
	//! \cond
	#endif
	#elif defined(_MSC_VER)
	//! \endcond
	//! \brief Microsoft Visual C++ compiler
	#	define GT_COMPILER_MSVC
	#pragma warning(disable : 4002)	//	to many actual parameters for macro
	#pragma warning(disable : 4091) // dbghelp.h 
	#pragma warning(disable : 4668)
	#pragma warning(disable : 4710)
	#pragma warning(disable : 4777) // _snprintf %i
	#pragma warning(disable : 4477) // _snprintf %#010x
	#pragma warning(disable : 4820)
	#pragma warning(disable : 4826)	//	conversion from 'unsigned long *' to DWORD64 is sign-extended
	#pragma warning(disable : 4917)
	#include <SDKDDKVer.h>
	#include <windows.h>
	#include <shlobj.h>
	#include <shlwapi.h>
	#pragma comment(lib,"winmm.lib")
	//! \cond
	#elif defined(__MWERKS__)
	//! \endcond
	#	define GT_COMPILER_MWERKS
	#else
	#	error Компилятор не поддерживается
	#endif

	//	Архитектура
	//! \cond
	#if defined(__i386__) || defined(_M_IX86)
	//! \endcond
	#	define	GT_ARCH_IA32
	#	define	GT_ARCH_INTEL
	#	define	GT_ENDIAN_LITTLE	1
	#	define	GT_ENDIAN_BIG		0
	#	define	GT_POINTER_SIZE		4
	#	define	GT_NUM_SIMD_REGISTERS 8
	//! \cond
	#elif defined(_M_AMD64) || defined(_M_X64) || defined(__amd64) || defined(__x86_64)
	//! \endcond
	#	define	GT_ARCH_X64
	#	define	GT_ARCH_INTEL
	#	define	GT_ENDIAN_LITTLE	1
	#	define	GT_ENDIAN_BIG		0
	#	define	GT_POINTER_SIZE		8
	#	define	GT_ARCH_SUPPORTS_INT64
	#	define	GT_NUM_SIMD_REGISTERS 16
	//! \cond
	#elif defined(_PPC_) || defined(__POWERPC__) || defined(_M_PPC) || defined(_M_PPCBE) || defined(GEKKO) || defined(EPPC)
	//! \endcond
	#	define	GT_ARCH_PPC
	#	define	GT_ENDIAN_LITTLE	0
	#	define	GT_ENDIAN_BIG		1
	#	define	GT_POINTER_SIZE		4
	#	define	GT_ARCH_SUPPORTS_INT64
	#	define	GT_NUM_SIMD_REGISTERS 8
	//! \cond
	#elif defined(__PPU__) && defined(__CELLOS_LV2__)
	//! \endcond
	#	define	GT_ARCH_PS3
	#	define	GT_ENDIAN_LITTLE	0
	#	define	GT_ENDIAN_BIG		1
	#	define	GT_POINTER_SIZE		4
	#	define	GT_ARCH_SUPPORTS_INT64
	#	define	GT_NUM_SIMD_REGISTERS 32
	//! \cond
	#elif defined(__SPU__) && defined(__CELLOS_LV2__)
	//! \endcond
	#	define	GT_ARCH_PS3SPU
	#	define	GT_ENDIAN_LITTLE	0
	#	define	GT_ENDIAN_BIG		1
	#	define	GT_POINTER_SIZE		4
	#	define	GT_NUM_SIMD_REGISTERS 128
	//! \cond
	#elif defined(arm) || defined(__arm__) || defined(_M_ARM)
	//! \endcond
	#	define	GT_ARCH_ARM
	//! \cond
	#	if defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)  || ( defined(_M_ARM) && (_M_ARM >= 7) )
	//! \endcond
	#	define	GT_ARCH_ARM_V7		1
	//! \cond
	#	elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK) || defined(__ARM_ARCH_6T2__) || defined(NN_PROCESSOR_ARM) || defined(NN_PROCESSOR_ARM_V6) || ( defined(_M_ARM) && (_M_ARM < 7) )
	//! \endcond
	#	define	GT_ARCH_ARM_V6		1
	#	else
	#	error	Эта ARM архитектура не поддерживается
	#	endif
	//! \cond
	#	if defined(__ARMEB__)
	//! \endcond
	#	define	GT_ENDIAN_LITTLE	0
	#	define	GT_ENDIAN_BIG		1
	//! \cond
	#	else
	//! \endcond
	#	define	GT_ENDIAN_LITTLE	1
	#	define	GT_ENDIAN_BIG		0
	//! \cond
	#	endif
	//! \endcond
	#	define	GT_POINTER_SIZE		4
	#	define	GT_NATIVE_ALIGN_CHECK 0x7
	#	define	GT_NUM_SIMD_REGISTERS 8
	#	else
	#	error	Невозможно определить архитектуру
	#endif

	#ifndef GT_NATIVE_ALIGN_CHECK 
	#define GT_NATIVE_ALIGN_CHECK 0xf
	#endif

	//	Платформа
	//! \cond
	#if defined(__ORBIS__)
	//! \endcond
	#	define	GT_PLATFORM_PS4
	#	define	GT_PLATFORM_IS_CONSOLE	1
	//! \cond
	#elif defined(__APPLE_CC__)
	//! \endcond
	#	include <TargetConditionals.h>
	//! \cond
	#	if	defined(GT_ARCH_IA32)
	#		if  defined(TARGET_IPHONE_SIMULATOR) && TARGET_IPHONE_SIMULATOR
	//! \endcond
	#			define	GT_PLATFORM_IS_CONSOLE	1
	#			define	GT_PLATFORM_IOS
	#			define	GT_PLATFORM_IOS_SIM
	//! \cond
	#		else
	//! \endcond
	#			define	GT_PLATFORM_IS_CONSOLE	0
	#			define	GT_PLATFORM_MAC386
	//! \cond
	#		endif
	#	elif	defined(GT_ARCH_PPC)
	//! \endcond
	#			define	GT_PLATFORM_MACPPC
	#			define	GT_PLATFORM_IS_CONSOLE	0
	//! \cond
	#	elif	defined(GT_ARCH_ARM)
	//! \endcond
	#			define	GT_PLATFORM_IOS
	#			define	GT_PLATFORM_IS_CONSOLE	1
	//! \cond
	#			if	defined(__ARM_NEON__) && efined(GT_ARCH_ARM_V7) && !defined(GT_DISABLE_NEON)
	//! \endcond
	#				define	GT_COMPILER_HAS_INTRINSICS_NEON
	#			endif
	#	else
	#			error	Данная Mac платформа не поддерживается
	//! \endcond
	//! \cond
	#	endif
	#elif defined(_WIN32)
	//! \endcond
	//!	\brief this app for Windows
	#		define	GT_PLATFORM_WIN32
	//! \cond
	#		if	defined(_WIN64)
	//! \endcond
	//!	\brief this app for Windows 64bit
	#			define	GT_PLATFORM_X64
	#		endif
	//! \cond
	#ifndef WINAPI_FAMILY_APP
	//! \endcond
	#	define	WINAPI_FAMILY_APP			0x1
	#	define	WINAPI_FAMILY_DESKTOP_APP	0x2
	#	define	WINAPI_FAMILY_PHONE_APP		0x3
	#	define	___UNDEF_FAMILY				1
	//! \cond
	#endif
	#	if	defined(WINAPI_FAMILY)	&&	((WINAPI_FAMILY==WINAPI_FAMILY_APP) || (WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP))
	//! \endcond
	#		define	GT_PLATFORM_WINRT
	//! \cond
	#		if WINAPI_FAMILY == WINAPI_FAMILY_APP
	//! \endcond
	#			define	GT_PLATFORM_METRO	1
	//! \cond
	#		elif WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
	//! \endcond
	#			define	GT_PLATFORM_IS_CONSOLE	1
	#		endif
	//! \cond
	#	elif defined(_DURANGO)
	//! \endcond
	#			define	GT_PLATFORM_DURANGO
	#			define	GT_PLATFORM_IS_CONSOLE	1
	#	endif
	#	ifdef	___UNDEF_FAMILY
	#		undef	WINAPI_FAMILY_APP
	#		undef	WINAPI_FAMILY_DESKTOP_APP
	#		undef	WINAPI_FAMILY_PHONE_APP
	#		undef	___UNDEF_FAMILY
	#	endif
	//! \cond
	#	ifndef	GT_PLATFORM_IS_CONSOLE
	//! \endcond
	#		define	GT_PLATFORM_IS_CONSOLE	0
	#	endif
	//! \cond
	#elif defined(ANDROID_NDK) || defined(ANDROID) || defined(NDK) || defined(__ANDROID_API__)
	//! \endcond
	#	define	GT_PLATFORM_ANDROID
	#	define	GT_PLATFORM_IS_CONSOLE	1
	//! \cond
	#	if defined(GT_ARCH_ARM_V7)	&&	!defined(GT_DISABLE_NEON)
	//! \endcond
	#		define	GT_COMPILER_HAS_INTRINSICS_NEON	1
	#	endif
	//! \cond
	#elif defined(GT_PLATFORM_TIZEN) || defined(__tizen__) || defined(TIZEN)
	//! \endcond
	#		define	GT_PLATFORM_TIZEN
	#		define	GT_PLATFORM_IS_CONSOLE	1
	//! \cond
	#elif (defined(__unix__) || defined(__linux__))
	//! \endcond
	#	define	GT_PLATFORM_LINUX
	#	define	GT_PLATFORM_IS_CONSOLE	0
	//! \cond
	#	if defined(__native_client__) || defined(__nacl__) || defined(NACL)
	//! \endcond
	#		define	GT_PLATFORM_NACL
	#		undef	GT_POINTER_SIZE
	#		define	GT_POINTER_SIZE	4
	#	endif
	//! \cond
	#elif defined(CAFE) || defined(GT_COMPILER_GHS)
	#	include <cafe.h>
	#	include <ppc_ghs.h>
	//! \endcond
	#	define	GT_PLATFORM_WIIU
	//! \cond
	#	if !defined(GT_ENABLE_PAIRED_SINGLE_OPTS)
	//! \endcond
	#	define	GT_ENABLE_PAIRED_SINGLE_OPTS
	#	endif
	#	define	GT_VECTOR_PARTS_MUST_BE_VALID
	#	define	GT_PLATORM_IS_CONSOLE	1
	//! \cond
	#elif defined(GEKKO) || defined(__PPCGEKKO__)
	//! \endcond
	#	define	GT_PLATFORM_GC
	//! \cond
	#	if defined(RVL_OS)
	#		if !defined(GT_ENABLE_PAIRED_SINGLE_OPTS)
	//! \endcond
	#			define	GT_ENABLE_PAIRED_SINGLE_OPTS
	#		endif
	#		define	GT_PLATFORM_RVL
	#	endif
	#	define	GT_PLATFORM_IS_CONSOLE	1
	//! \cond
	#elif defined(__PPU__) && defined(__CELLOS_LV2__)
	//! \endcond
	#	define	GT_EXPENSIVE_LHS
	#	define	GT_EXPENSIVE_FLOAT_IF
	#	define	GT_PLATFORM_PS3
	#	define	GT_PLATFORM_PS3_PPU
	#	define	GT_PLATFORM_IS_CONSOLE	1
	//! \cond
	#elif defined(__SPU__) && defined(__CELLOS_LV2__)
	//! \endcond
	#	define	GT_PLATFORM_PS3
	#	define	GT_PLATFORM_PS3_SPU
	#	define	GT_PLATFORM_SPU
	#	define	GT_PLATFORM_IS_CONSOLE	1
	//! \cond
	#elif defined(__CTR__) || defined(NN_PLATFORM_CTR)
	//! \endcond
	#	define	GT_PLATFORM_CTR
	#	define	GT_PLATFORM_IS_CONSOLE	1
	//! \cond
	#elif (defined(GT_ARCH_ARM) && defined(GT_COMPILER_SNC))
	//! \endcond
	#	define	GT_PLATFORM_PSVITA	1
	#	define	GT_PLATFORM_IS_CONSOLE	1
	#	define	GT_COMPILER_HAS_INTRINSICS_NEON 1
	#else
	#endif

	//! \cond
	#if defined(GT_COMPILER_MSVC) || defined(GT_COMPILER_INTEL)
	//! \endcond
	#	define	GT_ALIGN_OF(T)	__alignof(T)
	//! \cond
	#elif defined(GT_COMPILER_GCC) || defined(GT_COMPILER_SNC) || defined(GT_COMPILER_CLANG)
	//! \endcond
	#	define	GT_ALIGN_OF(T)	__alignof__(T)
	//! \cond
	#elif defined(GT_COMPILER_MWERKS)
	//! \endcond
	#	define	GT_ALIGN_OF(T)	__builtin_align(T)
	#endif

	//! \cond
	#if defined(GT_ARCH_IA32) || defined(GT_ARCH_X64)
	#	if defined(GT_COMPILER_MSVC)
	//! \endcond
	#		define GT_BREAKPOINT(ID) __debugbreak();
	//! \cond
	#	elif defined(GT_PLATFORM_NACL)
	//! \endcond
	#		define GT_BREAKPOINT(ID) ((*((int*)0)) = ID);
	//! \cond
	#	elif defined(GT_COMPILER_GCC) || defined(GT_COMPILER_CLANG)
	//! \endcond
	#		define GT_BREAKPOINT(ID) asm("int $3")
	#	else
	//#	error
	#	endif
	//! \cond
	#elif defined(GT_PLATFORM_LRB)
	#	include <unistd.h>
	#	include <signal.h>
	//! \endcond
	#	define GT_BREAKPOINT(ID) signal(SIGSTOP, 0 );
	//! \cond
	#elif defined(GT_PLATFORM_PS3_PPU)
	//! \endcond
	#		define	GT_BREAKPOINT(ID) __asm__ volatile ( "tw 31,1,1" )
	//! \cond
	#elif defined(GT_PLATFORM_PS3_SPU)
	//! \endcond
	#	define GT_BREAKPOINT(ID) __asm__ volatile ("ilhu $0, %0\n\tiohl $0, %1\n\tstopd $0,$0,$0" : : "i"(ID>>16), "i"(ID&0xFFFF) ); 
	//! \cond
	#elif defined(GT_PLATFORM_XBOX360)
	//! \endcond
	#	define GT_BREAKPOINT(ID) __debugbreak();
	//! \cond
	#elif defined(GT_PLATFORM_WIIU)
	//! \endcond
	#	define GT_BREAKPOINT(ID) OSDebug();
	//! \cond
	#elif defined(GT_PLATFORM_GC)
	//! \endcond
	#	define GT_BREAKPOINT(ID) 
	//! \cond
	#elif defined(GT_PLATFORM_CTR)
	#	include <nn/dbg.h>
	//! \endcond
	#	define GT_BREAKPOINT(ID) ::nn::dbg::Break(nn::dbg::BREAK_REASON_ASSERT);
	//! \cond
	#elif defined(GT_ARCH_ARM)
	#	if defined(GT_COMPILER_MSVC) 
	//! \endcond
	#		define GT_BREAKPOINT(ID) __debugbreak();
	//! \cond
	#	elif defined(GT_COMPILER_SNC) 
	//! \endcond
	#		define GT_BREAKPOINT(ID) __breakpoint(0);
	//! \cond
	#	else
	//! \endcond
	#		define GT_BREAKPOINT(ID) asm("bkpt 0"); 
	#	endif
	//! \cond
	#else
	//! \endcond
	#	define GT_BREAKPOINT(ID) ((*((int*)0)) = ID);
	#endif

	//! \cond
	#if __cplusplus >= 201103
	//! \endcond
	#	define GT_OVERRIDE override
	#	define GT_FINAL final
	#	define GT_FINAL_OVERRIDE final override
	//! \cond
	#elif defined (GT_COMPILER_MSVC)
	#	if (_MSC_VER >= 1800)
	//! \endcond
	#		define GT_OVERRIDE override
	#		define GT_FINAL	final
	#		define GT_FINAL_OVERRIDE final override
	//! \cond
	#	elif (_MSC_VER >= 1600)
	//! \endcond
	#		define GT_OVERRIDE override
	#		define GT_FINAL	sealed
	#		define GT_FINAL_OVERRIDE sealed override
	//! \cond
	#	elif (_MSC_VER >=1400)
	//! \endcond
	#		define GT_OVERRIDE override
	#		define GT_FINAL
	#		define GT_FINAL_OVERRIDE override
	//! \cond
	#	else
	//! \endcond
	#		define GT_OVERRIDE 
	#		define GT_FINAL 
	#		define GT_FINAL_OVERRIDE
	//! \cond
	#	endif
	#else
	//! \endcond
	#	define GT_OVERRIDE 
	#	define GT_FINAL 
	#	define GT_FINAL_OVERRIDE
	#endif

	//! \cond
	#if defined(GT_PLATFORM_WIN32)
	//! \endcond
	//! \brief Library handle. Windows version.
	#	define GT_LIBRARY_HANDLE HMODULE
	//! \brief Load library. Windows version.
	//! \param x: library handle
	#	define GT_LOAD_LIBRARY(x) LoadLibrary(x)
	//! \brief Get procedure address rom lybrary. Windows version.
	#	define GT_LOAD_FUNCTION GetProcAddress
	//! \brief Free library. Windows version.
	#	define GT_FREE_LIBRARY FreeLibrary
	//! \cond
	#elif GT_PLATFORM_LINUX
	//! \endcond
	//! \brief Library handle. Linux version.
	#	define GT_LIBRARY_HANDLE void*
	//! \brief Load library. Linux version.
	//! \param x: library handle
	#	define GT_LOAD_LIBRARY(x) dlopen(x,(RTLD_NOW|RTLD_GLOBAL))
	//! \brief Get procedure address rom lybrary. Linux version.
	#	define GT_LOAD_FUNCTION dlsym
	//! \brief Free library. Linux version.
	#	define GT_FREE_LIBRARY dlclose
	#endif
	
	//! \brief Load function with safe pointer casting
	//! \param type: function type
	//! \param handle: library handle
	//! \param funcName: function name for load
	#define GT_LOAD_FUNCTION_SAFE_CAST(type,handle,funcName)reinterpret_cast<type>(reinterpret_cast<void*>(GT_LOAD_FUNCTION(handle,funcName)))

	//! \cond
	#ifndef LOWORD 
	//! \endcond
		//! \brief Получить младшие 2 байта из 32 битного целого
		#define LOWORD(x)((u16)(((u32)(x))&0xffff))
	//! \cond
	#endif
	#ifndef HIWORD
	//! \endcond
		//! \brief Получить старшие 2 байта из 32 битного целого
		#define HIWORD(x)((u16)((((u32)(x))>>16)&0xffff))
	//! \cond
	#endif
	//! \endcond

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