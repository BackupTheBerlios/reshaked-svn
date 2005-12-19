
#include "song.h"
#include "track_pattern.h"

namespace ReShaked {

BarMap& Song::get_bar_map() {
	
	return bar_map;	
}
AudioGraph& Song::get_track_graph() {
	
	return track_graph;
}

AudioPortLayout& Song::get_audio_port_layout() {
	
	return audio_port_layout;	
}

void Song::add_track(Track* p_track,int p_at_pos,std::list<AudioGraph::Connection> *p_node_connections) {
	
	if (p_at_pos==-1)
		p_at_pos=track_list.size();
	
	track_list.insert(track_list.begin()+p_at_pos,p_track);
	track_graph.add_node( p_track, p_node_connections);

	
}

int Song::get_track_count() {

	return track_list.size();
}

Track* Song::get_track(int p_idx) {

	ERR_FAIL_INDEX_V(p_idx,track_list.size(),NULL);
	return track_list[p_idx];
}

void Song::remove_track(int p_idx,std::list<AudioGraph::Connection> *p_node_connections) { /* remove but dont delete */

	ERR_FAIL_INDEX(p_idx,track_list.size());
	Track *t=track_list[p_idx];
	track_list.erase( track_list.begin() + p_idx );
	track_graph.erase_node(track_graph.get_node_index(t),p_node_connections);
	

};

void Song::set_input_node(AudioNode *p_node) {
	
	if (input_node) {
	
		std::list<AudioGraph::Connection> connections;
		int idx=track_graph.get_node_index( input_node );
		if (idx>=0) 
			track_graph.erase_node( idx, &connections );
		else {
			ERR_PRINT("input idx is -1 ?");
		}
		
		track_graph.add_node( p_node, &connections );
	} else {
		
		track_graph.add_node( p_node );
	
	}
	
	input_node=p_node;	
	
}
void Song::set_output_node(AudioNode *p_node) {
	
	if (output_node) {
		
		std::list<AudioGraph::Connection> connections;
		int idx=track_graph.get_node_index( output_node );
		if (idx>=0) 
			track_graph.erase_node( idx, &connections );
		else {
			ERR_PRINT("output node idx is -1 ?");
		}
		
		track_graph.add_node( p_node, &connections );
	} else {
		
		track_graph.add_node( p_node );
		
	}
	
	output_node=p_node;	
}

AudioNode *Song::get_input_node() {
	
	return input_node;
}
AudioNode *Song::get_output_node() {
	
	return output_node;
}

void Song::set_mix_rate(float p_mix_rate) {
	
	
}

Song::Song()  {
	
	input_node=NULL;
	output_node=NULL;
	audio_port_layout.port_out_info.push_back(2);
	audio_port_layout.port_in_info.push_back(2);
}

Song::~Song() {
	
}

};
