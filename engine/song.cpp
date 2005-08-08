
#include "song.h"


namespace ReShaked {

void Song::add_track(Track *p_track) {

    track_list.push_back(p_track);
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


};


