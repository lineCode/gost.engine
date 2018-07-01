#pragma once
#ifndef __GT_AUDIO_SOURCE_H__
#define __GT_AUDIO_SOURCE_H__

namespace gost{

		//	Information about audio
	struct gtAudioSourceInfo{
		gtAudioSourceInfo():
			m_formatType( gtConst0U ),
			m_channels( gtConst0U ),
			m_sampleRate( gtConst0U ),
			m_bytesPerSec( gtConst0U ),
			m_blockAlign( gtConst0U ),
			m_bitsPerSample( gtConst0U )
		{}

		u32		m_formatType;	//< for XAudio2 - 1 - PCM
		u32		m_channels;		//< number of channels (i.e. mono, stereo...)
		u32		m_sampleRate;	//< 44100 et.c.
		u32		m_bytesPerSec;	//<
		u32		m_blockAlign;	//<
		u32		m_bitsPerSample;//< 16bits
	};

		//	Audio source
	class gtAudioSource : public gtRefObject{

		gtAudioSourceInfo m_info;

			//	Raw data
		u8	*	m_data;
		u32		m_dataSize;

	public:

		gtAudioSource():
			m_data( nullptr ),
			m_dataSize( gtConst0U )
		{}

		virtual ~gtAudioSource(){
			if( m_data ){
				delete []m_data;
			}
		}

		void allocate( u32 size ){
			if( !m_data ){
				m_data = new u8[ size ];
				m_dataSize = size;
			}
		}

		u8* getData(){ return m_data; }
		u32 getDataSize(){ return m_dataSize;}

		const gtAudioSourceInfo& getInfo() const { return m_info; }
		void setInfo( const gtAudioSourceInfo& info ){ m_info = info; }
		
		void setData( u8 * data, u32 sz ){
			m_data = data;
			m_dataSize = sz;
		}

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