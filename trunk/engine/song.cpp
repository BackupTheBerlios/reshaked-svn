
#include "song.h"
#include "track_pattern.h"
#include "audio_control.h"

namespace ReShaked {


ValueStream<int,String>& Song::get_marker_list() {
	
	return marker_list;
}

GlobalTrack& Song::get_global_track() {
	
	return global_track;
}

BarMap& Song::get_bar_map() {
	
	return bar_map;	
}
AudioGraph& Song::get_track_graph() {
	
	return track_graph;
}

AudioPortLayout& Song::get_audio_port_layout() {
	
	return audio_port_layout;	
}

GlobalProperties& Song::get_global_properties() {
	
	return global_properties;
}

SongPlayback& Song::get_song_playback() {
	
	return song_playback;	
}
void Song::add_track(Track* p_track,int p_at_pos,std::list<AudioGraph::Connection> *p_node_connections) {
	
	if (p_at_pos==-1)
		p_at_pos=track_list.size();
	
	AudioControl::mutex_lock();
	track_list.insert(track_list.begin()+p_at_pos,p_track);
	track_graph.add_node( p_track, p_node_connections);
	p_track->update_plugins_mix_rate();
	AudioControl::mutex_unlock();
	
	
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
	
	AudioControl::mutex_lock();
	track_list.erase( track_list.begin() + p_idx );
	track_graph.erase_node(track_graph.get_node_index(t),p_node_connections);
	AudioControl::mutex_unlock();	

};

void Song::set_input_node(AudioNode *p_node) {
	
	AudioControl::mutex_lock();
	
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
	AudioControl::mutex_unlock();
	
	
}
void Song::set_output_node(AudioNode *p_node) {
	
	AudioControl::mutex_lock();
	
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
	AudioControl::mutex_unlock();
	
}

AudioNode *Song::get_input_node() {
	
	return input_node;
}
AudioNode *Song::get_output_node() {
	
	return output_node;
}

void Song::set_mix_rate(float p_mix_rate) {
	
	song_playback.set_mix_rate(p_mix_rate);
	for (int i=0;i<track_list.size();i++) 
		track_list[i]->update_plugins_mix_rate();
	
}

int Song::process(int p_frames) {
	
	/*
	song_playback.advance(p_frames);
	global_track.process_automations(true);
	track_graph.process( p_frames );
	return p_frames;
	*/

	
	int todo=p_frames;
	int process_size=(1<<process_data.buffer_exp);
	while (todo) {
		
		int to_write=(todo<process_size)?todo:process_size;
		
		song_playback.advance(to_write);
		global_track.process_automations(true);
		
		int to_process=to_write;
		while (to_process) {
			to_process-=track_graph.process( to_process );
		}
		todo-=to_write;
	}
	
	return p_frames;
}


void Song::play(Tick p_from_pos) {

	
	AudioControl::mutex_lock();
	song_playback.play(p_from_pos);
	AudioControl::mutex_unlock();
}

void Song::loop() {
	
	if (loopdata.begin_beat<loopdata.end_beat && loopdata.begin_beat>=0)
		loop(loopdata.begin_beat*TICKS_PER_BEAT, loopdata.end_beat*TICKS_PER_BEAT);
}

void Song::loop(Tick p_begin,Tick p_end) {
	
	ERR_FAIL_COND( !(p_begin>=0 && p_end>p_begin));
	
	AudioControl::mutex_lock();
	song_playback.loop(p_begin,p_end);
	AudioControl::mutex_unlock();
}
void Song::set_pause(bool p_paused) {
	
	AudioControl::mutex_lock();
	song_playback.set_pause(p_paused);
	AudioControl::mutex_unlock();
}
void Song::stop() {
	
	AudioControl::mutex_lock();
	song_playback.stop();
	for (int i=0;i<track_list.size();i++) {
		track_list[i]->reset_automations();
		track_list[i]->reset_plugins();
	}
	AudioControl::mutex_unlock();
}

void Song::set_loop_begin(int p_beat) {
	
	loopdata.begin_beat=p_beat;
	if (loopdata.begin_beat>loopdata.end_beat) {
		
		SWAP(loopdata.begin_beat,loopdata.end_beat);
	}
}
int Song::get_loop_begin() {
	
	
	return loopdata.begin_beat;
}
void Song::set_loop_end(int p_beat) {
	
	loopdata.end_beat=p_beat;
	if (loopdata.begin_beat>loopdata.end_beat) {
		
		SWAP(loopdata.begin_beat,loopdata.end_beat);
	}
	
}
int Song::get_loop_end() {
	
	return loopdata.end_beat;
	
}

AudioNode *Song::get_node_at_visual_pos(int p_pos) {
	
	if (!input_node || !output_node)
		return track_list[p_pos];
	
	if (p_pos==0)
		return input_node;
	else if (p_pos==(track_graph.get_node_count()-1))
		return output_node;
	
	return track_list[p_pos-1];
}



void Song::clear(bool p_new) {
	
	for (int i=0;i<track_list.size();i++) {
			
		delete track_list[i];
	}
	track_list.clear();
	bar_map.reset();
	marker_list.clear();
	audio_port_layout.port_in_info.clear();
	audio_port_layout.port_out_info.clear();
	track_graph.clear();
	global_track.clear();
	input_node=NULL;
	output_node=NULL;
	loopdata.begin_beat=0;
	loopdata.end_beat=0;
		
	if (p_new) {
		
		audio_port_layout.port_out_info.push_back(2);
		audio_port_layout.port_in_info.push_back(2);
		marker_list.insert(0,"Markers");
		
	}
}

void Song::set_info_title(String p_title) {
	
	info.title=p_title;
}
void Song::set_info_author(String p_author) {
	
	info.author=p_author;
}
void Song::set_info_notes(String p_notes) {
	
	info.notes=p_notes;
}

String Song::get_info_title() {
	
	
	return info.title;
}
String Song::get_info_author() {
	
	
	return info.author;
}
String Song::get_info_notes() {
	
	
	return info.notes;
}

Song::Song() : song_playback(&global_properties), global_track(&global_properties,&song_playback) {
	
	input_node=NULL;
	output_node=NULL;
	audio_port_layout.port_out_info.push_back(2);
	audio_port_layout.port_in_info.push_back(2);
	marker_list.insert(0,"Markers");
	loopdata.begin_beat=0;
	loopdata.end_beat=0;
	process_data.buffer_exp = 7;
	track_graph.set_visual_node_order( this );

}

		
Song::~Song() {
	
}

};
