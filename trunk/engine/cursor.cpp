#include "cursor.h"



namespace ReShaked {


void Cursor::window_moved() {
	
	ui_update_notify->edit_window_changed();       
	
}
void Cursor::cursor_moved() {
	
	ui_update_notify->cursor_moved();
	
}

int Cursor::ticks_to_snap(Tick p_ticks) {

	int snap_beats=(p_ticks/TICKS_PER_BEAT)*snap;
	int snap_in_beat=(p_ticks%TICKS_PER_BEAT)*snap/TICKS_PER_BEAT;
	return snap_beats+snap_in_beat;
}

Tick Cursor::get_snap_tick_size() {
	
	return TICKS_PER_BEAT/snap;
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

	
	if (cursor_pos==p_pos)
		return;
	if (p_pos<0)
		p_pos=0;
	
	cursor_pos=p_pos;	
	
	bool window_has_moved=false;
	if (cursor_pos<window_offset) {
		window_has_moved=true;
		window_offset=cursor_pos;
	} else if (cursor_pos>=(window_offset+window_size)) {
		window_offset=cursor_pos-window_size+1;
		window_has_moved=true;
		
	}
	if (window_offset<0)
		window_offset=0;
	
	
	cursor_moved();
	if (window_has_moved)
		window_moved();
		
}

Tick Cursor::get_tick_pos() {

	return snap_to_ticks(cursor_pos);
}

Tick Cursor::get_tick_from_pos(int p_snap) {
	
	return snap_to_ticks( p_snap);	
}
void Cursor::set_from_tick_pos(Tick p_pos) {
	
	set_pos(ticks_to_snap(p_pos));
	
}


void Cursor::set_snap(Tick p_snap) {

	if (snap==p_snap)
		return;
	
	int window_offset_diff=cursor_pos-window_offset;
	
	Tick old_cursor_snap=snap_to_ticks(cursor_pos);
	snap=p_snap;
	cursor_pos=ticks_to_snap(old_cursor_snap);
	int new_window_offset=cursor_pos-window_offset_diff;
	if (window_offset!=new_window_offset) {
		
		window_offset=new_window_offset;
		window_moved();
	}
}

Tick Cursor::get_snap() {


    	return snap;

}

void Cursor::set_window_offset(int p_offset) {

	if (window_offset==p_offset)
		return;
 	window_offset=p_offset;
	window_moved();
}

int Cursor::get_window_offset() {

	return window_offset;
}
void Cursor::set_window_size(int p_size) {

	if (window_size==p_size)
		return;
	window_size=p_size;
	window_moved();
	
}
int Cursor::get_window_size() {

	return window_size;
}

Tick Cursor::get_snapped_window_tick_pos(int p_pos) {

	return snap_to_ticks(window_offset+p_pos);
}
int Cursor::get_snapped_window_beat(int p_pos) {
	
	return (window_offset+p_pos)/snap;
}
int Cursor::get_snapped_window_subsnap(int p_pos) {
	
	return (window_offset+p_pos)%snap;
	
}

int Cursor::get_beat() {
	
	
	return cursor_pos/snap;
}


Cursor::~Cursor() {
	
	
}
Cursor::Cursor(UI_UpdateNotify *p_ui_update_notify) {

	ui_update_notify=p_ui_update_notify;
	cursor_pos=0;
	snap=4;
	window_size=5;
	window_offset=0;
}

};
