#include "event_buffer.h"


namespace ReShaked {



void EventBuffer::clear() {

	size=0;	
	
}

EventBuffer::EventBuffer(int p_max_size) {
	
	events = new Event[p_max_size];
	event_ptrs = new Event*[p_max_size];
	max_size=p_max_size;
	clear();
	
}

EventBuffer::~EventBuffer() {
	
	delete [] events;
	delete [] event_ptrs;
}






};
