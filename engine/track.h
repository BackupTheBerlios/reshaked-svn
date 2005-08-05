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



	Track();
	~Track();

};

}

#endif
