//
// C++ Implementation: get_time
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "get_time.h"

namespace ReShaked {

GetTime *GetTime::singleton=0;

unsigned int GetTime::get_time_msec() {
	
	if (!singleton)
		return 0;
	
	unsigned int sec,usec;
	singleton->get_time(sec,usec);
	sec*=1000;
	usec/=1000;
	return sec+usec;
}

Sint64 GetTime::get_time_usec() {
	
	if (!singleton)
		return 0;
	
	unsigned int sec,usec;
	singleton->get_time(sec,usec);
	
	Sint64 time=sec;
	time*=1000000;
	time+=(Sint64)usec;
	
	return time;
}

GetTime::~GetTime() {}
}
