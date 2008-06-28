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
#include "version.h"
#ifdef WIN32_ENABLED
#include "typedefs.h"

namespace ReShaked {


void GetTime_Win32::get_time(unsigned int &sec,unsigned int &usec) {
	
	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);
	
	time.QuadPart-=time_begin.QuadPart; //difference

	usec=time.QuadPart%freq_divisor.QuadPart;
	usec = (Uint64)usec * 1000000L / freq_divisor.QuadPart;

	
	sec=time.QuadPart/freq_divisor.QuadPart;

}


GetTime_Win32::GetTime_Win32()
{
	QueryPerformanceCounter(&time_begin);
	QueryPerformanceFrequency(&freq_divisor);
}


GetTime_Win32::~GetTime_Win32()
{
}


}


#endif //win32 enabled