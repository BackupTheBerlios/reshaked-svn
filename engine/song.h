#ifndef SONG_H
#define SONG_H

#include <vector>


namespace ReShaked {

class Song {

	std::vector<Track*> track_list;
	AudioGraph track_graph;

public:
	
	void add_track(Track *p_track);
	int get_track_count();
	Track* get_track(int p_idx);
	void erase_track(int p_idx);

};


};

#endif /* song_h */