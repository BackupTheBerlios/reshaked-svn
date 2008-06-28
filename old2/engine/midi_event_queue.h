//
// C++ Interface: midi_event_queue
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDMIDI_EVENT_QUEUE_H
#define RESHAKEDMIDI_EVENT_QUEUE_H

#include "typedefs.h"
#include "dsp/lock_free_ring_buffer.h"
#include "engine/event.h"
namespace ReShaked {

/**
	@author red <red@killy>
*/
class MidiEventQueue {
	
	enum {
		MAX_QUEUE_EVENTS=2048
	};
	
	/* Insanely strange methodology for inserting events and sorting them at extremely high speed */
	
	struct LocalQueue {
		
		EventMidi event_array[MAX_QUEUE_EVENTS];
		
		int events_used;
		
		struct EventNode {
			
			int event_idx;
			int next_event_node; //-1 for end
			
		} event_list[MAX_QUEUE_EVENTS];
		
		
		int last_inserted_node;
	} local_queue;
			
			
	
protected:
	
	/* To be called from MIDI Thread */
	
	bool queue_rt_events_pending();
	bool queue_rt_pop_event(const EventMidi& p_ev);
	Sint64 queue_rt_get_time_for_next_event();
	
	/* To be called from audio thread */
	void queue_push_event(const EventMidi& p_ev);
	void queue_commit_events();
	
public:
	
	MidiEventQueue();
	~MidiEventQueue();

};

}

#endif
