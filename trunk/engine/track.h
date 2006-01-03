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
#include "engine/global_properties.h"
#include "engine/sound_plugin.h"
#include "engine/audio_graph.h"
#include "engine/proxy_node.h"
#include "property.h"


namespace ReShaked {

/**
@author Juan Linietsky
*/

class Track : public AudioNode , public BlockList, public ProxyNodeBase {
public:

	class TrackAutomation : public Automation {
	friend class Track;	// will add more stuff later
		void *private_data;
		bool visible;
		TrackAutomation(Property *p_property) : Automation(p_property) { private_data=NULL; }
	};
	
	
	struct PropertyRef {
		
		Property *property; //pointer to property
		String visual_path;
		TrackAutomation *automated;
	};
	
	struct PluginInsertData { /* crappy struct so undo/redo works */
		
		SoundPlugin *plugin;
		int pos; //pos where to insert, -1 means default
		struct AutomationTrack {
			TrackAutomation *automation;
			int pos;
		};
			
		std::list<AutomationTrack> automated_tracks; //automations in use by the plugin
		std::list<AudioGraph::Connection> connections; //connections of the plugin
		PluginInsertData() { plugin=NULL; pos=-1; }
	};
private:
	
	struct BasePrivate {


		int channels;
		const EventBuffer * seq_events;
		std::vector<TrackAutomation*> automations;
		AudioPlug *input_plug;
		AudioPlug *output_plug;

		ValueStream<Tick,Block*> block_list;
		
		
	
		std::vector<PropertyRef*> property_list;
		AudioGraph plugin_graph;
		std::vector<SoundPlugin*> sound_plugins;
		
		String name;
		GlobalProperties *global_props;
		
		ProxyNode input_proxy;
		ProxyNode output_proxy;
		
	} base_private;

	int get_input_plug_count();
	int get_output_plug_count();
	AudioPlug *get_input_plug(int p_index);
	AudioPlug *get_output_plug(int p_index);
	
	void process(int p_frames);
	void feed_input(int p_frames);
	void read_output(int p_frames);
	
protected:

	
	const EventBuffer& get_seq_event_buffer();
	GlobalProperties &get_global_props();

	void add_property(String p_visual_path,Property *p_prop);

	String get_caption();
	
	virtual bool is_visible();
	
public:

	/* plugins */
	
	int get_plugin_count();
	SoundPlugin *get_plugin(int p_index);
	AudioGraph& get_plugin_graph();
	void add_plugin(PluginInsertData* p_plugin);
	void remove_plugin(int p_pos,PluginInsertData* p_plugin_recovery);
	
	/* Sequencer Event Handling */
	void set_sequencer_event_buffer(const EventBuffer *p_seq);

	/* Automations/Props */

	int get_property_count();
	//int get_idx_by_path(String p_path);
	Property *get_property(int p_idx);
	Automation *get_property_automation(int p_idx);
	String get_property_visual_path(int p_idx);
	void property_show_automation(int p_idx);
	void property_hide_automation(int p_idx);
	int get_visible_automation_count();
	Automation *get_visible_automation(int p_index);
	bool has_property_automation(int p_index);
	bool has_property_visible_automation(int p_index);
	
	
	void set_name(String p_name);
	String get_name();
	int get_channels();
	
	Track(int p_channels,BlockType p_type,GlobalProperties *p_global_props);
	~Track();

};

}

#endif
