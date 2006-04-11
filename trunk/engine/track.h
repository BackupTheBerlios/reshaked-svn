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
#include "engine/song_playback.h"
#include "property.h"


namespace ReShaked {

/**
@author Juan Linietsky
*/

class Track : public AudioNode , public BlockList, public ProxyNodeBase, public AudioGraph::VisualNodeOrder {
public:

	class TrackAutomation : public Automation {
	friend class Track;	// will add more stuff later
		void *private_data;
		bool visible;
		int property_idx;
	public:
		TrackAutomation(Property *p_property) : Automation(p_property) { private_data=NULL; visible=false; }
	};
	
	
	struct PropertyRef {
		
		Property *property; //pointer to property
		String visual_path;
		TrackAutomation *automation;
		bool builtin;
		
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
		PluginInsertData(SoundPlugin *p_plugin=NULL) { plugin=p_plugin; pos=-1; }
	};
private:
	
	struct BasePrivate {


		int channels;
		const EventBuffer * seq_events;
		std::vector<TrackAutomation*> active_automation_cache;
		AudioPlug *input_plug;
		AudioPlug *output_plug;

		std::vector<PropertyRef*> property_list;
		AudioGraph plugin_graph;
		std::vector<SoundPlugin*> sound_plugins;
		
		String name;
		GlobalProperties *global_props;
		
		ProxyNode input_proxy;
		ProxyNode output_proxy;
		SongPlayback *song_playback;
		
		
		String rack_file;
		struct Audio {
			float highest_energy;
			float volume_ratio;
			bool mute;
		} audio;
		
	} base_private;

	int get_input_plug_count();
	int get_output_plug_count();
	AudioPlug *get_input_plug(int p_index);
	AudioPlug *get_output_plug(int p_index);
	
	void process(int p_frames);
	void feed_input(int p_frames);
	void read_output(int p_frames);
	
	void validate_plugin_duplicate(SoundPlugin *p_plugin);
protected:

	
	SongPlayback*get_song_playback();
	const EventBuffer& get_seq_event_buffer();
	GlobalProperties &get_global_props();

	void add_property(String p_visual_path,Property *p_prop,TrackAutomation *p_automation=NULL,int p_pos=-1,bool p_builtin=true);
	
	

	String get_caption();
	
	virtual bool is_visible();
	virtual void plugin_added_notify(SoundPlugin *p_plugin);
	virtual void track_pre_process(int p_frames);
	
	void rebuild_active_automation_cache();	
	AudioNode *get_node_at_visual_pos(int p_pos);	
	
	void process_automations_at_tick(Tick p_tick);	
public:
	

	void offline_process_automations(Tick p_tick);
	void process_automations(bool p_use_current_tick_to=false);

	/* plugins */
	
	AudioNode *get_record_node();
	AudioNode *get_playback_node();
	int get_plugin_count();
	SoundPlugin *get_plugin(int p_index);
	AudioGraph& get_plugin_graph();
	void add_plugin(PluginInsertData* p_plugin);
	void remove_plugin(int p_pos,PluginInsertData* p_plugin_recovery);
	void update_plugins_mix_rate();
	void move_plugin_left(int p_index);
	void move_plugin_right(int p_index);
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
	bool has_property_visible_automation(int p_index);
	bool is_property_builtin(int p_index); ///< property is builtin of track (as in, not from a plugin)
	
	int find_plugin_idx_for_property(Property *p_property);

	int find_recording_property(int p_chan,int p_control);
	
	void set_name(String p_name);
	String get_name();
	int get_channels();
	
	void reset_automations();
	float read_highest_energy();
	void set_mute(bool p_mute);
	bool is_mute();
	
	void reset_plugins();
	
	void set_rack_file(String p_file);
	String get_rack_file();
	
	virtual bool can_use_synths()=0;
	
	Track(int p_channels,BlockType p_type,GlobalProperties *p_global_props,SongPlayback *p_song_playback);
	~Track();

};

}

#endif
