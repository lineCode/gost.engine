#include "common.h"

gtAudioSourceImpl::gtAudioSourceImpl( void ):
	m_time( 0. )
{
	ZeroMemory( &m_xaudio2Buffer, sizeof(m_xaudio2Buffer));
	m_xaudio2Buffer.Flags = XAUDIO2_END_OF_STREAM;
}

gtAudioSourceImpl::~gtAudioSourceImpl( void ){

}

void gtAudioSourceImpl::updateData( void ){
	m_xaudio2Buffer.pAudioData = getData();
	m_xaudio2Buffer.AudioBytes = getDataSize();
}
