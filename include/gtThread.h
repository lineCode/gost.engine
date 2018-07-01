#pragma once
#ifndef __GT_THREAD_H__
#define __GT_THREAD_H__

namespace gost{

	enum class gtThreadStatus : u32{
		Not_started,
		Running,
		Terminated
	};

		//	Thread
	class gtThread : public gtRefObject{
	protected:

			//	Currecnt status
		gtThreadStatus m_status;

			//	Thread id
		u32 m_id;

	public:
	
			// Constructor
		gtThread():
			m_status( gtThreadStatus::Not_started ),
			m_id( gtConst0U )
		{};

			//	Destructor
		virtual ~gtThread(){};


		typedef void * (GT_CDECL *StartFunction)(void*);


			//	Start new thread
			//	\param f: Function for new thread
			//	\param args: Arguments
			//	\param stackSize: Stack size. 0 means default stack size.
			//	\return \b true if done
		virtual bool start( StartFunction f, void* args, u32 stackSize = 0 ) = 0;

			//	Wait for the thread will be complete.
		virtual void join() = 0;

			//	Get handle
			//	\return handle
		virtual void * getHandle() = 0;

			//	Get current status
			//	\return \see gtThreadStatus
		gtThreadStatus status(){ return m_status; }

			// Get thread id
			//	\return thread id
		u32	id(){ return m_id; }

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