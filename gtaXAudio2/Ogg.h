
#include <ogg\ogg.h>
#include <vorbis/vorbisfile.h>
#include <vorbis/codec.h>

struct Ogg{
	
	Ogg():
		m_position( 0 )
	{
	}

	Ogg( const gtString& fileName ):
		m_position( 0 )
	{
		m_fileName = fileName;
		
	}

	~Ogg(){
		

	}

	gtAudioSourceImpl* read( gtAudioSourceInfo info );

	bool	getInfo( gtAudioSourceInfo& info );
	void	closeStream();
	bool	prepareToStreaming( IXAudio2SourceVoice*	sourceVoice,  gtAudioState *state);
	void	setPos( f32 p );
	f32		getPos();

	gtFile	  * m_file		= nullptr;
	gtString	m_fileName;
	OggStream	m_stream;
	gtPtr<gtThread>	m_thread = nullptr;
	PlayBackCommand	m_playBackCommand = PBC_NONE;
	u8			m_isLoop;
	f64			m_position;

	gtAudioSourceInfo m_info;
	f64			m_time;
};

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