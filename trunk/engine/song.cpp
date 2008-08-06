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

int Song::process(int p_frames) {

	return audio_graph.process(p_frames);
}

Song::Song() {

	ERR_FAIL_COND( singleton );
	singleton=this;
}


Song::~Song()
{
}


