#pragma once
#ifndef __GT_AUDIO_SYSTEM_XAUDIO2_H__
#define __GT_AUDIO_SYSTEM_XAUDIO2_H__


namespace gost{

	class gtAudioSystemXAudio2 : public gtAudioSystem{
	public:

		gtAudioSystemXAudio2( void );
		~gtAudioSystemXAudio2( void );


	
		bool initialize( void );

	};

}

#endif

