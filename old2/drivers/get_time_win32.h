//
// C++ Interface: get_time_win32
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDGET_TIME_WIN32_H
#define RESHAKEDGET_TIME_WIN32_H

#include "version.h"

#ifdef WIN32_ENABLED

#include "get_time.h"
#include <windows.h>

namespace ReShaked {

/**
	@author red <red@killy>
*/
class GetTime_Win32 : public GetTime{
	
	LARGE_INTEGER time_begin;
	LARGE_INTEGER freq_divisor;
	virtual void get_time(unsigned int &sec,unsigned int &usec); //time since app started
	
public:
	
	GetTime_Win32();
	~GetTime_Win32();

};

}

#endif //meh

#endif
