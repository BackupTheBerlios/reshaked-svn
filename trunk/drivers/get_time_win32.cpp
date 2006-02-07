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
	time.QuadPart/=freq_divisor.QuadPart;
	time.QuadPart-=time_begin.QuadPart;
	
	usec=(int)( (time.QuadPart%1000)*1000 );
	sec=time.QuadPart/1000;	
}


GetTime_Win32::GetTime_Win32()
{
	QueryPerformanceCounter(&time_begin);
	QueryPerformanceFrequency(&freq_divisor);
	freq_divisor.QuadPart/=1000; // this way, further divisions will just return milliseconds
	
	time_begin.QuadPart/=freq_divisor.QuadPart; //move to msec
}


GetTime_Win32::~GetTime_Win32()
{
}


}


#endif //win32 enabled