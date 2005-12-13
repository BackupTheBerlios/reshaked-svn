
#ifndef SONG_H
#define SONG_H

#include <vector>
#include "engine/track.h"
#include "audio_graph.h"
#include "engine/data_pool.h"
#include "engine/bar_map.h"


namespace ReShaked {


class Song {
private:
	std::vector<Track*> track_list;
	AudioGraph track_graph;

	BarMap bar_map;
public:

	BarMap& get_bar_map();
	AudioGraph& get_track_graph();
	
	void add_track(Track* p_track,int p_at_pos=-1); ///< pos, or -1 for at end (default)
	int get_track_count();
	Track* get_track(int p_idx);
	void remove_track(int p_idx); 
	
	Song();
	~Song();
};


};

#endif /* song_h */

