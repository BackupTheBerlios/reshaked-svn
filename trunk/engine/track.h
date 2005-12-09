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

#include "property.h"


namespace ReShaked {

/**
@author Juan Linietsky
*/

class Track : public AudioNode , public BlockList {
public:

	class TrackAutomation : public Automation {
	friend class Track;	// will add more stuff later
		void *private_data;
		String path;
		bool visible;
		TrackAutomation(String p_path,Property *p_property) : Automation(p_property) { private_data=NULL; path=p_path; }
	};
	
	
	struct PropertyRef {
		
		Property *property; //pointer to property
		String path;
		TrackAutomation *automated;
	};

	struct BasePrivate {


		int channels;
		const EventBuffer * seq_events;
		std::vector<TrackAutomation*> automations;
		AudioPlug *input_plug;
		AudioPlug *output_plug;

		ValueStream<Tick,Block*> block_list;
		
		
		LocalProperty swing;
		LocalProperty volume;
		LocalProperty balance;
		
		std::vector<PropertyRef*> property_list;
		
		String name;

	} base_private;

	int get_input_plug_count();
	int get_output_plug_count();
	AudioPlug *get_input_plug(int p_index);
	AudioPlug *get_output_plug(int p_index);
	
	void process(int p_frames);

	
protected:

	
	const EventBuffer& get_seq_event_buffer();

	virtual void process_track(AudioBuffer *p_in_buf,AudioBuffer *p_out_buf,int p_frames)=0;
	void add_property(String p_path,Property *p_prop);

public:

	Property &swing();
	Property &volume();
	Property &balance();
	
	
	/* Sequencer Event Handling */
	void set_sequencer_event_buffer(const EventBuffer *p_seq);

	/* Automations/Props */

	int get_property_count();
	int get_idx_by_path(String p_path);
	Property *get_property(int p_idx);
	Automation *get_property_automation(int p_idx);
	String get_property_path(int p_idx);

	
	void show_automation(String p_path);
	void hide_automation(String p_path);
	int get_visible_automation_count();
	Automation *get_visible_automation(int p_index);
	bool has_property_automation(int p_index);
	bool has_property_visible_automation(int p_index);
	
	
	void set_name(String p_name);
	String get_name();
	
	Track(int p_channels,BlockType p_type);
	~Track();

};

}

#endif
