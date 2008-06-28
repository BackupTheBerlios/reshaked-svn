
#ifndef SONG_H
#define SONG_H

#include <vector>
#include "engine/track.h"
#include	 "audio_graph.h"
#include "engine/data_pool.h"
#include "engine/bar_map.h"
#include "engine/audio_port_layout.h"
#include "engine/global_track.h"
#include "engine/song_playback.h"
#include "engine/audio_process_base.h"

namespace ReShaked {


class Song : public AudioProcessBase, public AudioGraph::VisualNodeOrder {
private:
	std::vector<Track*> track_list;
	AudioGraph track_graph;
	BarMap bar_map;
	AudioPortLayout audio_port_layout;
	GlobalProperties global_properties;
	SongPlayback song_playback;
	GlobalTrack global_track;
	
	struct Loop {
		int begin_beat;
		int end_beat;	
	} loopdata;
	
	struct ProcessData {
		
		int buffer_exp;
		float max_gain;
		
	} process_data;
	
	AudioNode *input_node; ///< Global Input Node
	AudioNode *output_node; ///< Global Output Node

	
	ValueStream<int,String> marker_list; ///< beat/name
	virtual AudioNode *get_node_at_visual_pos(int p_pos);
	
	struct Info {
		
		String title;
		String author;
		String notes;
		
	} info;
public:

	GlobalProperties& get_global_properties();
	GlobalTrack& get_global_track();
	BarMap& get_bar_map();
	AudioGraph& get_track_graph();
	AudioPortLayout& get_audio_port_layout();
	ValueStream<int,String>& get_marker_list();
	SongPlayback& get_song_playback();
	
	void add_track(Track* p_track,int p_at_pos=-1,std::list<AudioGraph::Connection> *p_node_connections=NULL); ///< pos, or -1 for at end (default)
	int get_track_count();
	Track* get_track(int p_idx);
	void remove_track(int p_idx,std::list<AudioGraph::Connection> *p_node_connections=NULL); 
	
	void set_input_node(AudioNode *p_node);
	void set_output_node(AudioNode *p_node);

	AudioNode *get_input_node();
	AudioNode *get_output_node();
	
	
	void play(Tick p_from_pos=0);
	void loop(Tick p_begin,Tick p_end);
	void loop();
	void set_pause(bool p_paused);
	void stop();
	
	float grab_accumulated_max_gain();
	
	void set_loop_begin(int p_beat);
	int get_loop_begin();
	void set_loop_end(int p_beat);
	int get_loop_end();
	
	int process(int p_frames);
	void set_mix_rate(float p_mix_rate);
	
	void clear(bool p_new=false);
	
	void set_info_title(String p_title);
	void set_info_author(String p_author);
	void set_info_notes(String p_notes);
	
	String get_info_title();
	String get_info_author();
	String get_info_notes();
	
	Song();
	~Song();
};


};

#endif /* song_h */

