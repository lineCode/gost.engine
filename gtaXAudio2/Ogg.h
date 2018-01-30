
#include <ogg\ogg.h>
#include <vorbis/vorbisfile.h>
#include <vorbis/codec.h>

struct Ogg{
	
	Ogg( void ){
	}

	~Ogg( void ){
	}

	gtAudioSourceImpl* read( const gtString& fileName );

};