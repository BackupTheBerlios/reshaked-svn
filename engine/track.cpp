//
// C++ Implementation: track
//
// Description: 
//
//
// Author: Juan Linietsky <coding@reduz.com.ar>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "track.h"

namespace ReShaked {



void Track::set_sequencer_event_buffer(const EventBuffer *p_seq) {
	
	base_private.seq_events=p_seq;	
}


const EventBuffer& Track::get_seq_event_buffer() {
	
	return *base_private.seq_events;
}


/* Automations */
int Track::get_automation_count() {
	
	return base_private.automations.size();	
}
Automation *Track::get_automation(int p_index) {
	
	ERR_FAIL_INDEX_V(p_index,(int)base_private.automations.size(),NULL);
	return base_private.automations[p_index];
}

Track::Track() {
	
	base_private.seq_events=NULL;
}


Track::~Track()
{
}


}
