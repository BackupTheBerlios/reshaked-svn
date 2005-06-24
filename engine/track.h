//
// C++ Interface: track
//
// Description: 
//
//
// Author: Juan Linietsky <coding@reduz.com.ar>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDTRACK_H
#define RESHAKEDTRACK_H

#include "engine/audio_node.h"
#include "engine/event_buffer.h"
#include "engine/automation.h"
namespace ReShaked {

/**
@author Juan Linietsky
*/
class Track : public AudioNode {

	struct BasePrivate {
		
		int channels;
		const EventBuffer * seq_events;
		std::vector<Automation*> automations;		
	} base_private;
protected:	

	
	const EventBuffer& get_seq_event_buffer();
	void add_automation(String p_path,Automation *p_automation);
	
			
public:

	
	/* Sequencer Event Handling */
	void set_sequencer_event_buffer(const EventBuffer *p_seq);	
	
	/* Automations */
	int get_automation_count();
	Automation *get_automation(int p_index);
	
	/**
	 * Relation of one-to-many:
	 * Outputs need to have a buffer associated, and inputs read from them?
	 * If inputs read from many outputs, they need their own buffer
	 * so then isnt it best to have a buffer for each connected input? as they
	 * dont need to e replicated and can be connected to a null/empty buffer
	 * since it's constant */
	

	
	Track();
	~Track();

};

}

#endif
