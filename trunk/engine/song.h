//
// C++ Interface: song
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SONG_H
#define SONG_H

#include "engine/audio_graph.h"
#include "engine/track.h"
#include "engine/bar_map.h"
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class Song {

	AudioGraph audio_graph;
	
	static Song *singleton;
	
	std::vector<Track*> tracks;
	
	ValueStream<int,String> marker_map;
	BarMap bar_map;
	
	
	struct Loop {
	
		int begin,end;
	} loop;
	
public:

	void add_track(Track* p_track,int p_at_idx=-1);
	void remove_track(int p_at_idx);
	int get_track_count() const;
	Track *get_track(int p_index) const;
	int find_track_pos(Track *p_track) const;
	void swap_tracks(int p_track, int p_with_track);
	
	BarMap& get_bar_map();
	ValueStream<int,String>& get_marker_map();

	int process(int p_frames);


	void set_loop_begin(int p_loop_begin);
	int get_loop_begin() const;

	void set_loop_end(int p_loop_end);
	int get_loop_end() const;

	static Song *get_singleton();
	
	AudioGraph* get_audio_graph();
	
	Song();	
	~Song();

};

#endif
