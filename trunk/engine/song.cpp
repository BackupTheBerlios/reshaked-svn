
#include "song.h"
#include "track_pattern.h"

namespace ReShaked {


Cursor& Song::get_cursor() {
	
	return cursor;	
}

void Song::add_track(TrackType p_type,int p_channels) {

	switch (p_type) {
		
		case TRACK_TYPE_PATTERN: {
			
			
			track_list.push_back(new Track_Pattern(p_channels,pattern_pool));
			track_list[track_list.size()-1]->set_automation_pool(automation_pool);
			track_list[track_list.size()-1]->create_block(0);
			track_list[track_list.size()-1]->add_automation("hoe");
			track_list[track_list.size()-1]->get_automation(0)->create_block(TICKS_PER_BEAT*4);
		} break;
	}
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


Song::Song() {
	
	pattern_pool = new DataPool;	
	automation_pool = new DataPool;	
}

Song::~Song() {
	
	delete pattern_pool;
	delete automation_pool;
}

};
