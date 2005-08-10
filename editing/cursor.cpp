#include "cursor.h"



namespace ReShaked {

int Cursor::ticks_to_snap(Tick p_ticks) {

	int snap_beats=(p_ticks/TICKS_PER_BEAT)*snap;
	int snap_in_beat=(p_ticks%TICKS_PER_BEAT)*snap/TICKS_PER_BEAT;
	return snap_beats+snap_in_beat;
}

Tick Cursor::snap_to_ticks(int p_snap) {

	Tick tick_beats=(Tick)(p_snap/snap)*(Tick)TICKS_PER_BEAT;
	Tick beat_snaps=p_snap%snap;
	Tick tick_snaps=beat_snaps*TICKS_PER_BEAT/(Tick)snap;
	return tick_beats+tick_snaps;

}

Tick Cursor::get_tick() {

	return cursor_tick;

}
void Cursor::set_tick(Tick p_tick) {

	cursor_tick=p_tick;

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
	snap=p_snap;
	window_offset=ticks_to_snap(old_window_offset_snap);
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

Cursor::Cursor() {

    cursor_tick=0;
    current_track=0;
    current_automation=0;
    edit_target=EDIT_TRACK;
    snap=4;
    window_size=5;
    window_offset=0;
}

};
