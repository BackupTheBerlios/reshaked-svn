//
// C++ Implementation: song
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "song.h"


Song *Song::singleton=NULL;


void Song::add_track(Track* p_track,int p_at_idx) {

	if (p_at_idx<0 || p_at_idx>=tracks.size())
		tracks.push_back(p_track);
	else
		tracks.insert( tracks.begin() + p_at_idx, p_track );
}
void Song::remove_track(int p_at_idx) {

	ERR_FAIL_INDEX( p_at_idx, tracks.size() );
	tracks.erase( tracks.begin() + p_at_idx );
}
int Song::get_track_count() const {

	return tracks.size();
}
Track *Song::get_track(int p_index) const {

	ERR_FAIL_INDEX_V( p_index, tracks.size(), NULL );
	return tracks[p_index];
}

void Song::swap_tracks(int p_track, int p_with_track) {

	ERR_FAIL_INDEX(p_track,tracks.size());
	ERR_FAIL_INDEX(p_with_track,tracks.size());
	
	SWAP( tracks[p_track], tracks[p_with_track] );
}

int Song::find_track_pos(Track *p_track) const {

	for (int i=0;i<tracks.size();i++) {
	
		if (tracks[i]==p_track)
			return i;
	}
	
	return -1;
}

AudioGraph* Song::get_audio_graph() {

	return &audio_graph;
}

Song *Song::get_singleton() {

	return singleton;
}

BarMap& Song::get_bar_map() {

	return bar_map;
}

ValueStream<int,String>& Song::get_marker_map() {

	return marker_map;
}

void Song::set_loop_begin(int p_loop_begin) {

	loop.begin=p_loop_begin;
}
int Song::get_loop_begin() const  {

	return loop.begin;
}
void Song::set_loop_end(int p_loop_end) {

	loop.end=p_loop_end;
}
int Song::get_loop_end() const  {

	return loop.end;
}

void Song::set_initial_tempo(float p_bpm) {

	if (p_bpm<32)
		p_bpm=32;
		
	if (p_bpm>255)
		p_bpm=255;
	
	initial_tempo=p_bpm;
}

float Song::get_initial_tempo() const {

	return initial_tempo;
}

int Song::process(int p_frames) {


	return audio_graph.process(p_frames);
}

Song::Song() {

	ERR_FAIL_COND( singleton );
	singleton=this;
	marker_map.insert(0,"Markers");
	loop.begin=0;
	loop.end=0;
	initial_tempo=125;
}


Song::~Song()
{
}


