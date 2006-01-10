#ifndef EVENT_BUFFER_H
#define EVENT_BUFFER_H

#include "engine/event.h"

namespace ReShaked {



class EventBuffer {

	enum {
		
		DEFAULT_MAX_SIZE=1024
	};


	int max_size;
	Event *events; /* stored here */
	Event **event_ptrs; /* sorted here */
	int size;

public:

	void clear();
	inline void push_event(const Event& p_event);
	inline int get_event_count() const;
	inline const Event* get_event(int p_idx) const;
	
	EventBuffer(int p_max_size=DEFAULT_MAX_SIZE);
};

void EventBuffer::push_event(const Event& p_event) {
	
	if (size==max_size)
		return; /* buffer full */
	
	/* First, add the event */
	events[size]=p_event; 
	
	/* Then update the pointer */
	if (size<=0 || event_ptrs[size-1]->frame_offset<=p_event.frame_offset) {
		/* If the events are added in order, insertion is done at virtually no cost */
		event_ptrs[size]=&events[size];
	} else {
		/* But if not, then the event must be inserted in the proper place */
		int insert_index=0;
		/* find the place where it belongs */
		while (p_event.frame_offset>=event_ptrs[insert_index]->frame_offset)
			insert_index++;
		
		/* Make space for it (could use memcopy) */
		for (int i=size;i>insert_index;i--)
			event_ptrs[i]=event_ptrs[i-1];
		
		event_ptrs[insert_index]=&events[size];
		
	}
	
	
	size++;
}

inline int EventBuffer::get_event_count() const {
	
	return size;	
}

inline const Event* EventBuffer::get_event(int p_idx) const {
	
	return event_ptrs[p_idx];	
}


};
#endif
