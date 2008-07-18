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


