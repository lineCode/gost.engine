// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#pragma warning(disable : 4668, 4820)


#include <SDKDDKVer.h>

#include <windows.h> 
#include <mmeapi.h> 

//#pragma comment( lib, "xaudio2.lib" )
//#pragma comment( lib, "winmm.lib" )
//#pragma comment( lib, "dsound.lib" )
//#pragma comment( lib, "X3DAudio.lib" )
//#pragma comment( lib, "dxerr.lib" )
//#pragma comment( lib, "xapobase.lib" )

#include <XAudio2.h> // from DXSDK
#include <XAudio2fx.h> // from DXSDK

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

#define NUM_OF_SUPPORTED_EXTENSIONS 2u

constexpr s8* Extensions[ NUM_OF_SUPPORTED_EXTENSIONS ] = {
	"wav",
	"ogg"
};

#define _USE_MATH_DEFINES
#undef _MATH_DEFINES_DEFINED

#include "gtAudioSourceImpl.h"

enum class AudioFileFormat{
	ogg,
	wav
};

class VoiceCallback : public IXAudio2VoiceCallback{
public:
	HANDLE	hBufferEndEvent;
	u32*	p_num_of_played_sounds;

	VoiceCallback(): hBufferEndEvent( CreateEvent( NULL, FALSE, FALSE, NULL ) ){
		p_num_of_played_sounds = nullptr;
	}

	~VoiceCallback(){ CloseHandle( hBufferEndEvent ); }
		
	void STDMETHODCALLTYPE  OnStreamEnd() { 
		if( p_num_of_played_sounds )
			*p_num_of_played_sounds -= 1;
	}

	void STDMETHODCALLTYPE OnVoiceProcessingPassEnd() { }
	void STDMETHODCALLTYPE OnVoiceProcessingPassStart(UINT32 SamplesRequired) {    }
	void STDMETHODCALLTYPE OnBufferEnd(void * pBufferContext)    { }
	void STDMETHODCALLTYPE OnBufferStart(void * pBufferContext) {    }
	void STDMETHODCALLTYPE OnLoopEnd(void * pBufferContext) {    }
	void STDMETHODCALLTYPE OnVoiceError(void * pBufferContext, HRESULT Error) { }
};

struct StreamingVoiceContext : public IXAudio2VoiceCallback{
			STDMETHOD_( void, OnVoiceProcessingPassStart )( UINT32 ){}
			STDMETHOD_( void, OnVoiceProcessingPassEnd )(){}
			STDMETHOD_( void, OnStreamEnd )(){}
			STDMETHOD_( void, OnBufferStart )( void* ){}
			STDMETHOD_( void, OnBufferEnd )( void* ){
	//			SetEvent( hBufferEndEvent );
			}
			STDMETHOD_( void, OnLoopEnd )( void* ){}
			STDMETHOD_( void, OnVoiceError )( void*, HRESULT ){}

	//		HANDLE hBufferEndEvent;

	//		StreamingVoiceContext() : hBufferEndEvent( CreateEvent( NULL, FALSE, FALSE, NULL ) ){}

//	virtual ~StreamingVoiceContext(){
	//	if( hBufferEndEvent )
	//		CloseHandle( hBufferEndEvent );
//	}
};

enum PlayBackCommand{
	PBC_NONE,
	PBC_STOP,
	PBC_PAUSE,
	PBC_SETPOS
};

struct OggStream{
	u8		*	isLoop;
	gtAudioState	*state;
	gtString	filePath;
	IXAudio2SourceVoice*	sourceVoice;
	PlayBackCommand* command;
	f64*		currentPosition;
};

struct WaveStream{
	u8		*	isLoop;
	DWORD		currentDiskReadBuffer = 0;
	DWORD*		currentPosition;
	DWORD		waveLength;
	gtAudioState	*state;
	gtString	filePath;
	IXAudio2SourceVoice*	sourceVoice;
	PlayBackCommand* command;
};

#include "wave.h"
#include "Ogg.h"
#include "Mp3.h"

#include "gtAudioSystemXAudio2.h"

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