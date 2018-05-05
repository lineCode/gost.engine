#pragma once
#ifndef __GT_AUDIO_OBJECT_H__
#define __GT_AUDIO_OBJECT_H__

#include <gtAudioSource.h>

namespace gost{

	enum class gtAudioState : u32{
		play,
		pause,
		stop
	};

			//	Common class for all sound objects
	class gtAudioObject : public gtRefObject{
	public:

			//	Start playback
		virtual void play( void ) = 0;

			//	Check is play
			//	\return \b true if play
		virtual bool isPlay( void ) = 0;

			//	Pause playback
		virtual void pause( void ) = 0;

			//	Stop playback
		virtual void stop( void ) = 0;

			//	Set volume
			//	\param volume: new volume
		virtual void setVolume( f32 volume ) = 0;

			//	Get volume
			//	\return volume
		virtual f32  getVolume( void ) = 0;

			//	Set repeat
			//	\param isLoop: \b true if repeat
		virtual void setLoop( bool isLoop ) = 0;

			//	Check is repeat
			//	\return \b true if repeat
		virtual bool isLoop( void ) = 0;

			//	Set source
		virtual void setAudioSource( gtAudioSource* source ) = 0;

			//	Get source
			//	\return Source object
		virtual gtAudioSource* getAudioSource( void ) = 0;

			//	Get name
			//	\return Name
		virtual const gtStringA&	getName( void ) = 0;

			//	Set name
			//	\param name: new name
		virtual void setName( const gtStringA& name ) = 0;
	};

		//	Common class for audio object
	class gtAudioObjectCommon : public gtAudioObject{
	protected:
		bool			m_isLoop;
		gtAudioState	m_state;
		gtStringA		m_name;

		f64				m_time;

	public:

		gtAudioObjectCommon( void ):
			m_isLoop( false ),
			m_state( gtAudioState::stop ),
			m_time( 0. )
		{
		}

		virtual ~gtAudioObjectCommon( void ){}

		bool isPlay( void ){
			return m_state == gtAudioState::play;
		}

		bool isPause( void ){
			return m_state == gtAudioState::pause;
		}

		bool isStop( void ){
			return m_state == gtAudioState::stop;
		}

		bool isLoop( void ){
			return m_isLoop;
		}

		const gtStringA&	getName( void ){
			return m_name;
		}

		void setName( const gtStringA& name ){
			m_name = name;
		}

		f64 time( void )  { return m_time; }
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