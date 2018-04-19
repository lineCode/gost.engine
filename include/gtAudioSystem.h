/*!	GOST
	\file gtAudioSystem.h
	\brief GoST audio system
*/

#pragma once
#ifndef __GT_AUDIO_SYSTEM_H__
#define __GT_AUDIO_SYSTEM_H__ //< include guard

#include <gtAudioObject.h>
#include <gtAudioStream.h>

namespace gost{

	enum class gtAudioPluginFeatures : u32{
		streaming_audio,
		reverberation,
		pitch,
		_3D
	};

	class gtAudioSystem : public gtRefObject{
	public:

			//	Load audio file and create audio object.
			// \param fileName: audio file
			//	\param sp: Simultaneous playback.
			//	\return Audio object for playback
		virtual gtAudioObject*	createAudioObject( const gtString& fileName, u32 sp = 1u ) = 0;

			//	Create audio object from memmory
			// \param fileName: audio source
			//	\param sp: Simultaneous playback.
			//	\return Audio object for playback
			//	\remark `source` will be destroyed when `gtAudioObject` destroy. If you need `source`, call `source->addRef();`
		virtual gtAudioObject*	createAudioObject( gtAudioSource* source, u32 sp = 1u ) = 0;

			//	Load audio file
			//	\param fileName: path to the file
			//	\return Audio source
		virtual gtAudioSource*	loadAudioSource( const gtString& fileName ) = 0;

			//	Check supported features
			//	\param feature: feature to check
			//	\return \b true if supported
		virtual bool	checkFeature( gtAudioPluginFeatures feature ) = 0;

			//	Get num of supported file extensions
			//	\return Num of supported file extensions
		virtual u32		getSupportedExtensions( void ) = 0;

			//	Get supported extension
			//	\param id: number of extension. See getSupportedExtensions
			//	\return Extension
			//	\remark If `id` invalid (id>=getSupportedExtensions()), this function return `nullptr`
		virtual const s8*		getSupportedExtension( u32 id ) = 0;


			//	Open audio file to play streaming audio.
			//	\param fileName: path to the file
			//	\return Audio stream object
		virtual gtAudioStream*	createStream( const gtString& fileName ) = 0;
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