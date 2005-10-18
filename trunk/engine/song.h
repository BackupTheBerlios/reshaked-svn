
#ifndef SONG_H
#define SONG_H

#include <vector>
#include "engine/track.h"
#include "audio_graph.h"


namespace ReShaked {

class Song {

	std::vector<Track*> track_list;
	AudioGraph track_graph;

public:

	void add_track(TrackType p_type,int p_channels);
	int get_track_count();
	Track* get_track(int p_idx);
	void remove_track(int p_idx); 

};


};

#endif /* song_h */

