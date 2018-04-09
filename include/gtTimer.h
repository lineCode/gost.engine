/*!	GOST
	\file gtTimer.h
	\brief Работа с временем
*/
#pragma once
#ifndef __GT_TIMER_H__
#define __GT_TIMER_H__ //< include guard

namespace gost{
	
		// хранит реальное время
	struct gtRealTime{
		
			//	год
		s32 m_year;

			//	месяц 1 - 12
		u32 m_month;

			//	день 1 - 31
		u32 m_day;

			//	день недели 0 - 6, sunday, monday…
		u32 m_dayWheek;

			//	час	0 - 23
		u32 m_hour;

			//	минута 0 - 59
		u32 m_minute;

			//	секунда 0 - 59
		u32 m_second;
	};

		// содержит функции для работы с временем
	class gtTimer : public gtRefObject{
	public:

			// Получить реальное время
			// \return объект gtRealTime
		virtual gtRealTime	getRealTime( void ) = 0;
		
	};


}

#endif

/*
Copyright (c) 2017-2018 532235

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