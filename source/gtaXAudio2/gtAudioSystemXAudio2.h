#pragma once
#ifndef __GT_AUDIO_SYSTEM_XAUDIO2_H__
#define __GT_AUDIO_SYSTEM_XAUDIO2_H__


namespace gost{

	class gtAudioSystemXAudio2_7 : public gtAudioSystem{

		XAudioVersion			m_version;

		HMODULE					m_libXAudio;

		IXAudio2*				m_XAudio2;
		IXAudio2MasteringVoice* m_MasteringVoice;
		IUnknown*				m_ReverbEffect;
		IXAudio2SubmixVoice*	m_SubmixVoice;

		XAUDIO2FX_REVERB_I3DL2_PARAMETERS m_PRESET_PARAMS[ 30 ];
	public:

		gtAudioSystemXAudio2_7();
		~gtAudioSystemXAudio2_7();


	
		bool initialize( XAudioVersion version );


		class gtAudioObjectImpl : public gtAudioObjectCommon{
			gtAudioSource*					m_source;
			gtAudioSourceImpl*				m_sourceImpl;
			f32								m_volume;
			u32								m_sourceMax;
			u32								m_currentSourceVoice;
			u32								m_numOfPlayedSounds;
			gtAudioSystemXAudio2_7 *        m_AS;
			
			VoiceCallback					m_callback;
			
			gtArray<IXAudio2SourceVoice*>	m_sourceVoice;
			
		public:

			gtAudioObjectImpl( gtAudioSource *, gtAudioSystemXAudio2_7 * as );
			~gtAudioObjectImpl();

			void			play();
			void			pause();
			void			stop();
			void			setVolume( f32 volume );
			f32				getVolume();
			void			setLoop( bool isLoop );
			void			setAudioSource( gtAudioSource* source );
			gtAudioSource*	getAudioSource();

			void			updateBuffer();
			bool			init( u32 sp );

			void			setTime( f64 t );
		};

		class gtAudioStreamImpl : public gtAudioStream{
			bool					m_isOpen;
		//	gtFile *				m_file;
			f32						m_volume;
			StreamingVoiceContext	m_voiceContext;
			IXAudio2SourceVoice*	m_sourceVoice;

			Wave					m_wave;
			Ogg						m_ogg;

			gtAudioSourceInfo		m_info;


			IXAudio2*				m_device;

			AudioFileFormat			m_format;
		public:

			gtAudioStreamImpl( IXAudio2* d );
			~gtAudioStreamImpl();

			void			play();
			void			pause();
			void			stop();
			void			setVolume( f32 volume );
			f32				getVolume();
			void			setLoop( bool isLoop );
			void			setAudioSource( gtAudioSource* source );
			gtAudioSource*	getAudioSource();

			bool	close();
			bool	open( const gtString& fileName );
			void	setPlaybackPosition( f32 position );
			f32		getPlaybackPosition();


			bool	init( const gtString& fileName );

			bool isOgg(){ return m_format == AudioFileFormat::ogg; }
		};

		gtAudioObject*	createAudioObject( const gtString& fileName, u32 sp = 1u );
		gtAudioObject*	createAudioObject( gtAudioSource* source, u32 sp = 1u );

		gtAudioSource*	loadAudioSource( const gtString& fileName );

		bool			checkFeature( gtAudioPluginFeatures feature );
		u32				getSupportedExtensionCount();
		const s8*		getSupportedExtension( u32 id );

		gtAudioStream*	createStream( const gtString& fileName );

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