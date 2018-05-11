#pragma once
#ifndef __GT_AUDIO_STREAM_H__
#define __GT_AUDIO_STREAM_H__ 

namespace gost{

	class gtAudioStream : public gtAudioObjectCommon{
	public:

			//	Close stream
			//	\return \b true if done
		virtual bool	close() = 0;

			// Open file for streaming
			//	\param fileName: target file
			// \return \b true if done
		virtual bool	open( const gtString& fileName ) = 0;

			//	setPlaybackPosition
			//	\param position: Value from 0 to 1
		virtual void	setPlaybackPosition( f32 position ) = 0;

			//	Get playback position
			//	\return Value from 0 to 1 (begin-end)
		virtual f32		getPlaybackPosition() = 0;
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