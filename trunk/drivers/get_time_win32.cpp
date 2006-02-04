//
// C++ Implementation: get_time_win32
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "get_time_win32.h"

#ifdef WIN32_ENABLED

namespace ReShaked {


void GetTime_Win32::get_time(unsigned int &sec,unsigned int &usec) {
	
	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);
	time/=freq_divisor;
	time-=time_begin;
	
	usec=(int)( (time%1000)*1000 );
	sec=time/1000;	
}


GetTime_Win32::GetTime_Win32()
{
	QueryPerformanceCounter(&time_begin);
	QueryPerformanceFrequency(&freq_divisor);
	freq_divisor/=1000; // this way, further divisions will just return milliseconds
	
	time_begin/=freq_divisor; //move to msec
}


GetTime_Win32::~GetTime_Win32()
{
}


}


#endif //win32 enabled