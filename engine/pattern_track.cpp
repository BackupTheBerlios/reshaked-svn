//
// C++ Implementation: pattern_track
//
// Description:
//
//
// Author: Juan Linietsky <coding@reduz.com.ar>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//


#include "pattern_track.h"


namespace ReShaked {

PatternNote PatternTrack::get_note(Tick p_tick,int p_column) {

	int tick_idx = pattern.get_exact_index(PatternPosition(p_tick,p_column));
	if (tick_idx==INVALID_STREAM_INDEX)
	    return PatternNote(); //return empty note, as pos in tick does not exist */
        return pattern.get_index_value(tick_idx);


}
void PatternTrack::set_note(Tick p_tick,int p_column,const PatternNote &p_note) {

    if (p_note.is_empty()) { //sets an empty note, delete it */
	int tick_idx = pattern.get_exact_index(PatternPosition(p_tick,p_column));
	if (tick_idx==INVALID_STREAM_INDEX)
	    return; /* nothing to erase */
        pattern.erase_index(tick_idx);

    } else { /* just insert-overwrite wathever you can find */
	pattern.insert(PatternPosition(p_tick,p_column),p_note);
    }

}

Pattern& PatternTrack::get_pattern() {

	return pattern;
}

void PatternTrack::process_track(AudioBuffer *p_in_buf,AudioBuffer *p_out_buf,int p_frames) {



}

int PatternTrack::get_columns() {

	return columns;
}
void PatternTrack::set_columns(int p_columns) {

	columns=p_columns;

}

PatternTrack::PatternTrack(int p_channels) : Track(p_channels) {

	columns=2;
}

};