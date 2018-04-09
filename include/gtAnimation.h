/*!	GOST
	\file gtAnimation.h
	\brief animation class

	Предпологается что класс можно использовать как для спрайтовой анимации, так и для прочих.
	Вполне можно было сделать его наследуемым классом.
	Но я не уверен что система в принципе окажется подобной как для спрайтовой так и для скелетной анимаций (нет опыта во второй).
	По этому класс обычный, и в спрайте находится его объект.
*/

#pragma once
#ifndef __GT_ANIMATION_H__
#define __GT_ANIMATION_H__ //< include guard

namespace gost{
		
		//	Описывает базовые функции для анимации
	class gtAnimation{

		v2u m_loopSegment;	//< loop segment

		u32 m_frames;		//< num of frames
		u32 m_currentFrame; //< current frame id
		f32 m_frameRate;	//< frames on second

		bool m_isLoop;		//< is loop
		bool m_isPlay;		//< is play
		bool m_reverse;		//< is reverse play


	public:

			//	c-tor
		gtAnimation( void ):
			m_loopSegment(0u,0u),
			m_frames( 0u ),
			m_currentFrame( 0u ),
			m_frameRate( 60.f ),
			m_isLoop( false ),
			m_isPlay( false ),
			m_reverse( false )
		{}

			//	is reverse play
			// \return \b true if reverse play
		bool isReverse( void ) const {
			return m_reverse;
		}

			//	set reverse play
			//	\param v: \b true if reverse play
		void setReverse( bool v ){
			m_reverse = v;
		}

			//	increase frame count
		void addFrame( void ){
			++m_frames;
		}

			//	set active next frame
		void stepFrame( void ){

			if( m_reverse ){
				if( m_currentFrame == 0u || m_currentFrame == m_loopSegment.x ){
					m_currentFrame = m_loopSegment.y;
				}else --m_currentFrame;
			}else{
				++m_currentFrame;
				if( m_currentFrame > m_loopSegment.y ){
					m_currentFrame = m_loopSegment.x;
				}
			}
		}

			//	get count of frames
			// \return count
		u32	getFrameCount( void ) const {
			return m_frames;
		}

			// get current frame id
			//	\return current frame
		u32	getCurrentFrame( void ) const {
			return m_currentFrame;
		}

			//	set current frame id
			// \param frame: id
		void setCurrentFrame( u32 frame ){
			m_currentFrame = frame;
			if( m_currentFrame > m_loopSegment.y )
				m_currentFrame = m_loopSegment.y;
		}

			// get frame rate
			// \return frame rate
		f32	getFrameRate( void ) const {
			return m_frameRate;
		}

			// set frame rate
			// \param rate: new frame rate
		void	setFrameRate( f32 rate ){
			m_frameRate = rate;
		}

			// is loop
			// \return \b true if animation is looped
		bool isLoop( void ) const {
			return m_isLoop;
		}

			// is play
			// \return \b true if animation is played
		bool isPlay( void ) const {
			return m_isPlay;
		}

			// set loop
			// \param value: \b true if need for looping animation
		void setLoop( bool value ){
			m_isLoop = value;
		}

			// set loop segment
			// \param begin: start id
			// \param end: end id
		void setLoopSegment( u32 begin, u32 end ){
			m_currentFrame = begin;
			m_loopSegment.x = begin;
			m_loopSegment.y = end;
		}

			// play animation
		void play( void ){
			m_isPlay = true;
		}
		
			// pause animation
		void pause( void ){
			m_isPlay = false;
		}

			// stop animation
		void stop( void ){
			m_isPlay = false;
			m_currentFrame = 0u;
		}

			// clear animation
		void clear( void ){
			m_frames = 0u;
			m_currentFrame = 0u;
			m_frameRate = 60.f;
			m_isLoop = false;
			m_isPlay = false;
			m_loopSegment = v2u(0u,0u);
		}

			// get loop segment
			// \return current loop segment
		const v2u& getLoopSegment( void ){
			return m_loopSegment;
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