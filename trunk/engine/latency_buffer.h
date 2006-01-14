//
// C++ Interface: latency_buffer
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDLATENCY_BUFFER_H
#define RESHAKEDLATENCY_BUFFER_H

#include "typedefs.h"

namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
					       
//not the most accurate thing ever, but accurate enough
					       
class LatencyBuffer{

	enum {
		MSECS_MAX=4000,
	};
	
	
	Tick ring_buffer[MSECS_MAX];
	float indexf;
	int index;
	int written;

public:
	
	void clear();
	void add(Tick p_tick,Tick p_ticks,float p_msecs );
	Tick get_tick_at_latency(int p_frames);
	
	LatencyBuffer();
	~LatencyBuffer();

};

}

#endif
