#pragma once
#ifndef __GT_AUDIO_OBJECT_H__
#define __GT_AUDIO_OBJECT_H__

#include "AudioSystem/gtAudioSource.h"

namespace gost{

		// Состояния аудио объекта
	enum class gtAudioState : u32{
		Play,   // Когда звук играет
		Pause,  // Когда воспроизведение приостановлено
		Stop    // Когда звук не воспроизводился или остановлен
	};

			//	Common class for all sound objects
	class gtAudioObject : public gtRefObject{
	public:
		
			// Получить указатель на звук который объект воспроизводит
		virtual gtAudioSource* getAudioSource() = 0;
		
			// Получить имя. Имя устанавливается методом setName
		virtual const gtStringA& getName() = 0;
		
			// Получить текущую громкость этого звукового объекта
		virtual f32  getVolume() = 0;
		
			// Вернёт истину если объект повторно запускает воспроизведение звука
		virtual bool isLoop() = 0;
		
			// Вернёт истину если звук воспроизводится
		virtual bool isPlay() = 0;
		
			// Приостановить воспроизведение
		virtual void pause() = 0;
		
			// Начать воспроизведение
		virtual void play() = 0;
		
			// Установить источник звука
		virtual void setAudioSource( gtAudioSource* source ) = 0;
		
			// Установить повторяющееся воспроизведение
		virtual void setLoop( bool isLoop ) = 0;
		
			// Установить имя
		virtual void setName( const gtStringA& name ) = 0;
		
			// Установить громкость
		virtual void setVolume( f32 volume ) = 0;
		
			// Остановить воспроизведение
		virtual void stop() = 0;

	};

		//	Common class for audio object
	class gtAudioObjectCommon : public gtAudioObject{
	protected:
		bool			m_isLoop;
		gtAudioState	m_state;
		gtStringA		m_name;
		f64				m_time;
	public:

		gtAudioObjectCommon():
			m_isLoop( false ),
			m_state( gtAudioState::Stop ),
			m_time( 0. )
		{}

		virtual ~gtAudioObjectCommon(){}

		bool isPlay() { return m_state == gtAudioState::Play; }
		bool isPause(){ return m_state == gtAudioState::Pause; }
		bool isStop() { return m_state == gtAudioState::Stop; }
		bool isLoop() { return m_isLoop; }
		const gtStringA& getName(){ return m_name; }
		void setName( const gtStringA& name ){ m_name = name; }
		f64 time()    { return m_time; }
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