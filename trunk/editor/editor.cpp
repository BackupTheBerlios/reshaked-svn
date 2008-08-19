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
#include "edit_commands.h"

Editor *Editor::singleton=NULL;

Editor *Editor::get_singleton() {

	return singleton;
}


void Editor::set_volume_mask_active(bool p_active) {

	insert.volume_mask_active=p_active;
}
bool Editor::is_volume_mask_active() const {

	return insert.volume_mask_active;
}
void Editor::set_volume_mask(int p_volume_mask) {

	if (p_volume_mask<0)
		p_volume_mask=0;
	if (p_volume_mask>PatternTrack::Note::MAX_VOLUME)
		p_volume_mask=PatternTrack::Note::MAX_VOLUME;
		
	insert.volume_mask=p_volume_mask;
	
}
int Editor::get_volume_mask() const {

	return insert.volume_mask;
}


/****/

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
		p_field=1;		
			
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

void Editor::validate_cursor_pos() {

	if (cursor.track>=song->get_track_count() )
		cursor.track=song->get_track_count()-1;

	if (cursor.track<0 && song->get_track_count() )
		cursor.track=0;
		
	if (cursor.track<0 || cursor.track>=song->get_track_count()) {
		UpdateNotify::get_singleton()->track_list_changed();
		return;
	}
		
	if (song->get_track(cursor.track)->is_collapsed()) {
		// inside collapsed trac, try to relocate right
		
		int new_location=-1;
	
	
		// try to the right
		for (int i=cursor.track+1;i<song->get_track_count();i++) {
		
			if (song->get_track(i)->is_collapsed())
				continue;
				
			new_location=i;
			break;
		}
		
		if (new_location==-1) {
			
			// try to the right
			for (int i=cursor.track-1;i>=0;i--) {
			
				if (song->get_track(i)->is_collapsed())
					continue;
					
				new_location=i;
				break;
			}
			
		}
		
		cursor.track=new_location;		
	}	
	
	if (cursor.track<0 || cursor.track>=song->get_track_count()) {
		UpdateNotify::get_singleton()->track_list_changed();
		return;
	}
	
	
	if (cursor.col >= song->get_track(cursor.track)->get_visible_columns())
		cursor.col=song->get_track(cursor.track)->get_visible_columns()-1;
		
	UpdateNotify::get_singleton()->track_list_changed();
		
}

/**********/

void Editor::set_track_edit_mode( TrackEditMode p_track_edit_mode ) {

	track_edit_mode=p_track_edit_mode;
}
Editor::TrackEditMode Editor::get_track_edit_mode() const {

	return track_edit_mode;
}


void Editor::set_bar_len_at_beat(int p_beat,int p_len) {

	EditCommands::get_singleton()->barbeat_set_barlen(song,p_beat,p_len); // not really much to do....
}


void Editor::set_marker_at_beat(int p_beat,String p_marker) {

	EditCommands::get_singleton()->marker_set(song,p_beat,p_marker); // not really much to do....
}


void Editor::set_loop_begin_at_cursor() {

	bool disabled=song->get_loop_begin() == song->get_loop_end();
		
	int begin = cursor.tick / TICKS_PER_BEAT;
	int end = disabled?begin+1:song->get_loop_end();
			
	if (end < begin)
		SWAP( begin, end );
		
	song->set_loop_begin( begin );
	song->set_loop_end( end );
	
	
	UpdateNotify::get_singleton()->track_list_repaint();
}


void Editor::set_loop_end_at_cursor() {

	bool disabled=song->get_loop_begin() == song->get_loop_end();
		
	if (disabled)
		return;
	
	int begin = song->get_loop_begin();
	int end = cursor.tick / TICKS_PER_BEAT + 1;
			
	if (end < begin)
		SWAP( begin, end );
		
	song->set_loop_begin( begin );
	song->set_loop_end( end );
	
	
	UpdateNotify::get_singleton()->track_list_repaint();
}

void Editor::delete_block_at_cursor() {

	if (cursor.track<0 || cursor.track>= song->get_track_count())
		return;
		
	Track *t=song->get_track(cursor.track);
	
	int idx = t->get_block_at_pos( cursor.tick );
	
	if (idx<0) 
		return;
		
	Track::Block *block = t->get_block(idx);
	
	
		
	EditCommands::get_singleton()->track_remove_block( t, block);
}

void Editor::toggle_block_repeat_at_cursor() {

	if (cursor.track<0 || cursor.track>=song->get_track_count())
		return;
		
	int idx = song->get_track( cursor.track )->find_block_at_pos( cursor.tick );
	if (idx<0)
		return;
		
	Track::Block *b=song->get_track( cursor.track )->get_block( idx );
	EditCommands::get_singleton()->track_block_toggle_repeat( b, !b->is_repeat_enabled() );
}

void Editor::resize_block_at_cursor() {

	if (cursor.track<0 || cursor.track>=song->get_track_count())
		return;
		
	int idx = song->get_track( cursor.track )->find_block_at_pos( cursor.tick );
	if (idx<0)
		return;
		
		
	Tick pos = song->get_track( cursor.track)->get_block_pos( idx );
	
	Tick new_size = cursor.tick+get_ticks_per_row()-pos;
	
	if (new_size==0)
		return;
	
	if (song->get_track( cursor.track)->is_block_motion_snapped()) {
	
		if (new_size%TICKS_PER_BEAT) {
		
			new_size-=new_size%TICKS_PER_BEAT;
			new_size+=TICKS_PER_BEAT;
		}
	}
	
	
	
	EditCommands::get_singleton()->track_resize_block( song->get_track( cursor.track), idx, new_size );

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
	
	insert.octave=4;
	insert.volume_mask_active=false;
	insert.volume_mask=99;
	
	selection.active=false;
	shift_selection.active=false;
	
	track_edit_mode=EDIT_MODE_FRONT;
	
}


Editor::~Editor()
{

	clipboard.clear();
}


