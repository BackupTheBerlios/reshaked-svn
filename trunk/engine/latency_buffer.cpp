//
// C++ Implementation: latency_buffer
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "latency_buffer.h"
#include <math.h>
namespace ReShaked {


void LatencyBuffer::clear() {
	
	written=0;
	index=0;
	indexf=0;
}

void LatencyBuffer::add(Tick p_tick,Tick p_ticks,float p_msecs ) {
	if (p_msecs<=0.0)
		return;
	
	float new_pos=fmodf(indexf+p_msecs,(float)MSECS_MAX);
	int new_pos_i=(int)new_pos;
	if (new_pos_i>=MSECS_MAX) { //may happen, it's floats..
		new_pos_i=0;
		new_pos=0.0;
	}

	while (index!=new_pos_i) {
		
		index=(index+1)%MSECS_MAX;
		p_tick+=Tick((float)p_ticks/p_msecs);
		ring_buffer[index]=p_tick;
		if (written<MSECS_MAX)
			written++;
	}
	
	indexf=new_pos;
	
}


Tick LatencyBuffer::get_tick_at_latency(int p_msecs) {
	//all this is a piece of shit, and i will fix it some day!
	if (written==0)
		return -1;
	
	if (p_msecs>(written/2)) //safety
		p_msecs=(written/2);
	
	int pos=(MSECS_MAX+(index-p_msecs))%MSECS_MAX;
	if (pos<0)
		pos=0;
	return ring_buffer[pos];
}

LatencyBuffer::LatencyBuffer()
{
}


LatencyBuffer::~LatencyBuffer()
{
}


}
