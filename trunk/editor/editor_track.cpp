//
// C++ Implementation: editor_track
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
#include "engine/pattern_track.h"
#include "key_bindings.h"
#include "update_notify.h"
#include "piano_keys.h"
#include "editor/edit_commands.h"


void Editor::pattern_set_note_at_cursor( PatternTrack::Note p_note) {

	PatternTrack *pt = dynamic_cast<PatternTrack*>( song->get_track( cursor.track ) );
	
	if (!pt)
		return;

	EditCommands::get_singleton()->pattern_set_note( pt, cursor.col, get_cursor_tick(), p_note );
}

PatternTrack::Note Editor::pattern_get_note_at_cursor() const {

	PatternTrack *pt = dynamic_cast<PatternTrack*>( song->get_track( cursor.track ) );
	
	if (!pt)
		return PatternTrack::Note();



	int blk_idx=pt->get_block_at_pos( cursor.tick );
	if (blk_idx<0)
		return PatternTrack::Note();
		
	PatternTrack::PatternBlock *block = static_cast<PatternTrack::PatternBlock*>(pt->get_block(blk_idx));
	
	if (!block)
		return PatternTrack::Note();; //pointless
		
	Tick tick = cursor.tick - pt->get_block_pos(blk_idx);

	PatternTrack::Position pos( tick, cursor.col );
	return block->get( pos );

}

bool Editor::pattern_editor_keypress( unsigned int p_code ) {

	
	if (track_edit_mode==EDIT_MODE_FRONT) {
		// front edit
			
		/* first of all, check note */

		if (cursor.field==0) {
		
			
		
			int note_entry=-1;
			for(int i=0;i<MAX_PIANO_KEYS;i++) {
			
				String keyname=String("note_entry/")+piano_key_name[i];
				
				if (p_code!=KEYBIND( keyname ) )
					continue;
					
				note_entry=insert.octave*12+i;
				break;
			
			}
			
			if (note_entry!=-1) {
			
				if (note_entry>=PatternTrack::Note::MAX_NOTES)
					return false; // completely pointless
					
				PatternTrack::Note note=pattern_get_note_at_cursor();
				
				note.note=note_entry;
				if (note.is_note() || insert.volume_mask_active)
					note.volume=insert.volume_mask;
					
				
				pattern_set_note_at_cursor( note );
				set_cursor_row( get_cursor_row() + get_cursor_step() );
				
				
				return true;			
			} 
						
		} else if (cursor.field==1) {
			// check set octave
			PatternTrack::Note note=pattern_get_note_at_cursor();
			
			if (note.is_note()) {
				
				for (int i=0;i<10;i++) {

					if (!IS_NUMBIND(i,p_code))
						continue;
					
				
					note.note=note.note%12;
					note.note+=i*12;
					pattern_set_note_at_cursor( note );
					set_cursor_row( get_cursor_row() + get_cursor_step() );
					return true;
				}						
			}		
		}		
		

	} else {
	
		PatternTrack::Note note=pattern_get_note_at_cursor();
	
		if (note.is_note()) {
			
			for (int i=0;i<10;i++) {
			

				if (!IS_NUMBIND(i,p_code))
					continue;
				
				if (cursor.field==0) {
					note.volume=note.volume%10;
					note.volume+=i*10;
					pattern_set_note_at_cursor( note );
					
					set_cursor_field(1);
				} else {
				
					note.volume-=note.volume%10;
					note.volume+=i;
					pattern_set_note_at_cursor( note );
					
					set_cursor_field(0);
					set_cursor_row( get_cursor_row() + get_cursor_step() );
				
				}
				
				return true;
			}						
		}				
	}
	
	if (p_code==KEYBIND("note_entry/note_off")) {
		
		pattern_set_note_at_cursor( PatternTrack::Note( PatternTrack::Note::NOTE_OFF) );
		set_cursor_row( get_cursor_row() + get_cursor_step() );			
		return true;
	}
	
	if (p_code==KEYBIND("note_entry/clear_field")) {
		
		pattern_set_note_at_cursor( PatternTrack::Note() );
		set_cursor_row( get_cursor_row() + get_cursor_step() );
		return true;
	}
	
	return false;
}


void Editor::cursor_move_track_left() {
		
	int prev=-1;
	
	for (int i=cursor.track-1;i>=0;i--) {
	
		if (song->get_track( i ) && !song->get_track( i )->is_collapsed()) {
		
			prev=i;
			break;
		}
	}
		
	if (prev<0)
		return; //nothing to move to
	
	set_cursor_track(prev);
	
	Track * track = song->get_track(cursor.track);
	set_cursor_col( track->get_visible_columns() );
	
	if (dynamic_cast<PatternTrack*>(track)) 
		set_cursor_field(1);
	else 
		set_cursor_field(0);
		
	
		
	
}
void Editor::cursor_move_track_right() {

	int next=-1;
	
	for (int i=cursor.track+1;i<song->get_track_count();i++) {
	
		if (song->get_track( i ) && !song->get_track( i )->is_collapsed()) {
		
			next=i;
			break;
		}
	}
		
	if (next<0)
		return; //nothing to move to
		
	set_cursor_track(next);
	set_cursor_col(0);
	set_cursor_field(0);
	
}


bool Editor::track_editor_keypress( unsigned int p_code ) {

	bool handled=true;
	
	Track * track = song->get_track( cursor.track );
	
	if (!track)
		return false;
		
	if (track->is_collapsed())
		return false;
		
	bool is_pattern=dynamic_cast<PatternTrack*>(track);
	/* handle standard keybinds first */
	
	
	/* shift select */
	
	bool shift_select_check;
	
	shift_selection_check_begin(p_code);
		
	SWITCH(p_code) 
	
		CASE( KEYBIND("up") )
			set_cursor_row( get_cursor_row() - cursor.step );
		CASE( KEYBIND("down") )
		
			set_cursor_row( get_cursor_row() + cursor.step );
		CASE( KEYBIND("left") )
			
			if (is_pattern && cursor.field>0)
				set_cursor_field( 0 );
			else if (cursor.col>0)
				set_cursor_col( cursor.col-1 );
			else if (cursor.track>0) {
				cursor_move_track_left();
				// enter new track to the left			
			}
		CASE( KEYBIND("right") )
		
			if (is_pattern && cursor.field==0)
				set_cursor_field( 1 );
			else if (cursor.col < (track->get_visible_columns()-1))
				set_cursor_col( cursor.col+1 );
			else if (cursor.track< (song->get_track_count()-1)) {
				cursor_move_track_right();
				// enter new track to the right
			}
		CASE( KEYBIND("next_track") )
		
			cursor_move_track_right();
		CASE( KEYBIND("prev_track") )
		
			cursor_move_track_left();
			set_cursor_col(0);
			set_cursor_field(0);
			
		CASE( KEYBIND("next_column") )
		
			if (cursor.col < (track->get_visible_columns()-1)) {
				set_cursor_col( cursor.col+1 );
				set_cursor_field(0);
			} else if (cursor.track< (song->get_track_count()-1)) {
				cursor_move_track_right();
				// enter new track to the right
			}
			
		CASE( KEYBIND("prev_column") )
		
			if (cursor.col>0) {
				set_cursor_col( cursor.col-1 );
				set_cursor_field(0);
			} else if (cursor.track>0) {
				cursor_move_track_left();
				set_cursor_field(0);
				// enter new track to the left			
			}		
		CASE( KEYBIND("page_up") )
		
			//Tick barlen = song->get_bar_map().get_bar_len_at_beat( cursor.tick / TICKS_PER_BEAT );
			//barlen *= TICKS_PER_BEAT;
			
			//set_cursor_tick( cursor.tick - barlen );
			set_cursor_row( get_cursor_row() - 16 );
			
		CASE( KEYBIND("page_down") )
		
//			Tick barlen = song->get_bar_map().get_bar_len_at_beat( cursor.tick / TICKS_PER_BEAT );
			//barlen *= TICKS_PER_BEAT;
			
			//set_cursor_tick( cursor.tick + barlen );
			set_cursor_row( get_cursor_row() + 16 );
			
		
		CASE( KEYBIND("home") )
		
			if (cursor.col!=0 || (is_pattern && cursor.field!=0))  {
			
				set_cursor_col(0);
				
				set_cursor_field(0);
			} else {
			
				set_cursor_track(0);
			}
			
		CASE( KEYBIND("end") )
		
			if (cursor.col!=(track->get_visible_columns()-1) || (is_pattern && cursor.field!=1))  {
			
				set_cursor_col(track->get_visible_columns()-1);
				if (is_pattern)
					set_cursor_field(1);
			} else {
			
				set_cursor_track( song->get_track_count()-1);
			}
		
		CASE( KEYBIND("editor/block_up") )
		
			int block_idx = track->find_block_at_pos( cursor.tick );
			if (block_idx<0)
				break; // no blocks above
			Tick block_tick = track->get_block_pos( block_idx );
			Tick block_len = track->get_block( block_idx )->get_length();
			
			if (cursor.tick > block_tick + block_len )
				set_cursor_tick( block_tick + block_len - get_ticks_per_row() ); 
			else if ( cursor.tick > block_tick ) 
				set_cursor_tick( block_tick ); 
			else if ( block_idx > 0 )
				set_cursor_tick( track->get_block_pos( block_idx -1 ) + track->get_block( block_idx -1 )->get_length() - get_ticks_per_row() ); 
			
		CASE( KEYBIND("editor/block_down") )
				
			int block_idx = track->find_block_at_pos( cursor.tick );
			if (block_idx>=0) {
			
				Tick block_tick = track->get_block_pos( block_idx );
				Tick block_len = track->get_block( block_idx )->get_length();
				
				if (cursor.tick < (block_tick + block_len - get_ticks_per_row()) ) {
					set_cursor_tick( block_tick + block_len - get_ticks_per_row() ); 
					break;
				}
			}
			
			if ( block_idx < (track->get_block_count()-1) )
				set_cursor_tick( track->get_block_pos( block_idx +1 ) ); 
				
		CASE( KEYBIND("editor/cursor_step_0") )
			
			cursor.step=0;
			UpdateNotify::get_singleton()->cursor_step_changed();
		CASE( KEYBIND("editor/cursor_step_1") )
			
			cursor.step=1;
			UpdateNotify::get_singleton()->cursor_step_changed();
		CASE( KEYBIND("editor/cursor_step_2") )
			
			cursor.step=2;
			UpdateNotify::get_singleton()->cursor_step_changed();
		CASE( KEYBIND("editor/cursor_step_3") )
			
			cursor.step=3;
			UpdateNotify::get_singleton()->cursor_step_changed();
		CASE( KEYBIND("editor/cursor_step_4") )
			
			cursor.step=4;
			UpdateNotify::get_singleton()->cursor_step_changed();
		CASE( KEYBIND("editor/cursor_step_5") )
			
			cursor.step=5;
			UpdateNotify::get_singleton()->cursor_step_changed();
		CASE( KEYBIND("editor/cursor_step_6") )
			
			cursor.step=6;
			UpdateNotify::get_singleton()->cursor_step_changed();
		CASE( KEYBIND("editor/cursor_step_7") )
			
			cursor.step=7;
			UpdateNotify::get_singleton()->cursor_step_changed();
		CASE( KEYBIND("editor/cursor_step_8") )
			
			cursor.step=8;
			UpdateNotify::get_singleton()->cursor_step_changed();
		CASE( KEYBIND("editor/cursor_step_9") )
			
			cursor.step=9;
			UpdateNotify::get_singleton()->cursor_step_changed();				
		CASE( KEYBIND("editor/window_snap_1") )
			
			set_cursor_zoom_divisor(0);
			
		CASE( KEYBIND("editor/window_snap_2") )
			
			set_cursor_zoom_divisor(1);
			
		CASE( KEYBIND("editor/window_snap_3") )
			
			set_cursor_zoom_divisor(2);
			
		CASE( KEYBIND("editor/window_snap_4") )
			
			set_cursor_zoom_divisor(3);
			
		CASE( KEYBIND("editor/window_snap_6") )
			
			set_cursor_zoom_divisor(4);
			
		CASE( KEYBIND("editor/window_snap_8") )
			
			set_cursor_zoom_divisor(5);
			
		CASE( KEYBIND("editor/window_snap_12") )
			
			set_cursor_zoom_divisor(6);
			
		CASE( KEYBIND("editor/window_snap_16") )
			
			set_cursor_zoom_divisor(7);
			
		CASE( KEYBIND("editor/window_snap_24") )
			
			set_cursor_zoom_divisor(8);
			
		CASE( KEYBIND("editor/window_snap_32") )
			
			set_cursor_zoom_divisor(9);
			
		CASE( KEYBIND("editor/window_snap_48") )
			
			set_cursor_zoom_divisor(10);
			
		CASE( KEYBIND("editor/window_snap_64") )
			
			set_cursor_zoom_divisor(11);
			
		CASE( KEYBIND("editor/window_snap_half") )
		
			set_cursor_zoom_divisor( cursor.zoom_divisor-2);
		
		CASE( KEYBIND("editor/window_snap_double") )
		
			set_cursor_zoom_divisor( cursor.zoom_divisor+2);
			
		CASE( KEYBIND("editor/selection_begin") )
		
			set_selection_begin_at_cursor();
				
		CASE( KEYBIND("editor/selection_end") )
		
			set_selection_end_at_cursor();
				
		CASE( KEYBIND("editor/selection_disable") )
		
			set_selection_disabled();
				
		CASE( KEYBIND("editor/select_column_block") )
		
			select_column_block_all();			
			
		CASE( KEYBIND("editor/toggle_front_back_edit") )
		
			if (track_edit_mode==EDIT_MODE_FRONT)
				track_edit_mode=EDIT_MODE_BACK;
			else
				track_edit_mode=EDIT_MODE_FRONT;

			UpdateNotify::get_singleton()->track_list_changed();
			
		CASE( KEYBIND("note_entry/toggle_volume_mask") )
			
			insert.volume_mask_active=!insert.volume_mask_active;
			UpdateNotify::get_singleton()->volume_mask_changed();
			/*
	key_bindings.add_key_bind("editor/selection_copy","Selection Copy",GUI::KEY_MASK_ALT+GUI::KEY_c);
	key_bindings.add_key_bind("editor/selection_paste_insert","Selection Paste Insert",GUI::KEY_MASK_ALT+GUI::KEY_p);
	key_bindings.add_key_bind("editor/selection_paste_overwrite","Selection Paste Overwrite",GUI::KEY_MASK_ALT+GUI::KEY_o);
	key_bindings.add_key_bind("editor/selection_paste_mix","Selection Paste Mix",GUI::KEY_MASK_ALT+GUI::KEY_m);
	key_bindings.add_key_bind("editor/selection_zap","Erase Data Under Selection",GUI::KEY_MASK_ALT+GUI::KEY_z);
			*/
		DEFAULT
		
			handled=false;
	END_SWITCH	

	shift_selection_check_end();

	
	if (handled)
		return true;

	
	
	if (dynamic_cast<PatternTrack*>(track)) {
	
		return pattern_editor_keypress(p_code);
	}
	
	return false;
}
