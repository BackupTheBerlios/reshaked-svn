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
#include "engine/block_list.h"


namespace ReShaked {

/**
@author Juan Linietsky
*/

class Track : public AudioNode , public BlockList {
public:

	class TrackAutomation : public Automation {
	friend class Track;	// will add more stuff later
		void *private_data;
		String param_string;
		
	};
	

	static int static_id;
	
	struct BasePrivate {

		int id;
		int channels;
		const EventBuffer * seq_events;
		std::vector<TrackAutomation*> automations;
		AudioPlug *input_plug;
		AudioPlug *output_plug;

		ValueStream<Tick,Block*> block_list;

	} base_private;

	int get_input_plug_count();
	int get_output_plug_count();
	AudioPlug *get_input_plug(int p_index);
	AudioPlug *get_output_plug(int p_index);
	
	void process(int p_frames);

protected:


	const EventBuffer& get_seq_event_buffer();

	virtual void process_track(AudioBuffer *p_in_buf,AudioBuffer *p_out_buf,int p_frames)=0;

public:

	
	/* Sequencer Event Handling */
	void set_sequencer_event_buffer(const EventBuffer *p_seq);

	/* Automations */

	void add_automation(String p_path);	
	int get_automation_count();
	Automation *get_automation(int p_index);
	

	Track(int p_channels,BlockType p_type);
	~Track();

};

}

#endif
