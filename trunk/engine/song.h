
#ifndef SONG_H
#define SONG_H

#include <vector>
#include "engine/track.h"
#include "audio_graph.h"
#include "engine/data_pool.h"
#include "engine/bar_map.h"
#include "engine/audio_port_layout.h"
#include "engine/global_track.h"


namespace ReShaked {


class Song {
private:
	std::vector<Track*> track_list;
	AudioGraph track_graph;
	BarMap bar_map;
	AudioPortLayout audio_port_layout;
	GlobalProperties global_properties;
	GlobalTrack global_track;
	
	AudioNode *input_node; ///< Global Input Node
	AudioNode *output_node; ///< Global Output Node

 
	ValueStream<int,String> marker_list; ///< beat/name
public:

	GlobalProperties& get_global_properties();
	GlobalTrack& get_global_track();
	BarMap& get_bar_map();
	AudioGraph& get_track_graph();
	AudioPortLayout& get_audio_port_layout();
	ValueStream<int,String>& get_marker_list();
	
	void add_track(Track* p_track,int p_at_pos=-1,std::list<AudioGraph::Connection> *p_node_connections=NULL); ///< pos, or -1 for at end (default)
	int get_track_count();
	Track* get_track(int p_idx);
	void remove_track(int p_idx,std::list<AudioGraph::Connection> *p_node_connections=NULL); 
	
	void set_input_node(AudioNode *p_node);
	void set_output_node(AudioNode *p_node);
	
	AudioNode *get_input_node();
	AudioNode *get_output_node();
	
	void set_mix_rate(float p_mix_rate);
	
	Song();
	~Song();
};


};

#endif /* song_h */

