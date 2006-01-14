//
// C++ Interface: get_time
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDGET_TIME_H
#define RESHAKEDGET_TIME_H

namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class GetTime{
protected:				       
	static GetTime *singleton;
	virtual void get_time(unsigned int &sec,unsigned int &usec)=0; //time since app started
public:
	static unsigned int get_time_msec();	
	
	virtual ~GetTime();
	
};

}

#endif
