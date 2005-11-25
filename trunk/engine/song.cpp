
#include "song.h"
#include "track_pattern.h"

namespace ReShaked {

BarMap& Song::get_bar_map() {
	
	
}

void Song::add_track(Track* p_track) {
	
	track_list.push_back(p_track);
	p_track->set_automation_pool(automation_pool);
	
}
DataPool *Song::get_pattern_pool() {
	
	return pattern_pool;
}
DataPool *Song::get_automation_pool() {
	
	return automation_pool;
	
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
	
	pattern_pool = new DataPool;	
	automation_pool = new DataPool;	
}

Song::~Song() {
	
	delete pattern_pool;
	delete automation_pool;
}

};
