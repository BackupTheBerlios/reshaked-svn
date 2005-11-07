
#ifndef SONG_H
#define SONG_H

#include <vector>
#include "engine/track.h"
#include "audio_graph.h"
#include "engine/data_pool.h"
#include "engine/cursor.h"

namespace ReShaked {


class Song {

	std::vector<Track*> track_list;
	AudioGraph track_graph;
	DataPool *pattern_pool;
	DataPool *automation_pool;
	Cursor cursor;

public:

	Cursor& get_cursor();
	void add_track(TrackType p_type,int p_channels);
	int get_track_count();
	Track* get_track(int p_idx);
	void remove_track(int p_idx); 

	
	Song();
	~Song();
};


};

#endif /* song_h */

