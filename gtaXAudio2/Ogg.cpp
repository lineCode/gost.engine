#include "common.h"

#define BUFFER_SIZE   32768

gtAudioSourceImpl* Ogg::read( const gtString& fileName ){
	gtAudioSourceInfo info;

	//gtFile_t file = util::openFileForReadBin( fileName );
	gtStringA str;
	str += fileName.data();

	FILE * f = fopen( str.data(), "rb" );

	int endian = 0;
	int bitStream;
	long bytes;
	char array[BUFFER_SIZE];
	vorbis_info *pInfo;
	OggVorbis_File oggFile;

	ov_open( f, &oggFile, NULL, 0);

	pInfo = ov_info(&oggFile, -1);
	
	info.m_formatType		= 1;
	info.m_channels			= pInfo->channels;
	info.m_sampleRate		= pInfo->rate;
	info.m_AvgBytesPerSec	= pInfo->bitrate_nominal;
	info.m_BlockAlign		= 4;
	info.m_BitsPerSample	= 16;
	
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



	return source;
}