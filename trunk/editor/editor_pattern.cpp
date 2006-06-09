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

        printf("Pattern Keybind\n");
	Track_Pattern *pattern_track=dynamic_cast<Track_Pattern*>(get_blocklist( get_current_blocklist() ) );
	ERR_FAIL_COND_V(pattern_track==NULL,false);

	bool handled=false;
	
	if (handle_navigation_key_press( pattern_track, p_event))
		return true;
	handled=true;
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
			d->ui_update_notify->notify_action( d->undo_stream.get_current_action_text() );
		
			d->cursor.set_pos( d->cursor.get_pos() + d->global_edit.cursor_step );
	
		}
		CASE( KEYBIND("note_entry/note_off") ) {
	
			d->undo_stream.begin("Note Off");
			SET_NOTE_AT_CURSOR(Track_Pattern::Note(Track_Pattern::Note::NOTE_OFF));
			d->undo_stream.end();
			d->ui_update_notify->notify_action( d->undo_stream.get_current_action_text() );
			d->cursor.set_pos( d->cursor.get_pos() + d->global_edit.cursor_step );
	
		}
	
		CASE( KEYBIND("editor/move_note_down") ) {
	
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
			d->ui_update_notify->notify_action( d->undo_stream.get_current_action_text() );
					
			d->cursor.set_pos( d->cursor.get_pos() + 1 );
		
		}
		CASE( KEYBIND("editor/move_note_up") ) {
	
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
			d->ui_update_notify->notify_action( d->undo_stream.get_current_action_text() );
		
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
		
			/* need to cache the operationsm, otherwise when deleting a note, the index will act funny */
			std::list<Track_Pattern::NoteListElement> operations;
			for (int i=(p->get_note_count()-1);i>=0;i--) {
			
				Track_Pattern::Position pos=p->get_note_pos(i);
				if (pos.tick<cursor_block_pos)
					continue;
				if (pos.column!=d->pattern_edit.column)
					continue;
			
				Track_Pattern::Note note=p->get_note(i);
			

			//erase
				Track_Pattern::NoteListElement op_clear;
				op_clear.note=Track_Pattern::Note();
				op_clear.pos=Track_Pattern::Position( pos.tick+block_pos, pos.column);
				operations.push_back(op_clear);
				
			//set again above
				Tick new_pos=pos.tick+block_pos+d->cursor.get_snap_tick_size();
				if (!pattern_track->is_pos_editable( new_pos ))
					continue;
				
				Track_Pattern::NoteListElement op_insert;
				op_insert.note=note;;
				op_insert.pos=Track_Pattern::Position( new_pos, pos.column);
				operations.push_back(op_insert);

			}
			
			foreach(I,operations) {
				
				SET_NOTE(I->pos, I->note);
			}
					
			
			d->undo_stream.end();
			d->ui_update_notify->notify_action( d->undo_stream.get_current_action_text() );
		
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
		
			/* need to cache the operationsm, otherwise when deleting a note, the index will act funny */
			std::list<Track_Pattern::NoteListElement> operations;
			for (int i=0;i<p->get_note_count();i++) {
			
				Track_Pattern::Position pos=p->get_note_pos(i);
				if (pos.tick<cursor_block_pos)
					continue;
				if (pos.column!=d->pattern_edit.column)
					continue;
			
				Track_Pattern::Note note=p->get_note(i);
			
		//erase
				Track_Pattern::NoteListElement op_clear;
				op_clear.note=Track_Pattern::Note();
				op_clear.pos=Track_Pattern::Position( pos.tick+block_pos, pos.column);
				operations.push_back(op_clear);
				
			//set again above
				Tick new_pos=pos.tick+block_pos-d->cursor.get_snap_tick_size();
			
				if (new_pos<cursor_pos)
					continue;
				
				if (!pattern_track->is_pos_editable( new_pos ))
					continue;
		
				Track_Pattern::NoteListElement op_insert;
				op_insert.note=note;;
				op_insert.pos=Track_Pattern::Position( new_pos, pos.column);
				operations.push_back(op_insert);
					
			
			}
		
			foreach(I,operations) {
				
				SET_NOTE(I->pos, I->note);
			}
			
			d->undo_stream.end();
			d->ui_update_notify->notify_action( d->undo_stream.get_current_action_text() );
		
		}
	
		
		CASE( KEYBIND("editor/transpose_up") ) {
			
			selection_cursor_transpose_up();			
		}
		CASE( KEYBIND("editor/transpose_down") ) {
			
			
			selection_cursor_transpose_down();
		}
		
		
		CASE( KEYBIND("note_entry/toggle_note_edit") ) {
	
	
			switch (d->pattern_edit.note_edit_mode) {
	
				case EditorData::MODE_NOTE: d->pattern_edit.note_edit_mode=EditorData::MODE_VOLUME; break;
				case EditorData::MODE_VOLUME: d->pattern_edit.note_edit_mode=EditorData::MODE_NOTE; break;
			}
		
			d->ui_update_notify->edit_window_changed();
	
		}

		CASE( KEYBIND("note_entry/toggle_volume_mask") ) {
		
			set_volume_mask_active( !is_volume_mask_active() );
			d->ui_update_notify->volume_mask_changed();
		}
		DEFAULT
		
			handled=false;

	END_SWITCH;

	if (d->pattern_edit.note_edit_mode==EditorData::MODE_NOTE && d->pattern_edit.field==0) {
	
		if (IS_KEYBIND("note_entry/play_row_at_cursor",p_event)) {
	
				
			if (d->pattern_edit.field==0) {
					
				for (int i=0;i<pattern_track->get_visible_columns();i++) {
						
					Track_Pattern::Note note=pattern_track->get_note( Track_Pattern::Position( d->cursor.get_tick_pos(), i ) );
					if (note.is_empty())
						continue;
					
					EventMidi em;
					em.channel=0;
					em.midi_type=note.is_note_off()?EventMidi::MIDI_NOTE_OFF:EventMidi::MIDI_NOTE_ON;
					em.data.note.note=note.note;
					em.data.note.velocity=note.is_note_off()?0:note.volume;
						
					pattern_track->add_edit_event( em, i );
				}	
					
				pattern_track->offline_process_automations( d->cursor.get_tick_pos() );
				d->cursor.set_pos( d->cursor.get_pos() + d->global_edit.cursor_step );
			}		
			handled=true;	
		} else if (IS_KEYBIND("note_entry/play_note_at_cursor",p_event) && d->pattern_edit.note_edit_mode==EditorData::MODE_NOTE) {
			if (d->pattern_edit.field==0) {
					
				play_note_at_cursor();
				d->cursor.set_pos( d->cursor.get_pos() +1 );
			}
			handled=true;	
		} else if (pattern_track->is_pos_editable( d->cursor.get_tick_pos() ) ) {
	

			for (int i=0;i<MAX_KEYS;i++) {
				if (IS_KEYBIND((String)"note_entry/"+key_name[i],p_event)) {
					/* @TODO default volume */
					int note=d->global_edit.editing_octave*12+i;
					Tick tickpos=d->cursor.get_tick_pos();
					
					
					int volume=99;
					if (d->pattern_edit.volume_mask_active) {
						volume=d->pattern_edit.volume_mask;
					} else {
						
						Track_Pattern::Note note=pattern_track->get_note(Track_Pattern::Position(tickpos,d->pattern_edit.column));
						
						if (note.is_note())
							volume=note.volume;
					}
					
					d->undo_stream.begin("Set Note");
					SET_NOTE(Track_Pattern::Position(tickpos,d->pattern_edit.column),Track_Pattern::Note(note,volume));
					d->undo_stream.end();
					play_note_at_cursor();
					d->cursor.set_pos( d->cursor.get_pos() + d->global_edit.cursor_step );
					
					if (d->pattern_edit.volume_mask_active)
						d->pattern_edit.volume_mask=volume;
					d->ui_update_notify->volume_mask_changed();
					d->ui_update_notify->notify_action( d->undo_stream.get_current_action_text() );
					break;
				}
			}
		}
	} else if (pattern_track->is_pos_editable( d->cursor.get_tick_pos() ) ) {
		
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
				d->cursor.set_pos( d->cursor.get_pos() + d->global_edit.cursor_step );
				d->pattern_edit.field=0;
			}

			d->undo_stream.begin("Set Volume");
			SET_NOTE(Track_Pattern::Position(tick,d->pattern_edit.column),note);
			d->undo_stream.end();
			d->pattern_edit.volume_mask=note.volume;
			d->ui_update_notify->volume_mask_changed();
			d->ui_update_notify->notify_action( d->undo_stream.get_current_action_text() );

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
			d->cursor.set_pos( d->cursor.get_pos() + d->global_edit.cursor_step );
			d->ui_update_notify->notify_action( d->undo_stream.get_current_action_text() );
		};

//d->ui_update_notify->edit_window_changed(); //there may be linked patterns, update all <- not necesary now

	}	
	
	return handled;
}



void Editor::track_pattern_add_column(int p_which) {
	
	int track_idx=(p_which==-1)?get_current_track():p_which;
	Track_Pattern * p = dynamic_cast<Track_Pattern*>(d->song->get_track(track_idx));	
	if (!p)
		return;
	
	
	d->undo_stream.begin("Add Pattern Column");
	d->undo_stream.add_command( Command1(&commands,&EditorCommands::track_pattern_add_column,p));

	d->undo_stream.end();
	d->ui_update_notify->notify_action( d->undo_stream.get_current_action_text() );
	

}


void Editor::track_pattern_remove_column(int p_which) {
	
	int track_idx=(p_which==-1)?get_current_track():p_which;
	Track_Pattern * p = dynamic_cast<Track_Pattern*>(d->song->get_track(track_idx));	
	if (!p)
		return;

	d->undo_stream.begin("Remove Pattern Column");
	
	d->undo_stream.add_command( Command1(&commands,&EditorCommands::track_pattern_remove_column,p));

	d->undo_stream.end();
	d->ui_update_notify->notify_action( d->undo_stream.get_current_action_text() );
	
	
}


} //end of namespace
