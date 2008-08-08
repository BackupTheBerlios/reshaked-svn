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

Editor *Editor::singleton=NULL;

Editor *Editor::get_singleton() {

	return singleton;
}

Track *Editor::get_current_track() const {

	if (cursor.track<0 || cursor.track >=song->get_track_count() )
		return NULL;
		
	return song->get_track(cursor.track);
}
void Editor::set_cursor_tick(Tick p_tick) {

	if (p_tick<0)
		return;
		
	cursor.tick=p_tick;
}
void Editor::set_cursor_track(int p_track) {

	if (p_track<0)
		p_track=0;
		
	if (p_track>=song->get_track_count())
		p_track=song->get_track_count()-1;
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
		
	cursor.zoom_divisor=p_divisor;
}

void Editor::set_window_offset(int p_offset) {

	if (p_offset<0)
		p_offset=0;
		
	cursor.window_offset=p_offset;
}

void Editor::set_window_rows(int p_rows) {

	if (p_rows<0)
		p_rows=0;
		
	cursor.window_rows=p_rows;
}

void Editor::set_cursor_row(int p_row) {

	cursor.tick = get_ticks_per_row() * p_row;
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


bool Editor::is_selection_active() const {

	return selection.active;
}

int Editor::get_selection_begin_track() const {

	return selection.begin.track;
}
int Editor::get_selection_begin_column() const {

	return selection.begin.col;
}
int Editor::get_selection_begin_row() const {

	return selection.begin.row;
}
int Editor::get_selection_end_track() const {

	return selection.end.track;
}
int Editor::get_selection_end_column() const {

	return selection.end.col;
}
int Editor::get_selection_end_row() const {

	return selection.end.row;
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
	
	selection.active=false;
	
	track_edit_mode=EDIT_MODE_FRONT;
	
}


Editor::~Editor()
{
}


