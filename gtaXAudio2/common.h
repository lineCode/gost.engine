// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once



#include <SDKDDKVer.h>

#include <windows.h> 
#include <mmeapi.h> 

//#pragma comment( lib, "xaudio2.lib" )
//#pragma comment( lib, "winmm.lib" )
//#pragma comment( lib, "dsound.lib" )
//#pragma comment( lib, "X3DAudio.lib" )
//#pragma comment( lib, "dxerr.lib" )
//#pragma comment( lib, "xapobase.lib" )

#include <XAudio2.h> /// from DXSDK
#include <XAudio2fx.h> /// from DXSDK

#include <gost.h>

using namespace gost;

#define INPUTCHANNELS 1
#define OUTPUTCHANNELS 8
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
#endif   

enum class XAudioVersion : u32{
	XAudio_2_9,
	XAudio_2_8,
	XAudio_2_7,
	XAudio_2_6,
	XAudio_2_5,
	XAudio_2_4,
	XAudio_2_3,
	XAudio_2_2,
	XAudio_2_1,
	XAudio_2_0,
	none
};

#define _USE_MATH_DEFINES
#undef _MATH_DEFINES_DEFINED

#include "gtAudioSourceImpl.h"

#include "wave.h"
#include "Ogg.h"
#include "Mp3.h"

#include "gtAudioSystemXAudio2.h"