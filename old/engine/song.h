
#ifndef SONG_H
#define SONG_H

#include <vector>
#include "engine/track.h"
#include "audio_graph.h"
#include "engine/pattern_list.h"

namespace ReShaked {

class Song {

	std::vector<Track*> track_list;
	AudioGraph track_graph;
	PatternList pattern_list;

public:

	PatternList &get_pattern_list();
	void add_track(Track *p_track);
	int get_track_count();
	Track* get_track(int p_idx);
	void remove_track(int p_idx); /* remove but dont delete */

};


};

#endif /* song_h */

