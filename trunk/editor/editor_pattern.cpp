//
// C++ Implementation: editor_pattern
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "editor.h"
#include "engine/track_pattern.h"
#include "keyboard_input.h"
#include "pianokeys.h"


namespace ReShaked {

/* THESE HELPER MACROS NEED pattern_track DEFINED */
static Track_Pattern::Note _fix_macro(const Track_Pattern::Note &p_note) { return p_note; } //fix template annoyance
#define SET_NOTE(m_pos,m_note) d->undo_stream.add_command( Command3(&commands,&EditorCommands::set_note,pattern_track,m_pos,_fix_macro(m_note)) )
#define SET_NOTE_AT_CURSOR(m_note) SET_NOTE( Track_Pattern::Position(d->cursor.get_tick_pos(),d->pattern_edit.column),_fix_macro(m_note))



void Editor::set_pattern_note_edit_mode(EditorData::PatternNoteEditMode p_mode) {
	
	d->pattern_edit.note_edit_mode=p_mode;
}
EditorData::PatternNoteEditMode Editor::get_pattern_note_edit_mode() {
	
	return d->pattern_edit.note_edit_mode;
}

void Editor::set_pattern_edit_column(int p_column) {
	
	d->pattern_edit.column=p_column;	
}

int Editor::get_pattern_edit_column() {
	
	
	return d->pattern_edit.column;
}

void Editor::set_pattern_edit_field(int p_field) {
	
	d->pattern_edit.field=p_field;	
}
int Editor::get_pattern_edit_field() {
	
	return d->pattern_edit.field;
	
}

bool Editor::pattern_edit_key_press(int p_event) {
	
	Track_Pattern *pattern_track=dynamic_cast<Track_Pattern*>(get_blocklist( get_current_blocklist() ) );
	ERR_FAIL_COND_V(pattern_track==NULL,false);

	bool repaint=false;
	
	if (handle_navigation_key_press( pattern_track, p_event))
		return true;
	repaint=true;
	SWITCH(p_event)
			
		CASE( KEYBIND("left") ) {
		
			begin_check_shift_selection();

			if (d->pattern_edit.field==1)
				d->pattern_edit.field=0;
			else if (d->pattern_edit.column>0) {
				d->pattern_edit.column--;
				d->pattern_edit.field=1;
			} else {
					
				if (d->global_edit.current_blocklist>0) {
					d->global_edit.current_blocklist--;
					enter_blocklist(EditorData::ENTER_RIGHT);
				}				
					
			}
			end_check_shift_selection();
			//song_edit->move_editing_left();
		}
		CASE( KEYBIND("right") ) {
			
			begin_check_shift_selection();
			if (d->pattern_edit.field==0)
				d->pattern_edit.field=1;
			else if (d->pattern_edit.column<(pattern_track->get_visible_columns()-1)) {
				d->pattern_edit.column++;
				d->pattern_edit.field=0;
			}  else {
				if (d->global_edit.current_blocklist<(get_blocklist_count()-1)) {
					d->global_edit.current_blocklist++;
					enter_blocklist(EditorData::ENTER_LEFT);
				}				
			
			
			}
			end_check_shift_selection();
			//song_edit->move_editing_right();
		}
		CASE( KEYBIND("note_entry/clear_field") ) {
	
			d->undo_stream.begin("Clear Field");
			SET_NOTE_AT_CURSOR(Track_Pattern::Note(Track_Pattern::Note::NO_NOTE));
			d->undo_stream.end();
		
			d->cursor.set_pos( d->cursor.get_pos() +1 );
	
		}
		CASE( KEYBIND("note_entry/note_off") ) {
	
			d->undo_stream.begin("Note Off");
			SET_NOTE_AT_CURSOR(Track_Pattern::Note(Track_Pattern::Note::NOTE_OFF));
			d->undo_stream.end();
			d->cursor.set_pos( d->cursor.get_pos() +1 );
	
		}
	
		CASE( KEYBIND("move_note_down") ) {
	
			Tick cursor_pos=d->cursor.get_tick_pos();
			Tick next_pos=d->cursor.get_tick_pos()+d->cursor.get_snap_tick_size();
			if (!pattern_track->is_pos_editable( cursor_pos ) ||
								!pattern_track->is_pos_editable( next_pos ) )
				return false;
			Track_Pattern::Note note=pattern_track->get_note( Track_Pattern::Position( cursor_pos, d->pattern_edit.column ) );
			if (note.is_empty())
				return false;
		
			if (!pattern_track->get_note( Track_Pattern::Position( next_pos, d->pattern_edit.column ) ).is_empty() )
				return false; //cant move here
		
			d->undo_stream.begin("Move Note Down");
		
			SET_NOTE(Track_Pattern::Position( next_pos, d->pattern_edit.column) , note );
			SET_NOTE(Track_Pattern::Position( cursor_pos, d->pattern_edit.column) ,Track_Pattern::Note()  );
		
			d->undo_stream.end();
					
			d->cursor.set_pos( d->cursor.get_pos() +1 );
		
		}
		CASE( KEYBIND("move_note_up") ) {
	
			Tick cursor_pos=d->cursor.get_tick_pos();
			Tick prev_pos=d->cursor.get_tick_pos()-d->cursor.get_snap_tick_size();
			if (!pattern_track->is_pos_editable( cursor_pos ) ||
								!pattern_track->is_pos_editable( prev_pos ) )
				return false;
			Track_Pattern::Note note=pattern_track->get_note( Track_Pattern::Position( cursor_pos, d->pattern_edit.column ) );
			if (note.is_empty())
				return false;
		
			if (!pattern_track->get_note( Track_Pattern::Position( prev_pos, d->pattern_edit.column ) ).is_empty() )
				return false; //cant move here
		
			d->undo_stream.begin("Move Note Up");
		
			SET_NOTE(Track_Pattern::Position( prev_pos, d->pattern_edit.column) , note );
			SET_NOTE(Track_Pattern::Position( cursor_pos, d->pattern_edit.column) ,Track_Pattern::Note()  );
		
			d->undo_stream.end();
		
			d->cursor.set_pos( d->cursor.get_pos() -1 );
		
		}
	
		CASE( KEYBIND("editor/add_column") ) {
		
		
			d->ui_update_notify->current_track_add_column();		
		}
		
		CASE( KEYBIND("editor/remove_column") ) {
		
			d->ui_update_notify->current_track_remove_column();
		
		}
		
		CASE( KEYBIND("editor/insert") ) {
	
		
			Tick cursor_pos=d->cursor.get_tick_pos();
	
			int idx=pattern_track->get_block_idx_at_pos( cursor_pos );
			if (idx==-1)
				return false;
		
			Tick block_pos=pattern_track->get_block_pos( idx );
			Tick cursor_block_pos=cursor_pos-block_pos;
		
			Track_Pattern::PatternBlock *p = pattern_track->get_block(idx);
		
			d->undo_stream.begin("Insert");
		
			for (int i=(p->get_note_count()-1);i>=0;i--) {
			
				Track_Pattern::Position pos=p->get_note_pos(i);
				if (pos.tick<cursor_block_pos)
					continue;
				if (pos.column!=d->pattern_edit.column)
					continue;
			
				Track_Pattern::Note note=p->get_note(i);
			
			//erase
				SET_NOTE(Track_Pattern::Position( pos.tick+block_pos, pos.column), Track_Pattern::Note() );
			//set again above
				Tick new_pos=pos.tick+block_pos+d->cursor.get_snap_tick_size();
				if (pattern_track->is_pos_editable( new_pos ))
					SET_NOTE(Track_Pattern::Position( new_pos, pos.column), note );
			
			}
					
			d->undo_stream.end();
		
		}
		CASE( KEYBIND("editor/delete") ) {
	
			Tick cursor_pos=d->cursor.get_tick_pos();
	
			int idx=pattern_track->get_block_idx_at_pos( cursor_pos );
			if (idx==-1)
				return false;
		
			Tick block_pos=pattern_track->get_block_pos( idx );
			Tick cursor_block_pos=cursor_pos-block_pos;
		
			Track_Pattern::PatternBlock *p = pattern_track->get_block(idx);
		
			d->undo_stream.begin("Delete");
		
			for (int i=0;i<p->get_note_count();i++) {
			
				Track_Pattern::Position pos=p->get_note_pos(i);
				if (pos.tick<cursor_block_pos)
					continue;
				if (pos.column!=d->pattern_edit.column)
					continue;
			
				Track_Pattern::Note note=p->get_note(i);
			
			//erase
				SET_NOTE( Track_Pattern::Position( pos.tick+block_pos, pos.column), Track_Pattern::Note() );
			//set again above
				Tick new_pos=pos.tick+block_pos-d->cursor.get_snap_tick_size();
			
				if (new_pos<cursor_block_pos)
					continue;
				
				if (pattern_track->is_pos_editable( new_pos ))
					SET_NOTE( Track_Pattern::Position( new_pos, pos.column), note );
			
			
			}
		
			d->undo_stream.end();
		
		}
	
		CASE( KEYBIND("note_entry/toggle_note_edit") ) {
	
	
			switch (d->pattern_edit.note_edit_mode) {
	
				case EditorData::MODE_NOTE: d->pattern_edit.note_edit_mode=EditorData::MODE_VOLUME; break;
				case EditorData::MODE_VOLUME: d->pattern_edit.note_edit_mode=EditorData::MODE_NOTE; break;
			}
		
			d->ui_update_notify->edit_window_changed();
	
		}

		DEFAULT
		
			repaint=false;

	END_SWITCH;

	/* NOTE EDIT */
	if (pattern_track->is_pos_editable( d->cursor.get_tick_pos() ) ) {
	
		if (d->pattern_edit.field==0 && d->pattern_edit.note_edit_mode==EditorData::MODE_NOTE) {

	
			for (int i=0;i<MAX_KEYS;i++) {
				if (IS_KEYBIND((String)"note_entry/"+key_name[i],p_event)) {
					/* @TODO default volume */
					int note=d->global_edit.editing_octave*12+i;
					Tick tickpos=d->cursor.get_tick_pos();
					d->undo_stream.begin("Set Note");
					SET_NOTE(Track_Pattern::Position(tickpos,d->pattern_edit.column),Track_Pattern::Note(note,60));
					d->undo_stream.end();
					d->cursor.set_pos( d->cursor.get_pos() +1 );

				}
			}
		}

		if (d->pattern_edit.note_edit_mode==EditorData::MODE_VOLUME && Keyboard_Input::get_singleton_instance()->is_number(p_event)) {

			int number=Keyboard_Input::get_singleton_instance()->get_number(p_event);
			Tick tick=d->cursor.get_tick_pos(); //@TODO make in many notes
			Track_Pattern::Note note = pattern_track->get_note(Track_Pattern::Position(tick,d->pattern_edit.column));
			if (note.is_empty())
				return false;
			if (d->pattern_edit.field==0) {
				note.volume=(note.volume%10)+10*number;
				d->pattern_edit.field=1;
			} else {
				note.volume=(note.volume/10)*10+number;
				d->cursor.set_pos( d->cursor.get_pos() +1 );
				d->pattern_edit.field=0;
			}

			d->undo_stream.begin("Set Volume");
			SET_NOTE(Track_Pattern::Position(tick,d->pattern_edit.column),note);
			d->undo_stream.end();
	

		} else if (d->pattern_edit.note_edit_mode==EditorData::MODE_NOTE && d->pattern_edit.field==1 && Keyboard_Input::get_singleton_instance()->is_number(p_event)) {

			int octave=Keyboard_Input::get_singleton_instance()->get_number(p_event);
			Tick tick=d->cursor.get_tick_pos(); //@TODO make in many notes
			Track_Pattern::Note note = pattern_track->get_note(Track_Pattern::Position(tick,d->pattern_edit.column));
			if (note.is_empty())
				return false;
	//set octave
			note.note=note.note%12+octave*12;
			d->undo_stream.begin("Set Octave");
			SET_NOTE(Track_Pattern::Position(tick,d->pattern_edit.column),note);
			d->undo_stream.end();
			d->cursor.set_pos( d->cursor.get_pos() +1 );
		};

//d->ui_update_notify->edit_window_changed(); //there may be linked patterns, update all <- not necesary now

	}	
	
	return repaint;
}



void Editor::track_pattern_add_column(int p_which) {
	
	int track_idx=(p_which==-1)?get_current_track():p_which;
	Track_Pattern * p = dynamic_cast<Track_Pattern*>(d->song->get_track(track_idx));	
	if (!p)
		return;
	
	
	d->undo_stream.begin("Add Pattern Column");
	d->undo_stream.add_command( Command1(&commands,&EditorCommands::track_pattern_add_column,p));

	d->undo_stream.end();
	

}


void Editor::track_pattern_remove_column(int p_which) {
	
	int track_idx=(p_which==-1)?get_current_track():p_which;
	Track_Pattern * p = dynamic_cast<Track_Pattern*>(d->song->get_track(track_idx));	
	if (!p)
		return;

	d->undo_stream.begin("Remove Pattern Column");
	
	d->undo_stream.add_command( Command1(&commands,&EditorCommands::track_pattern_remove_column,p));

	d->undo_stream.end();
	
	
}


} //end of namespace
