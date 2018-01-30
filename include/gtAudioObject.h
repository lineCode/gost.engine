﻿/*!	GOST
	\file gtAudioObject.h
	\brief Object with loaded audio
*/
#pragma once
#ifndef __GT_AUDIO_OBJECT_H__
#define __GT_AUDIO_OBJECT_H__ ///< include guard

#include <gtAudioSource.h>

namespace gost{

			///	Common class for all sound objects
	class gtAudioObject : public gtRefObject{
	public:

		virtual void play( void ) = 0;
		virtual bool isPlay( void ) = 0;

		virtual void pause( void ) = 0;
		virtual void stop( void ) = 0;

		virtual void setVolume( f32 volume ) = 0;
		virtual f32  getVolume( void ) = 0;

		virtual void setLoop( bool isLoop ) = 0;
		virtual bool isLoop( void ) = 0;

		virtual void setAudioSource( gtAudioSource* source ) = 0;
		virtual gtAudioSource* getAudioSource( void ) = 0;
	};

	class gtAudioObjectCommon : public gtAudioObject{
	protected:
		bool m_isPlay;
		bool m_isLoop;
	public:

		gtAudioObjectCommon( void ):
			m_isPlay( false ),
			m_isLoop( false )
		{
#ifdef GT_DEBUG
			m_debugName = "gtAudioObject";
#endif
		}

		virtual ~gtAudioObjectCommon( void ){}

		bool isPlay( void ){
			return m_isPlay;
		}


		bool isLoop( void ){
			return m_isLoop;
		}

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