#include "common.h"

gtAudioSourceImpl* Mp3::read( const gtString& fileName ){
	gtAudioSourceInfo info;


	gtAudioSourceImpl* source = new gtAudioSourceImpl;
	if( !source ) return nullptr;

//	source->allocate( sz );

//	memcpy( source->getData(), buffer.data(), sz );

	source->setInfo( info );

	source->updateData();



	return source;
}