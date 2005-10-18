#ifndef TRANSPORT_H
#define TRANSPORT_H

#include "engine/event_buffer.h"

namespace ReShaked {



class Transport {
	
	
	float initial_tempo;
	
	
	EventBuffer event_buffer;
public:
	
	EventBuffer& get_sequencer_event_buffer();
	
	
	void process(int p_frames);

	
};










};

#endif
