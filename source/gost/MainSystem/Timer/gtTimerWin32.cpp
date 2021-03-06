//	GOST

#include "common.h"

#if defined(GT_PLATFORM_WIN32)

gtTimerWin32::gtTimerWin32():
	m_delta( 0.f ),
	m_last( 0u ),
	m_now( 0u )
{
	m_ms = gtMainSystem::getInstance();
}

gtTimerWin32::~gtTimerWin32(){}

gtRealTime	gtTimerWin32::getRealTime(){
	SYSTEMTIME st;
	GetLocalTime( &st );

	gtRealTime t;
	t.m_year	=	st.wYear;
	t.m_month	=	st.wMonth;
	t.m_dayWheek=	st.wDayOfWeek;
	t.m_day		=	st.wDay;
	t.m_hour	=	st.wHour;
	t.m_minute	=	st.wMinute;
	t.m_second	=	st.wSecond;

	return t;
}

f32* gtTimerWin32::getDelta(){
	return &m_delta;
}

void gtTimerWin32::updateDelta(){
	static bool b = false;

	if( b )
		m_last = m_now;
	else b = true;

	m_now = m_ms->getTime();
	m_delta = f32(m_now - m_last)*0.001f;

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
