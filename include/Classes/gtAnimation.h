#pragma once
#ifndef __GT_ANIMATION_H__
#define __GT_ANIMATION_H__

namespace gost{
		
		// Класс описывающий общую информацию и действия для анимации
	class gtAnimation{

		v2u m_loopSegment;

		u32 m_frames;		// num of frames
		u32 m_currentFrame;
		f32 m_frameRate;	// frames on second

		bool m_isLoop;		
		bool m_isPlay;		
		bool m_reverse;		


	public:

		gtAnimation():
			m_loopSegment( 0u, 0u ),
			m_frames( 0u ),
			m_currentFrame( 0u ),
			m_frameRate( 60.f ),
			m_isLoop( true ),
			m_isPlay( false ),
			m_reverse( false )
		{}

			//	increase frame count
		void addFrame(){ ++m_frames; }

		void clear(){
			m_frames = 0u;
			m_currentFrame = 0u;
			m_frameRate = 60.f;
			m_isLoop = false;
			m_isPlay = false;
			m_loopSegment = v2u( 0u, 0u );
		}
		u32	getCurrentFrame() const { return m_currentFrame; }
		u32	getFrameCount() const   { return m_frames; }
		f32	getFrameRate() const    { return m_frameRate; }
		const v2u& getLoopSegment() { return m_loopSegment; }
		bool isLoop() const         { return m_isLoop; }
		bool isPlay() const         { return m_isPlay; }
		bool isReverse() const      { return m_reverse; }
		void pause()                { m_isPlay = false; }
		void play()                 { m_isPlay = true; }

		void setCurrentFrame( u32 frame ){
			m_currentFrame = frame;
			if( m_currentFrame > m_loopSegment.y )
				m_currentFrame = m_loopSegment.y;
		}
		
		void setFrameRate( f32 rate ){ m_frameRate = rate; }
		void setLoop( bool value ){ m_isLoop = value; }
		void setLoopSegment( u32 begin, u32 end ){
			m_currentFrame = begin;
			m_loopSegment.x = begin;
			m_loopSegment.y = end;
		}

		void setReverse( bool v ){ m_reverse = v; }

			//	set active next frame
		void stepFrame(){

			if( m_reverse ){
				if( m_currentFrame == 0u || m_currentFrame == m_loopSegment.x ){
					m_currentFrame = m_loopSegment.y;
					
					if( !m_isLoop )
						m_isPlay = false;

				}else --m_currentFrame;
			}else{
				++m_currentFrame;
				if( m_currentFrame > m_loopSegment.y ){
					m_currentFrame = m_loopSegment.x;

					if( !m_isLoop )
						m_isPlay = false;
				}
			}
		}

		void stop(){
			m_isPlay = false;
			m_currentFrame = 0u;
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