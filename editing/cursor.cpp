#include "cursor.h"



namespace ReShaked {

int Cursor::ticks_to_snap(Tick p_ticks) {

	int snap_beats=(p_ticks/TICKS_PER_BEAT)*snap;
	int snap_in_beat=(p_ticks%TICKS_PER_BEAT)*snap/TICKS_PER_BEAT;
	return snap_beats+snap_in_beat;
}

Tick Cursor::snap_to_ticks(int p_snap) {

	//printf("snap %i - ",p_snap);
	Tick tick_beats=(Tick)(p_snap/snap)*(Tick)TICKS_PER_BEAT;
	Tick beat_snaps=p_snap%snap;
	Tick tick_snaps=beat_snaps*TICKS_PER_BEAT/(Tick)snap;
	Tick res=tick_beats+tick_snaps;
	//printf("tick %i\n",(int)res);
	return res;

}

int Cursor::get_pos() {

	return cursor_pos;
}

void Cursor::set_pos(int p_pos) {

	if (p_pos<0)
		p_pos=0;
	cursor_pos=p_pos;	
}

Tick Cursor::get_tick_pos() {

	return snap_to_ticks(cursor_pos);
}


int Cursor::get_track() {

	return current_track;
}
void Cursor::set_track(int p_track) {


	current_track=p_track;
}
int Cursor::get_track_automation() {

	return current_automation;

}
void Cursor::set_edited_track_automation(int p_track_automation) {

	current_automation=p_track_automation;

}
void Cursor::set_snap(Tick p_snap) {

	Tick old_window_offset_snap=snap_to_ticks(window_offset);
	Tick old_cursor_snap=snap_to_ticks(cursor_pos);
	snap=p_snap;
	window_offset=ticks_to_snap(old_window_offset_snap);
	cursor_pos=ticks_to_snap(old_cursor_snap);
}

Tick Cursor::get_snap() {


    	return snap;

}

void Cursor::set_window_offset(int p_offset) {


 	window_offset=p_offset;
}

int Cursor::get_window_offset() {

	return window_offset;
}
void Cursor::set_window_size(int p_size) {

	window_size=p_size;
}
int Cursor::get_window_size() {

	return window_size;
}

Tick Cursor::get_snapped_window_tick_pos(int p_pos) {

	return snap_to_ticks(window_offset+p_pos);
}

Cursor::TrackEditTarget Cursor::get_edit_target() {


	return edit_target;
}

Cursor::Cursor() {

    cursor_pos=0;
    current_track=0;
    current_automation=0;
    edit_target=EDIT_TRACK;
    snap=4;
    window_size=5;
    window_offset=0;
}

};
