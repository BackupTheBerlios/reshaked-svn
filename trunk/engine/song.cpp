
#include "song.h"
#include "track_pattern.h"

namespace ReShaked {

BarMap& Song::get_bar_map() {
	
	return bar_map;	
}

void Song::add_track(Track* p_track,int p_at_pos) {
	
	if (p_at_pos==-1)
		p_at_pos=track_list.size();
	
	track_list.insert(track_list.begin()+p_at_pos,p_track);

	
}

int Song::get_track_count() {

	return track_list.size();
}

Track* Song::get_track(int p_idx) {

	ERR_FAIL_INDEX_V(p_idx,track_list.size(),NULL);
	return track_list[p_idx];
}

void Song::remove_track(int p_idx) { /* remove but dont delete */

	ERR_FAIL_INDEX(p_idx,track_list.size());
	track_list.erase( track_list.begin() + p_idx );
	

};


Song::Song()  {
	
}

Song::~Song() {
	
}

};
