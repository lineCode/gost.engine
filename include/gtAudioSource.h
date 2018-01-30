/*!	GOST
	\file gtAudioSource.h
	\brief Source for audio
*/
#pragma once
#ifndef __GT_AUDIO_SOURCE_H__
#define __GT_AUDIO_SOURCE_H__ ///< include guard

namespace gost{

	struct gtAudioSourceInfo{
		gtAudioSourceInfo( void ):
			m_formatType( 0u ),
			m_channels( 0u ),
			m_sampleRate( 0u ),
			m_AvgBytesPerSec( 0u ),
			m_BlockAlign( 0u ),
			m_BitsPerSample( 0u )
		{}

		u32		m_formatType;
		u32		m_channels;/* number of channels (i.e. mono, stereo...) */
		u32		m_sampleRate;
		u32		m_AvgBytesPerSec;    /* for buffer estimation */
		u32		m_BlockAlign;        /* block size of data */
		u32		m_BitsPerSample;     /* number of bits per sample of mono data */
	};

	class gtAudioSource : public gtRefObject{
		gtAudioSourceInfo m_info;
		
		u8	*	m_data;
		u32		m_dataSize;
	public:

		gtAudioSource( void ):
			m_data( nullptr ),
			m_dataSize( 0u )
		{}

		virtual ~gtAudioSource( void ){
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

		void setRawData( u8 * data, u32 sz ){
			m_data = data;
			m_dataSize = sz;
		}

		const gtAudioSourceInfo& getInfo( void ) const {
			return m_info;
		}

		void setInfo( const gtAudioSourceInfo& info ){
			m_info = info;
		}

		u8	*	getData( void ){
			return m_data;
		}

		u32 getDataSize( void ){
			return m_dataSize;
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