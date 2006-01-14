//
// C++ Implementation: get_time_posix
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//


#include "get_time_posix.h"

#ifdef POSIX_ENABLED

namespace ReShaked {




void GetTime_Posix::get_time(unsigned int &sec,unsigned int &usec) {

	struct timeval time_now;
	gettimeofday(&time_now,NULL);
	
	
	struct timeval time_diff;
	struct timeval time_then=app_begin_time;
	
	
	if (time_then.tv_usec>time_now.tv_usec) { /* compensate difference to usecs */
		
		time_now.tv_usec+=1000000; //one second in usecs!
		time_now.tv_sec-=1; //one less second in secs!
	}
	
	time_diff.tv_sec=time_now.tv_sec-time_then.tv_sec;
	time_diff.tv_usec=time_now.tv_usec-time_then.tv_usec;
	
	sec=time_diff.tv_sec;
	usec=time_diff.tv_usec;	
}
GetTime_Posix::GetTime_Posix() {
	
	gettimeofday(&app_begin_time,NULL);
	singleton=this;
}


GetTime_Posix::~GetTime_Posix()
{
}



}

#endif /* POSIX_ENABLED */