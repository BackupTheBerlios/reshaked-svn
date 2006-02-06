//
// C++ Interface: get_time_posix
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDGET_TIME_POSIX_H
#define RESHAKEDGET_TIME_POSIX_H

#include "version.h"

#ifdef POSIX_ENABLED

#include "get_time.h"

#include <sys/time.h>
#include <time.h>

namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class GetTime_Posix : public GetTime{

	struct timeval app_begin_time;
	virtual void get_time(unsigned int &sec,unsigned int &usec); //time since app started
				       
public:
	GetTime_Posix();
	~GetTime_Posix();

};

}

#endif /* posix enabled */

#endif
