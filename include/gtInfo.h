#pragma once
#ifndef __GT_INFO_H__
#define __GT_INFO_H__ //< include guard

	#define Q(x) #x
	#define QUOTE(x) Q(x)

	#define GT_MAKEFOURCC( ch0, ch1, ch2, ch3 )\
            ((u32)(u8)(ch0)|((u32)(u8)(ch1)<<8)|\
            ((u32)(u8)(ch2)<<16)|((u32)(u8)(ch3)<<24))

	#define gtPtr_t(Type,Name,exp) gtPtr<Type> Name(gtPtrNew<Type>(exp))

	#if defined(__clang__)
	#	define GT_COMPILER_CLANG
	#elif defined(__SNC__)
	#	define GT_COMPILER_SNC
	#elif defined(__GNUC__) || defined(__CC_ARM) || defined(__ARMCC__) || defined(__ghs__)
	#	define GT_COMPILER_GCC
	#if defined(__CC_ARM) || defined(__ARMCC__)
	#	define GT_COMPILER_ARMCC
	#elif defined(__ghs__)
	#	define GT_COMPILER_GHS
	#endif
	#elif defined(_MSC_VER)
	#	define GT_COMPILER_MSVC
	#include <SDKDDKVer.h>
	#include <windows.h>
	#include <shlobj.h>
	#include <shlwapi.h>
	#pragma comment(lib,"winmm.lib")
	#elif defined(__MWERKS__)
	#	define GT_COMPILER_MWERKS
	#else
	#	error Unknown compiler
	#endif

	#if defined(__i386__) || defined(_M_IX86)
	#	define	GT_ARCH_IA32
	#	define	GT_ARCH_INTEL
	#	define	GT_ENDIAN_LITTLE	1
	#	define	GT_ENDIAN_BIG		0
	#	define	GT_POINTER_SIZE		4
	#	define	GT_NUM_SIMD_REGISTERS 8
	#elif defined(_M_AMD64) || defined(_M_X64) || defined(__amd64) || defined(__x86_64)
	#	define	GT_ARCH_X64
	#	define	GT_ARCH_INTEL
	#	define	GT_ENDIAN_LITTLE	1
	#	define	GT_ENDIAN_BIG		0
	#	define	GT_POINTER_SIZE		8
	#	define	GT_ARCH_SUPPORTS_INT64
	#	define	GT_NUM_SIMD_REGISTERS 16
	#elif defined(_PPC_) || defined(__POWERPC__) || defined(_M_PPC) || defined(_M_PPCBE) || defined(GEKKO) || defined(EPPC)
	#	define	GT_ARCH_PPC
	#	define	GT_ENDIAN_LITTLE	0
	#	define	GT_ENDIAN_BIG		1
	#	define	GT_POINTER_SIZE		4
	#	define	GT_ARCH_SUPPORTS_INT64
	#	define	GT_NUM_SIMD_REGISTERS 8
	#elif defined(__PPU__) && defined(__CELLOS_LV2__)
	#	define	GT_ARCH_PS3
	#	define	GT_ENDIAN_LITTLE	0
	#	define	GT_ENDIAN_BIG		1
	#	define	GT_POINTER_SIZE		4
	#	define	GT_ARCH_SUPPORTS_INT64
	#	define	GT_NUM_SIMD_REGISTERS 32
	#elif defined(__SPU__) && defined(__CELLOS_LV2__)
	#	define	GT_ARCH_PS3SPU
	#	define	GT_ENDIAN_LITTLE	0
	#	define	GT_ENDIAN_BIG		1
	#	define	GT_POINTER_SIZE		4
	#	define	GT_NUM_SIMD_REGISTERS 128
	#elif defined(arm) || defined(__arm__) || defined(_M_ARM)
	#	define	GT_ARCH_ARM
	#	if defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)  || ( defined(_M_ARM) && (_M_ARM >= 7) )
	#	define	GT_ARCH_ARM_V7		1
	#	elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK) || defined(__ARM_ARCH_6T2__) || defined(NN_PROCESSOR_ARM) || defined(NN_PROCESSOR_ARM_V6) || ( defined(_M_ARM) && (_M_ARM < 7) )
	#	define	GT_ARCH_ARM_V6		1
	#	else
	#	error	Unknown arch
	#	endif
	#	if defined(__ARMEB__)
	#	define	GT_ENDIAN_LITTLE	0
	#	define	GT_ENDIAN_BIG		1
	#	else
	#	define	GT_ENDIAN_LITTLE	1
	#	define	GT_ENDIAN_BIG		0
	#	endif
	#	define	GT_POINTER_SIZE		4
	#	define	GT_NATIVE_ALIGN_CHECK 0x7
	#	define	GT_NUM_SIMD_REGISTERS 8
	#	else
	#	error	Unknown arch
	#endif

	#ifndef GT_NATIVE_ALIGN_CHECK
	#define GT_NATIVE_ALIGN_CHECK 0xf
	#endif

	#if defined(__ORBIS__)
	#	define	GT_PLATFORM_PS4
	#	define	GT_PLATFORM_IS_CONSOLE	1
	#elif defined(__APPLE_CC__)
	#	include <TargetConditionals.h>
	#	if	defined(GT_ARCH_IA32)
	#		if  defined(TARGET_IPHONE_SIMULATOR) && TARGET_IPHONE_SIMULATOR
	#			define	GT_PLATFORM_IS_CONSOLE	1
	#			define	GT_PLATFORM_IOS
	#			define	GT_PLATFORM_IOS_SIM
	#		else
	#			define	GT_PLATFORM_IS_CONSOLE	0
	#			define	GT_PLATFORM_MAC386
	#		endif
	#	elif	defined(GT_ARCH_PPC)
	#			define	GT_PLATFORM_MACPPC
	#			define	GT_PLATFORM_IS_CONSOLE	0
	#	elif	defined(GT_ARCH_ARM)
	#			define	GT_PLATFORM_IOS
	#			define	GT_PLATFORM_IS_CONSOLE	1
	#			if	defined(__ARM_NEON__) && efined(GT_ARCH_ARM_V7) && !defined(GT_DISABLE_NEON)
	#				define	GT_COMPILER_HAS_INTRINSICS_NEON
	#			endif
	#	else
	#			error	Unsupported platform
	#	endif
	#elif defined(_WIN32)
	#		define	GT_PLATFORM_WIN32
	#		if	defined(_WIN64)
	#			define	GT_PLATFORM_X64
	#		endif
	#ifndef WINAPI_FAMILY_APP
	#	define	WINAPI_FAMILY_APP			0x1
	#	define	WINAPI_FAMILY_DESKTOP_APP	0x2
	#	define	WINAPI_FAMILY_PHONE_APP		0x3
	#	define	___UNDEF_FAMILY				1
	#endif
	#	if	defined(WINAPI_FAMILY)	&&	((WINAPI_FAMILY==WINAPI_FAMILY_APP) || (WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP))
	#		define	GT_PLATFORM_WINRT
	#		if WINAPI_FAMILY == WINAPI_FAMILY_APP
	#			define	GT_PLATFORM_METRO	1
	#		elif WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
	#			define	GT_PLATFORM_IS_CONSOLE	1
	#		endif
	#	elif defined(_DURANGO)
	#			define	GT_PLATFORM_DURANGO
	#			define	GT_PLATFORM_IS_CONSOLE	1
	#	endif
	#	ifdef	___UNDEF_FAMILY
	#		undef	WINAPI_FAMILY_APP
	#		undef	WINAPI_FAMILY_DESKTOP_APP
	#		undef	WINAPI_FAMILY_PHONE_APP
	#		undef	___UNDEF_FAMILY
	#	endif
	#	ifndef	GT_PLATFORM_IS_CONSOLE
	#		define	GT_PLATFORM_IS_CONSOLE	0
	#	endif
	#elif defined(ANDROID_NDK) || defined(ANDROID) || defined(NDK) || defined(__ANDROID_API__)
	#	define	GT_PLATFORM_ANDROID
	#	define	GT_PLATFORM_IS_CONSOLE	1
	#	if defined(GT_ARCH_ARM_V7)	&&	!defined(GT_DISABLE_NEON)
	#		define	GT_COMPILER_HAS_INTRINSICS_NEON	1
	#	endif
	#elif defined(GT_PLATFORM_TIZEN) || defined(__tizen__) || defined(TIZEN)
	#		define	GT_PLATFORM_TIZEN
	#		define	GT_PLATFORM_IS_CONSOLE	1
	#elif (defined(__unix__) || defined(__linux__))
	#	define	GT_PLATFORM_LINUX
	#	define	GT_PLATFORM_IS_CONSOLE	0
	#include <string.h>
	#	if defined(__native_client__) || defined(__nacl__) || defined(NACL)
	#		define	GT_PLATFORM_NACL
	#		undef	GT_POINTER_SIZE
	#		define	GT_POINTER_SIZE	4
	#	endif
	#elif defined(CAFE) || defined(GT_COMPILER_GHS)
	#	include <cafe.h>
	#	include <ppc_ghs.h>
	#	define	GT_PLATFORM_WIIU
	#	if !defined(GT_ENABLE_PAIRED_SINGLE_OPTS)
	#	define	GT_ENABLE_PAIRED_SINGLE_OPTS
	#	endif
	#	define	GT_VECTOR_PARTS_MUST_BE_VALID
	#	define	GT_PLATORM_IS_CONSOLE	1
	#elif defined(GEKKO) || defined(__PPCGEKKO__)
	#	define	GT_PLATFORM_GC
	#	if defined(RVL_OS)
	#		if !defined(GT_ENABLE_PAIRED_SINGLE_OPTS)
	#			define	GT_ENABLE_PAIRED_SINGLE_OPTS
	#		endif
	#		define	GT_PLATFORM_RVL
	#	endif
	#	define	GT_PLATFORM_IS_CONSOLE	1
	#elif defined(__PPU__) && defined(__CELLOS_LV2__)
	#	define	GT_EXPENSIVE_LHS
	#	define	GT_EXPENSIVE_FLOAT_IF
	#	define	GT_PLATFORM_PS3
	#	define	GT_PLATFORM_PS3_PPU
	#	define	GT_PLATFORM_IS_CONSOLE	1
	#elif defined(__SPU__) && defined(__CELLOS_LV2__)
	#	define	GT_PLATFORM_PS3
	#	define	GT_PLATFORM_PS3_SPU
	#	define	GT_PLATFORM_SPU
	#	define	GT_PLATFORM_IS_CONSOLE	1
	#elif defined(__CTR__) || defined(NN_PLATFORM_CTR)
	#	define	GT_PLATFORM_CTR
	#	define	GT_PLATFORM_IS_CONSOLE	1
	#elif (defined(GT_ARCH_ARM) && defined(GT_COMPILER_SNC))
	#	define	GT_PLATFORM_PSVITA	1
	#	define	GT_PLATFORM_IS_CONSOLE	1
	#	define	GT_COMPILER_HAS_INTRINSICS_NEON 1
	#else
	#endif

	#if defined(GT_COMPILER_MSVC) || defined(GT_COMPILER_INTEL)
	#	define	GT_ALIGN_OF(T)	__alignof(T)
	#elif defined(GT_COMPILER_GCC) || defined(GT_COMPILER_SNC) || defined(GT_COMPILER_CLANG)
	#	define	GT_ALIGN_OF(T)	__alignof__(T)
	#elif defined(GT_COMPILER_MWERKS)
	#	define	GT_ALIGN_OF(T)	__builtin_align(T)
	#endif

	#if defined(GT_ARCH_IA32) || defined(GT_ARCH_X64)
	#	if defined(GT_COMPILER_MSVC)
	#		define GT_BREAKPOINT(ID) __debugbreak();
	#	elif defined(GT_PLATFORM_NACL)
	#		define GT_BREAKPOINT(ID) ((*((int*)0)) = ID);
	#	elif defined(GT_COMPILER_GCC) || defined(GT_COMPILER_CLANG)
	#		define GT_BREAKPOINT(ID) asm("int $3");
	#	else
	//#	error
	#	endif
	#elif defined(GT_PLATFORM_LRB)
	#	include <unistd.h>
	#	include <signal.h>
	#	define GT_BREAKPOINT(ID) signal(SIGSTOP, 0 );
	#elif defined(GT_PLATFORM_PS3_PPU)
	#		define	GT_BREAKPOINT(ID) __asm__ volatile ( "tw 31,1,1" )
	#elif defined(GT_PLATFORM_PS3_SPU)
	#	define GT_BREAKPOINT(ID) __asm__ volatile ("ilhu $0, %0\n\tiohl $0, %1\n\tstopd $0,$0,$0" : : "i"(ID>>16), "i"(ID&0xFFFF) );
	#elif defined(GT_PLATFORM_XBOX360)
	#	define GT_BREAKPOINT(ID) __debugbreak();
	#elif defined(GT_PLATFORM_WIIU)
	#	define GT_BREAKPOINT(ID) OSDebug();
	#elif defined(GT_PLATFORM_GC)
	#	define GT_BREAKPOINT(ID)
	#elif defined(GT_PLATFORM_CTR)
	#	include <nn/dbg.h>
	#	define GT_BREAKPOINT(ID) ::nn::dbg::Break(nn::dbg::BREAK_REASON_ASSERT);
	#elif defined(GT_ARCH_ARM)
	#	if defined(GT_COMPILER_MSVC)
	#		define GT_BREAKPOINT(ID) __debugbreak();
	#	elif defined(GT_COMPILER_SNC)
	#		define GT_BREAKPOINT(ID) __breakpoint(0);
	#	else
	#		define GT_BREAKPOINT(ID) asm("bkpt 0");
	#	endif
	#else
	#	define GT_BREAKPOINT(ID) ((*((int*)0)) = ID);
	#endif

	#if __cplusplus >= 201103
	#	define GT_OVERRIDE override
	#	define GT_FINAL final
	#	define GT_FINAL_OVERRIDE final override
	#elif defined (GT_COMPILER_MSVC)
	#	if (_MSC_VER >= 1800)
	#		define GT_OVERRIDE override
	#		define GT_FINAL	final
	#		define GT_FINAL_OVERRIDE final override
	#	elif (_MSC_VER >= 1600)
	#		define GT_OVERRIDE override
	#		define GT_FINAL	sealed
	#		define GT_FINAL_OVERRIDE sealed override
	#	elif (_MSC_VER >=1400)
	#		define GT_OVERRIDE override
	#		define GT_FINAL
	#		define GT_FINAL_OVERRIDE override
	#	else
	#		define GT_OVERRIDE
	#		define GT_FINAL
	#		define GT_FINAL_OVERRIDE
	#	endif
	#else
	#	define GT_OVERRIDE
	#	define GT_FINAL
	#	define GT_FINAL_OVERRIDE
	#endif

	#if defined(GT_PLATFORM_WIN32)
	#	define GT_LIBRARY_HANDLE HMODULE
	#	define GT_LOAD_LIBRARY(x) LoadLibrary(x)
	#	define GT_LOAD_FUNCTION GetProcAddress
	#	define GT_FREE_LIBRARY FreeLibrary
	#elif defined(GT_PLATFORM_LINUX)
	#	define GT_LIBRARY_HANDLE void*
	#	define GT_LOAD_LIBRARY(x) dlopen((const char*)x,(RTLD_NOW|RTLD_GLOBAL))
	#	define GT_LOAD_FUNCTION dlsym
	#	define GT_FREE_LIBRARY dlclose
    #include <dlfcn.h>
	#endif



	#if defined(GT_PLATFORM_WIN32)
	#	define GT_32
	#else
	#	define GT_64
	#endif
	#define GT_LOAD_FUNCTION_SAFE_CAST(type,handle,funcName)reinterpret_cast<type>(reinterpret_cast<void*>(GT_LOAD_FUNCTION(handle,funcName)))

	#ifndef LOWORD
		#define LOWORD(x)((u16)(((u32)(x))&0xffff))
	#endif
	#ifndef HIWORD
		#define HIWORD(x)((u16)((((u32)(x))>>16)&0xffff))
	#endif

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
