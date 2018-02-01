#include "common.h"

#define BUFFER_SIZE   32768
#define STREAMING_BUFFER_SIZE 8192
#define MAX_BUFFER_COUNT 5

bool Ogg::getInfo( gtAudioSourceInfo& info ){
	gtStringA str;
	str += m_fileName.data();

	FILE * f = fopen( str.data(), "rb" );

	if( !f ){
		gtLogWriter::printWarning( u"Can not open file." );
		return false;
	}

	vorbis_info *pInfo;
	OggVorbis_File oggFile;

	ov_open( f, &oggFile, NULL, 0);

	pInfo = ov_info(&oggFile, -1);
	
	info.m_formatType		= 1;
	info.m_channels			= pInfo->channels;
	info.m_sampleRate		= pInfo->rate;
	info.m_bytesPerSec		= pInfo->bitrate_nominal;
	info.m_blockAlign		= 4;
	info.m_bitsPerSample	= 16;

	m_time = ov_time_total( &oggFile, -1 );

	fclose( f );
	ov_clear(&oggFile);

	m_info = info;

	return true;
}

gtAudioSourceImpl* Ogg::read( gtAudioSourceInfo info ){

	gtStringA str;
	str += m_fileName.data();

	FILE * f = fopen( str.data(), "rb" );
	
	if( !f ){
		gtLogWriter::printWarning( u"Can not open file." );
		return false;
	}

	int endian = 0;
	int bitStream;
	long bytes;
	char array[BUFFER_SIZE];
	OggVorbis_File oggFile;

	ov_open( f, &oggFile, NULL, 0);
		
	std::vector<u8> buffer;

	do{
		bytes = ov_read(&oggFile, array, BUFFER_SIZE, endian, 2, 1, &bitStream);
	
		buffer.insert(buffer.end(), array, array + bytes);

	}while( bytes > 0 );

	auto sz = buffer.size();

	fclose( f );

	ov_clear(&oggFile);

	gtAudioSourceImpl* source = new gtAudioSourceImpl;
	if( !source ) return nullptr;

	source->allocate( sz );

	memcpy( source->getData(), buffer.data(), sz );

	source->setInfo( info );

	source->updateData();
	source->m_time = m_time;

	return source;
}

size_t	readOgg( void *ptr, size_t size, size_t nmemb, void *datasource ){

	gtFile * file = (gtFile*)datasource;

	auto bytes = file->read( (u8*)ptr, size * nmemb );

	return bytes;
}

int seekOgg( void *datasource, ogg_int64_t offset, int whence ){
	gtFile * file = (gtFile*)datasource;

	gtFile::SeekPos pos;
	switch( whence ){
		case SEEK_CUR:
		pos = gtFile::SeekPos::ESP_CURRENT;
		break;
		case SEEK_SET:
		pos = gtFile::SeekPos::ESP_BEGIN;
		break;
		case SEEK_END:
		pos = gtFile::SeekPos::ESP_END;
		break;
	}

	file->seek( offset, pos );
	DWORD newpos = (DWORD)file->tell();

	if( newpos == INVALID_SET_FILE_POINTER ){
		DWORD error = GetLastError();
		return -1;
	}

	return newpos;
}

int closeOgg( void *datasource ){
	gtFile * file = (gtFile*)datasource;

	return 0;
}

long tellOgg( void *datasource ){
	gtFile * file = (gtFile*)datasource;
	DWORD newpos = (DWORD)file->tell();

	if( newpos == INVALID_SET_FILE_POINTER ){
		DWORD error = GetLastError();
		return -1;
	}
	return newpos;
}

void	OggStreamFunc( void * arg ){

	OggStream*	args = (OggStream*)arg;
	
	*args->state = gtAudioState::play;
	auto * mainSystem = gtMainSystem::getInstance();

	u8 currentDiskReadBuffer = 0u;

	gtFile_t file = util::openFileForReadBinShared( args->filePath );
	
	ov_callbacks callbacks;

	callbacks.read_func	=	readOgg;
	callbacks.seek_func	=	seekOgg;
	callbacks.tell_func	=	tellOgg;
	callbacks.close_func=	closeOgg;

	gtStringA str;
	str += args->filePath.data();

	int endian = 0;
	int bitStream;
	long bytes;
		
	char buffers[MAX_BUFFER_COUNT][STREAMING_BUFFER_SIZE];

	OggVorbis_File oggFile;


	if( ov_open_callbacks( file.data(), &oggFile, NULL, 0, callbacks ) < 0){
		gtLogWriter::printWarning( u"Can not open file for stream." );
		return;
	}

	do{
		

		if( !mainSystem->isRun() ) break;

		args->sourceVoice->Start( 0, XAUDIO2_COMMIT_ALL );

		ov_time_seek( &oggFile, *args->currentPosition );

		do{

			if( !mainSystem->isRun() ) break;
			
			bytes = ov_read(&oggFile, buffers[ currentDiskReadBuffer ], STREAMING_BUFFER_SIZE, endian, 2, 1, &bitStream);
	
			if( !bytes )
				break;

			XAUDIO2_VOICE_STATE state;
			for(; ; ){
				args->sourceVoice->GetState( &state );
				if( state.BuffersQueued < MAX_BUFFER_COUNT - 1 )
					break;
			}


			XAUDIO2_BUFFER buf = {0};
			buf.AudioBytes = bytes;
			buf.pAudioData = (BYTE*)buffers[currentDiskReadBuffer];
			//if( *args->currentPosition >= args->waveLength )
//				buf.Flags = XAUDIO2_END_OF_STREAM;

			args->sourceVoice->SubmitSourceBuffer( &buf );

			if( *args->command != PlayBackCommand::PBC_NONE )
				break;
			*args->currentPosition = ov_time_tell( &oggFile );

			currentDiskReadBuffer++;
			currentDiskReadBuffer %= MAX_BUFFER_COUNT;

		}while( bytes > 0 );

		args->sourceVoice->Stop();
		args->sourceVoice->FlushSourceBuffers();

		if( *args->command == PlayBackCommand::PBC_SETPOS){
			*args->command = PlayBackCommand::PBC_NONE;
			continue;
		}


		if( *args->command == PlayBackCommand::PBC_PAUSE){
			*args->state			=	gtAudioState::pause;
			ov_clear(&oggFile);
			return;
		}else if( *args->command == PlayBackCommand::PBC_STOP){
			*args->currentPosition	= 0;
			*args->state			=	gtAudioState::stop;
			ov_clear(&oggFile);
			return;
		}
		*args->currentPosition	= 0;

	}while( *args->isLoop == 1 );
}



bool	Ogg::prepareToStreaming( 
	IXAudio2SourceVoice*	sourceVoice,
	gtAudioState			*state){
	
	m_stream.filePath	=	m_fileName;
	m_stream.sourceVoice=	sourceVoice;
	m_stream.state		=	state;
	m_stream.command	=	&m_playBackCommand;
	m_stream.isLoop		=	&m_isLoop;
	m_stream.currentPosition	=	&m_position;


	if( !m_thread )
		m_thread = gtMainSystem::getInstance()->createThread();
	m_thread->join();
	m_thread->start( (gtThread::StartFunction)OggStreamFunc, (void*)&m_stream );

	return true;
}

void	Ogg::closeStream( void ){
	m_playBackCommand = PlayBackCommand::PBC_STOP;
	if( m_thread ){
		m_thread->join();
		m_thread->release();
		m_thread = nullptr;
	}
}

void	Ogg::setPos( f32 p ){
	m_position = p * m_time;
	m_playBackCommand	=	PlayBackCommand::PBC_SETPOS;
}

f32		Ogg::getPos( void ){
	return m_position * (1.0 / m_time);
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