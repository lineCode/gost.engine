#pragma once
#ifndef __GT_AUDIO_SYSTEM_H__
#define __GT_AUDIO_SYSTEM_H__

#include <gtAudioObject.h>
#include <gtAudioStream.h>

namespace gost{

	enum class gtAudioPluginFeatures : u32{
		Streaming_audio,
		Reverberation,
		Pitch,
		Audio_3D
	};

	class gtAudioSystem : public gtRefObject{
	public:

			//	Check supported features
		virtual bool	checkFeature( gtAudioPluginFeatures feature ) = 0;

			//	Load audio file and create audio object.
		virtual gtAudioObject*	createAudioObject( const gtString& fileName, u32 sp = 1u ) = 0;

			//	Create audio object from memmory
		virtual gtAudioObject*	createAudioObject( gtAudioSource* source, u32 sp = 1u ) = 0;

			//	Open audio file to play streaming audio.
		virtual gtAudioStream*	createStream( const gtString& fileName ) = 0;

		virtual const s8*		getSupportedExtension( u32 id ) = 0;
		virtual u32				getSupportedExtensionCount() = 0;

			//	Load audio file
		virtual gtAudioSource*	loadAudioSource( const gtString& fileName ) = 0;

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