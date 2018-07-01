#include "common.h"

#define STREAMING_BUFFER_SIZE 8192
#define MAX_BUFFER_COUNT 3



bool Wave::getInfo( gtAudioSourceInfo& info ){

	m_file = util::openFileForReadBinShared( m_fileName ).data();

	if( !m_file ){
		gtLogWriter::printWarning( u"Can not open file." );
		return false;
	}

	u32 bytesRead = m_file->read( (u8*)&m_header, sizeof(m_header) );
	m_file->release();
	m_file = nullptr;

	if( bytesRead != 44u ){
		gtLogWriter::printWarning( u"Can not read file. Bad header." );
		return false;
	}

	/*if( m_header.bitsPerSample > 16u ){
		gtLogWriter::printWarning( u"Unsupported bit depth." );
		return false;
	}*/

	if( m_header.audioFormat != gtConst1U ){
		gtLogWriter::printWarning( u"Unsupported wav format." );
		return false;
	}

	info.m_bytesPerSec	= m_header.byteRate;
	info.m_bitsPerSample= m_header.bitsPerSample;
	info.m_blockAlign	= m_header.blockAlign;
	info.m_channels		= m_header.numChannels;
	info.m_formatType	= m_header.audioFormat;
	info.m_sampleRate	= m_header.sampleRate;

	m_time = 0.;

	return true;
}

gtAudioSourceImpl* Wave::read( gtAudioSourceInfo info ){


	gtAudioSourceImpl* source = new gtAudioSourceImpl;
	if( !source ) return nullptr;

	source->allocate( m_header.subchunk2Size );
	if( !source->getData() ){
		gtLogWriter::printWarning( u"Can not allocate memory for audio source" );
		delete source;
		return nullptr;
	}

	m_file = util::openFileForReadBinShared( m_fileName ).data();
	if( !m_file ){
		gtLogWriter::printWarning( u"Can not open file." );
		delete source;
		return nullptr;
	}

	u32 offset = m_file->size() - m_header.subchunk2Size;

	m_file->seek( offset, gtFileSeekPos::Begin );
	m_file->read( source->getData(), source->getDataSize() );
	m_file->release();
	m_file = nullptr;


	source->setInfo( info );

	source->updateData();
	source->m_time = m_time;

	return source;
}


void	WavStreamFunc( void * arg ){

	WaveStream*	args = (WaveStream*)arg;
	
	*args->state = gtAudioState::Play;

	BYTE buffers[MAX_BUFFER_COUNT][STREAMING_BUFFER_SIZE];

	auto * mainSystem = gtMainSystem::getInstance();

	gtFile_t file = util::openFileForReadBinShared( args->filePath );
	HANDLE thread = GetCurrentThread();
	do{
		if( !mainSystem->isRun() ) break;

		file->seek( *args->currentPosition, gtFileSeekPos::Begin );

		args->sourceVoice->Start( 0, XAUDIO2_COMMIT_ALL );
		while( *args->currentPosition < args->waveLength ){

				//	Read-only operation
			if( !mainSystem->isRun() ) break;
		
			u32 reads = file->read( buffers[ args->currentDiskReadBuffer ], STREAMING_BUFFER_SIZE );
			if( !reads ) break;

			*args->currentPosition += reads;

			XAUDIO2_VOICE_STATE state;
			for(; ; ){
				args->sourceVoice->GetState( &state );
				if( state.BuffersQueued < MAX_BUFFER_COUNT - 1 )
					break;
				WaitForSingleObject( thread, 20 );
			}

			XAUDIO2_BUFFER buf = {0};
			buf.AudioBytes = reads;
			buf.pAudioData = buffers[args->currentDiskReadBuffer];
			if( *args->currentPosition >= args->waveLength )
				buf.Flags = XAUDIO2_END_OF_STREAM;

			args->sourceVoice->SubmitSourceBuffer( &buf );
		
			if( *args->command != PlayBackCommand::PBC_NONE )
				break;

			args->currentDiskReadBuffer++;
			args->currentDiskReadBuffer %= MAX_BUFFER_COUNT;
		}

		args->sourceVoice->Stop();
		args->sourceVoice->FlushSourceBuffers();

		if( *args->command == PlayBackCommand::PBC_SETPOS){
			*args->command = PlayBackCommand::PBC_NONE;
			continue;
		}

		if( *args->command == PlayBackCommand::PBC_PAUSE){
			*args->state			=	gtAudioState::Pause;
			return;
		}else if( *args->command == PlayBackCommand::PBC_STOP){
			*args->currentPosition	= 44u;
			*args->state			=	gtAudioState::Stop;
			return;
		}
		*args->currentPosition	= 44u;

	}while( *args->isLoop == 1 );
}

void Wave::setPos( f32 p ){
	f32 res = (f64)m_header.subchunk2Size * p;
	m_position = (DWORD)res;

		//	align
	if( m_position & 0x000001 ) m_position++;

	m_playBackCommand	=	PlayBackCommand::PBC_SETPOS;
}

f32 Wave::getPos(){
	return (f32)(((f64)(m_position))*1.0 / (f64)m_header.subchunk2Size);
}

bool	Wave::prepareToStreaming( 
	IXAudio2SourceVoice*	sourceVoice,
	gtAudioState			*state){
	
	m_stream.waveLength	= m_header.subchunk2Size;

	m_stream.filePath	=	m_fileName;
	m_stream.sourceVoice	= sourceVoice;
	m_stream.state		=	state;
	m_stream.command	=	&m_playBackCommand;
	m_stream.isLoop		=	&m_isLoop;
	m_stream.currentPosition	=	&m_position;


	if( !m_thread.data() )
		m_thread = gtMainSystem::getInstance()->createThread();
	m_thread->join();
	m_thread->start( (gtThread::StartFunction)WavStreamFunc, (void*)&m_stream );

	return true;
}

void	Wave::closeStream(){
	if( m_thread.data() ){
		m_thread->join();
		m_thread->release();
		m_thread = nullptr;
	}
}

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