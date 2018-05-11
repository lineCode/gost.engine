
struct WaveHeader{
	u32 chunkId;		//"RIFF"
	u32 chunkSize;		//file size - 8bytes
	u32 format;			//"WAVE"-be (0x57415645)
	u32 subchunk1Id;	//"fmt"-be (0x666d7420)
	u32 subchunk1Size;	// 16 - PCM
	u16	audioFormat;	// 1  - PCM
	u16 numChannels;	// 1 - mono, 2 - stereo
	u32 sampleRate;		// 44100
	u32 byteRate;		// bytes in sec
	u16 blockAlign;		//
	u16 bitsPerSample;	// 16 bit
	u32 subchunk2Id;	//"data"-be (0x64617461)
	u32 subchunk2Size;	//data size
};


struct Wave{
	

	Wave(){}

	Wave( const gtString& fileName ){
		m_fileName = fileName;
	}

	~Wave(){
		if( m_file )
			m_file->release();
	}

	gtAudioSourceImpl* read( gtAudioSourceInfo info );

	bool	getInfo( gtAudioSourceInfo& info );
	void	closeStream();
	bool	prepareToStreaming( IXAudio2SourceVoice*	sourceVoice,  gtAudioState *state);
	void	setPos( f32 p );
	f32		getPos();

	gtFile	  * m_file		= nullptr;
	WaveHeader	m_header;
	gtString	m_fileName;
	WaveStream	m_stream;
	gtPtr<gtThread>	m_thread = nullptr;
	PlayBackCommand	m_playBackCommand = PBC_NONE;
	u8			m_isLoop;
	DWORD		m_position = 44;
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