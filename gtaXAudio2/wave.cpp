#include "common.h"

gtAudioSourceImpl* Wave::read( const gtString& fileName ){

	gtAudioSourceImpl* source = new gtAudioSourceImpl;

	gtAudioSourceInfo info;

	if( !source ) return nullptr;

	HRESULT hr;
	m_dwFlags = 1;

	m_hmmio = mmioOpen( (wchar_t*)fileName.c_str(), NULL, MMIO_ALLOCBUF | MMIO_READ );
	if( NULL == m_hmmio ){
		gtLogWriter::printWarning( u"Can not open file for read" );
		delete source;
		return nullptr;
	}

		/// chunk info. for general use.
	MMCKINFO ckIn;
	memset( &ckIn, 0, sizeof(ckIn) );

		/// Temp PCM structure to load in.
	PCMWAVEFORMAT pcmWaveFormat;

	if( ( 0 != mmioDescend( m_hmmio, &m_ckRiff, NULL, 0 ) ) ){
		gtLogWriter::printWarning( u"mmioDescend == 0" );
		delete source;
		return nullptr;
	}

		/// Check to make sure this is a valid wave file
	if( ( m_ckRiff.ckid != FOURCC_RIFF ) ||
		( m_ckRiff.fccType != mmioFOURCC( 'W', 'A', 'V', 'E' ) ) ){
		gtLogWriter::printWarning( u"Bad magic" );
		delete source;
		return nullptr;
	}

		/// Search the input file for for the 'fmt ' chunk.
	ckIn.ckid = mmioFOURCC( 'f', 'm', 't', ' ' );
	if( 0 != mmioDescend( m_hmmio, &ckIn, &m_ckRiff, MMIO_FINDCHUNK ) ){
		gtLogWriter::printWarning( u"Can not Search the input file for for the 'fmt ' chunk" );
		delete source;
		return nullptr;
	}

		/// Expect the 'fmt' chunk to be at least as large as <PCMWAVEFORMAT>;
		/// if there are extra parameters at the end, we'll ignore them
	if( ckIn.cksize < ( LONG )sizeof( PCMWAVEFORMAT ) ){
		gtLogWriter::printWarning( u"'fmt' chunk large as <PCMWAVEFORMAT>" );
		delete source;
		return nullptr;
	}

		/// Read the 'fmt ' chunk into <pcmWaveFormat>.
	if( mmioRead( m_hmmio, ( HPSTR )&pcmWaveFormat,
				  sizeof( pcmWaveFormat ) ) != sizeof( pcmWaveFormat ) ){
		gtLogWriter::printWarning( u"Can not read the 'fmt ' chunk into <pcmWaveFormat>" );
		delete source;
		return nullptr;
	}

		/// Allocate the waveformatex, but if its not pcm format, read the next
		/// word, and thats how many extra bytes to allocate.
	if( pcmWaveFormat.wf.wFormatTag == WAVE_FORMAT_PCM ){
		/*m_pwfx = ( WAVEFORMATEX* )new CHAR[ sizeof( WAVEFORMATEX ) ];
		if( NULL == m_pwfx ){
			gtLogWriter::printWarning( u"Can not allocate memmory for WAVEFORMATEX" );
			return nullptr;
		}*/

			/// Copy the bytes from the pcm structure to the waveformatex structure
		//memcpy( m_pwfx, &pcmWaveFormat, sizeof( pcmWaveFormat ) );
		//m_pwfx->cbSize = 0;

		info.m_AvgBytesPerSec	= pcmWaveFormat.wf.nAvgBytesPerSec;
		info.m_BitsPerSample	= pcmWaveFormat.wBitsPerSample;
		info.m_BlockAlign		= pcmWaveFormat.wf.nBlockAlign;
		info.m_channels			= pcmWaveFormat.wf.nChannels;
		info.m_formatType		= pcmWaveFormat.wf.wFormatTag;
		info.m_sampleRate		= pcmWaveFormat.wf.nSamplesPerSec;

	}else{
			/// Read in length of extra bytes.
		WORD cbExtraBytes = 0L;
		if( mmioRead( m_hmmio, ( CHAR* )&cbExtraBytes, sizeof( WORD ) ) != sizeof( WORD ) ){
			gtLogWriter::printWarning( u"Can not read length of extra bytes." );
			delete source;
			return nullptr;
		}
			

		/*m_pwfx = ( WAVEFORMATEX* )new CHAR[ sizeof( WAVEFORMATEX ) + cbExtraBytes ];
		if( NULL == m_pwfx ){
			gtLogWriter::printWarning( u"Can not allocate memmory for WAVEFORMATEX" );
			return nullptr;
		}*/

			/// Copy the bytes from the pcm structure to the waveformatex structure
		//memcpy( m_pwfx, &pcmWaveFormat, sizeof( pcmWaveFormat ) );
		//m_pwfx->cbSize = cbExtraBytes;
		info.m_AvgBytesPerSec	= pcmWaveFormat.wf.nAvgBytesPerSec;
		info.m_BitsPerSample	= pcmWaveFormat.wBitsPerSample;
		info.m_BlockAlign		= pcmWaveFormat.wf.nBlockAlign;
		info.m_channels			= pcmWaveFormat.wf.nChannels;
		info.m_formatType		= pcmWaveFormat.wf.wFormatTag;
		info.m_sampleRate		= pcmWaveFormat.wf.nSamplesPerSec;

			/// Now, read those extra bytes into the structure, if cbExtraAlloc != 0.
		u32 xb = sizeof(WAVEFORMATEX) + cbExtraBytes;
		if( mmioRead( m_hmmio, ( CHAR* )( ( ( BYTE* )&( xb ) ) + sizeof( WORD ) ),
						cbExtraBytes ) != cbExtraBytes ){
			gtLogWriter::printWarning( u"Can not read extra bytes" );
			delete source;
			return nullptr;
		}

			/// Ascend the input file out of the 'fmt ' chunk.
		if( 0 != mmioAscend( m_hmmio, &ckIn, 0 ) ){
			gtLogWriter::printWarning( u"Can not ascend the input file out of the 'fmt ' chunk." );
			delete source;
			return nullptr;
		}
	}

		/// Seek to the data
	if( -1 == mmioSeek( m_hmmio, m_ckRiff.dwDataOffset + sizeof( FOURCC ),
						SEEK_SET ) ){
		gtLogWriter::printWarning( u"Can not seek the data" );
		delete source;
		return nullptr;
	}

		/// Search the input file for the 'data' chunk.
	m_ck.ckid = mmioFOURCC( 'd', 'a', 't', 'a' );
	if( 0 != mmioDescend( m_hmmio, &m_ck, &m_ckRiff, MMIO_FINDCHUNK ) ){
		gtLogWriter::printWarning( u"Can not seek the data" );
		delete source;
		return nullptr;
	}

	source->allocate( m_ck.cksize );

	if( !source->getData() ){
		gtLogWriter::printWarning( u"Can not allocate memmory for audio source" );
		delete source;
		return nullptr;
	}


	///	READ

		/// current status of m_hmmio
	MMIOINFO mmioinfoIn;

	if( 0 != mmioGetInfo( m_hmmio, &mmioinfoIn, 0 ) ){
		gtLogWriter::printWarning( u"Can not get MMIOINFO" );
		delete source;
		return nullptr;
	}

	for( DWORD cT = 0; cT < m_ck.cksize; cT++ ){
		// Copy the bytes from the io to the buffer.
		if( mmioinfoIn.pchNext == mmioinfoIn.pchEndRead ){
			if( 0 != mmioAdvance( m_hmmio, &mmioinfoIn, MMIO_READ ) ){
				gtLogWriter::printWarning( u"mmioAdvance fail" );
				delete source;
				return nullptr;
			}

			if( mmioinfoIn.pchNext == mmioinfoIn.pchEndRead ){
				gtLogWriter::printWarning( u"mmioinfoIn.pchNext" );
				delete source;
				return nullptr;
			}
		}

			/// Actual copy.
		*( ( BYTE* )source->getData() + cT ) = *( ( BYTE* )mmioinfoIn.pchNext );
		mmioinfoIn.pchNext++;
	}

	if( 0 != mmioSetInfo( m_hmmio, &mmioinfoIn, 0 ) ){
		gtLogWriter::printWarning( u"mmioSetInfo fail" );
		delete source;
		return nullptr;
	}

	source->setInfo( info );

	source->updateData();

	return source;
}