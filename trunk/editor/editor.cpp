//
// C++ Implementation: editor
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "editor.h"
#include "update_notify.h"

Editor *Editor::singleton=NULL;

Editor *Editor::get_singleton() {

	return singleton;
}


void Editor::adjust_window_offset() {

	if (get_cursor_row() < get_window_offset())
		set_window_offset( get_cursor_row() ); 
		
	if (get_cursor_row() >= get_window_offset() + get_window_rows() )
		set_window_offset( get_cursor_row() - get_window_rows() + 1 );

}

Track *Editor::get_current_track() const {

	if (cursor.track<0 || cursor.track >=song->get_track_count() )
		return NULL;
		
	return song->get_track(cursor.track);
}
void Editor::set_cursor_tick(Tick p_tick) {

	if (p_tick<0)
		p_tick=0;
		
	cursor.tick=p_tick;
	
	adjust_window_offset();
}
void Editor::set_cursor_track(int p_track) {

	if (p_track<0)
		p_track=0;
		
	if (p_track>=song->get_track_count())
		p_track=song->get_track_count()-1;
		
	int prev=cursor.track;
	cursor.track=p_track;
	
	if (prev!=cursor.track)
		UpdateNotify::get_singleton()->cursor_track_changed(prev,cursor.track);
}
void Editor::set_cursor_col(int p_col) {

	if (p_col<0)
		p_col=0;
		
	Track *t = get_current_track();
	
	if (!t)
		return;
		
	if (p_col>=t->get_visible_columns())
		p_col=t->get_visible_columns()-1;
	
	cursor.col=p_col;
}

void Editor::set_cursor_field(int p_field) {

	if (p_field<0)
		p_field=0;
	if (p_field>1)
		p_field>1;		
			
	cursor.field=p_field;
}


void Editor::set_cursor_zoom_divisor(int p_divisor) {

	if (p_divisor<0)
		p_divisor=0;
		
	if (p_divisor>=MAX_DIVISORS)
		p_divisor=MAX_DIVISORS-1;
		
	// hmm cursor should always be centered when doing this..
	// so save where the window was, relative to the window offset row		
	int offset_to_window=get_cursor_row()-get_window_offset();
	
	// then assign the divisor
	cursor.zoom_divisor=p_divisor;
	
	// and reassign the window offset
	set_window_offset( get_cursor_row()-offset_to_window );

	UpdateNotify::get_singleton()->window_snap_changed();
}

void Editor::set_window_offset(int p_offset) {

	if (p_offset<0)
		p_offset=0;
		
	if (p_offset==cursor.window_offset)
		return;
		
	cursor.window_offset=p_offset;
	adjust_window_offset();
	
	UpdateNotify::get_singleton()->window_offset_changed();
	
}

void Editor::set_window_rows(int p_rows) {

	if (p_rows<0)
		p_rows=0;
		
	cursor.window_rows=p_rows;
	adjust_window_offset();
}

void Editor::set_cursor_row(int p_row) {

	set_cursor_tick( get_ticks_per_row() * p_row );
}

void Editor::set_cursor_step(int p_step) {

	if (p_step<0)
		p_step=0;
	cursor.step=p_step;
	UpdateNotify::get_singleton()->cursor_step_changed();				
}

Tick Editor::get_cursor_tick() const {

	return cursor.tick;
}
int Editor::get_cursor_track() const {

	return cursor.track;
}
int Editor::get_cursor_col() const {

	return cursor.col;
}
int Editor::get_cursor_field() const {

	return cursor.field;
}
int Editor::get_cursor_zoom_divisor() const {

	return cursor.zoom_divisor;
}
int Editor::get_window_offset() const {

	return cursor.window_offset;
}

int Editor::get_window_rows() const {

	return cursor.window_rows;
}

int64 Editor::get_cursor_row() const {

	return cursor.tick / get_ticks_per_row();
}

int Editor::get_cursor_step() const {

	return cursor.step;
}

int64 Editor::get_ticks_per_row() const {

	return TICKS_PER_BEAT / beat_divisors[cursor.zoom_divisor];
}

int64 Editor::get_row_ticks(int p_row) const {

	return p_row * get_ticks_per_row();
}

int64 Editor::get_window_offset_ticks() const {

	return cursor.window_offset * get_ticks_per_row();
}

/**********/

void Editor::set_track_edit_mode( TrackEditMode p_track_edit_mode ) {

	track_edit_mode=p_track_edit_mode;
}
Editor::TrackEditMode Editor::get_track_edit_mode() const {

	return track_edit_mode;
}



Editor::Editor(Song *p_song) {
	
	singleton=this;
	song=p_song;
	
	cursor.tick=0;
	cursor.track=-1;
	cursor.zoom_divisor=3;
	cursor.col=0;
	cursor.field=0;
	cursor.window_offset=0;
	cursor.window_rows=0;
	cursor.step=1;
	
	selection.active=false;
	shift_selection.active=false;
	
	track_edit_mode=EDIT_MODE_FRONT;
	
}


Editor::~Editor()
{
}


