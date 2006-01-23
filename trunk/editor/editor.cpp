//
// C++ Implementation: editor
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



void Editor::marker_set(int p_beat,String p_text) {
	
	d->song->get_marker_list().insert(p_beat,p_text);
	
}
void Editor::marker_remove(int p_index) {
	
	d->song->get_marker_list().erase_index(p_index);
}


bool Editor::handle_navigation_key_press(BlockList *p_blocklist,int &p_event) {
	
	bool handled=true;
	
	test_key_press_selecting(p_event);
	
	SWITCH(p_event)
	
		CASE( KEYBIND("up") ) {

			begin_check_shift_selection();
			d->cursor.set_pos( d->cursor.get_pos() -1 );
			end_check_shift_selection();
			

		}
		CASE( KEYBIND("down") ) {

			begin_check_shift_selection();
			d->cursor.set_pos( d->cursor.get_pos() +1 );
			end_check_shift_selection();

		}
		CASE( KEYBIND("page_up") ) {

			begin_check_shift_selection();
			d->cursor.set_pos( d->cursor.get_pos() - d->cursor.get_window_size()/2 );
			end_check_shift_selection();

		}
		CASE( KEYBIND("page_down") ) {

			begin_check_shift_selection();
			d->cursor.set_pos( d->cursor.get_pos() + d->cursor.get_window_size()/2 );
			end_check_shift_selection();

		}
		CASE( KEYBIND("home") ) {
		
			begin_check_shift_selection();
			if (d->global_edit.current_blocklist==0) {
				d->cursor.set_pos( 0 );
			} else {
				d->global_edit.current_blocklist=0;
				enter_blocklist(EditorData::ENTER_LEFT);
			}
			end_check_shift_selection();

		}
		CASE( KEYBIND("end") ) {
		
			begin_check_shift_selection();
			d->global_edit.current_blocklist=get_blocklist_count()-1;
			enter_blocklist(EditorData::ENTER_RIGHT);
			end_check_shift_selection();
		
		}

		CASE( KEYBIND("next_track") ) {
		
			if (d->global_edit.current_blocklist<(get_blocklist_count()-1)) {
				d->global_edit.current_blocklist++;
				enter_blocklist(EditorData::ENTER_LEFT);
			}				
		
		
		}
		CASE( KEYBIND("prev_track") ) {
		
		
			if (d->global_edit.current_blocklist>0) {
				d->global_edit.current_blocklist--;
				enter_blocklist(EditorData::ENTER_RIGHT);
			}				
		
		}


		CASE( KEYBIND("global/raise_octave") ) {
		
			set_editing_octave( get_editing_octave() +1 );
		}

		CASE( KEYBIND("global/lower_octave") ) {
		
			set_editing_octave( get_editing_octave() -1 );
		}


		CASE( KEYBIND("editor/page_up_blocksnap") ) {
		
			Tick cursor_pos=d->cursor.get_tick_pos();

			int idx=p_blocklist->get_prev_block_from_idx( d->cursor.get_tick_pos() );
			if (idx==-1)
				return false;
		
			Tick begin=p_blocklist->get_block_pos(idx);
			Tick end=begin+p_blocklist->get_block(idx)->get_length();
		
			if (cursor_pos<end) { /* inside */
			
				if (cursor_pos==begin) { /* at top */
				//goto end of next block 
					if (idx>0) { //have somewhere to go?				
						cursor_pos = p_blocklist->get_block_pos(idx-1);
						cursor_pos+=p_blocklist->get_block(idx-1)->get_length();					
						cursor_pos-=(TICKS_PER_BEAT/d->cursor.get_snap());						
					}
				} else {
				
					cursor_pos=begin;	
				}
			} else {
			
				cursor_pos=end-(TICKS_PER_BEAT/d->cursor.get_snap());
			}
		
		
			d->cursor.set_from_tick_pos( cursor_pos );
		}
		CASE( KEYBIND("editor/page_down_blocksnap") ) {
		
			Tick cursor_pos=d->cursor.get_tick_pos();

			int idx=p_blocklist->get_block_idx_at_pos( cursor_pos );
			if (idx<0)
				idx=p_blocklist->get_next_block_from_idx( d->cursor.get_tick_pos() );
			if (idx==p_blocklist->get_block_count())
				return false;
		
			Tick begin=p_blocklist->get_block_pos(idx);
			Tick end=begin+p_blocklist->get_block(idx)->get_length();
			end-=(TICKS_PER_BEAT/d->cursor.get_snap());
		
			if (cursor_pos>=begin) { /* inside */
			
				if (cursor_pos==end) { /* at top */
				//goto end of next block 
					if (idx<(p_blocklist->get_block_count()-1)) { //have somewhere to go?				
						cursor_pos = p_blocklist->get_block_pos(idx+1);
					
					}
				} else {
				
					cursor_pos=end;	
				}
			} else {
			
				cursor_pos=begin;
			}
		
		
			d->cursor.set_from_tick_pos( cursor_pos );
		}
		
		CASE( KEYBIND("editor/selection_begin") ) {
		
			selection_begin();
			
		}
		CASE( KEYBIND("editor/selection_end") ) {
		
			selection_end();			
		}
		CASE( KEYBIND("editor/selection_copy") ) {
		
			selection_copy();
		}
		CASE( KEYBIND("editor/selection_paste_insert") ) {
		
			selection_paste_insert();
			printf("can paste: %i\n",selection_can_paste_at( get_cursor_selection_pos() ) );
		}
		CASE( KEYBIND("editor/selection_paste_overwrite") ) {
		
			selection_paste_overwrite();
		}
		CASE( KEYBIND("editor/selection_paste_mix") ) {
		
			selection_paste_mix();
		}
		CASE( KEYBIND("editor/select_column_block_all") ) {
		
			
		}
		CASE( KEYBIND("editor/selection_zap") ) {
		
			selection_zap();			
		}
		CASE( KEYBIND("editor/selection_disable") ) {
		
			d->selection.enabled=false;
			d->ui_update_notify->edit_window_changed();			
		}
		
		DEFAULT
			
			handled=false;
	
	END_SWITCH;
			
	return handled;
}




void Editor::revalidate_cursor() {
	
	
	if (get_blocklist_count()==0) { //special case
		
		d->global_edit.current_blocklist=0; //and that's all
	
	} else if (d->global_edit.current_blocklist>=get_blocklist_count()) {
		d->global_edit.current_blocklist=get_blocklist_count()-1;
		if (d->global_edit.current_blocklist<0)
			d->global_edit.current_blocklist=0;
		if (get_current_blocklist()==-1 || get_blocklist(get_current_blocklist())==NULL || get_blocklist(get_current_blocklist())->get_type_name()!="pattern") {
			
			d->pattern_edit.column=0;
		} else if (get_blocklist(get_current_blocklist())->get_type_name()=="pattern") {
			
			Track_Pattern *pattern_track=dynamic_cast<Track_Pattern*>(get_blocklist( get_current_blocklist() ) );
			
			ERR_FAIL_COND(pattern_track==NULL);

			if (d->pattern_edit.column>=pattern_track->get_visible_columns())
				d->pattern_edit.column=pattern_track->get_visible_columns()-1;
		}
		
		d->ui_update_notify->cursor_changed_blocklist();
	}
	
}


int Editor::find_track_idx(Track *p_track) {
	
	for (int i=0;i<d->song->get_track_count();i++) 
		if (d->song->get_track(i)==p_track)
			return i;
	
	return -1;
}

void Editor::set_editing_octave(int p_octave) {
	
	if (p_octave<0 || p_octave>8)
		return;
	
	if (d->global_edit.editing_octave==p_octave)
		return; //pointless otherwise
	d->global_edit.editing_octave=p_octave;
	d->ui_update_notify->editing_octave_changed();
}
int Editor::get_editing_octave() {
	
	return d->global_edit.editing_octave;
}

void Editor::set_snap(int p_new_snap) {
	
	d->cursor.set_snap( p_new_snap );	
}




Tick Editor::get_song_max_len() {
	Tick max_len=0;
	
	int bl_count=get_blocklist_count();
	for (int i=0;i<bl_count;i++) {
	
		int bl_max_len=get_block_list_max_len(get_blocklist(i));
		if (bl_max_len>max_len)
			max_len=bl_max_len;
	}		
	return max_len;
}

void Editor::begin_meta_undo_block(String p_name) {
	
	d->undo_stream.begin(p_name);
	
}
void Editor::end_meta_undo_block() {
	
	d->undo_stream.end();
}

void Editor::lock_undo_stream() {
	
	d->undo_stream.lock();
	
}

void Editor::unlock_undo_stream() {
	
	d->undo_stream.unlock();
	
}

void Editor::undo() {
	
	d->undo_stream.undo();
}
void Editor::redo() {
	
	d->undo_stream.redo();

}

Song* Editor::get_song() {
	
	return d->song;
}

Cursor &Editor::get_cursor() {
	
	return d->cursor;
}


UI_UpdateNotify *Editor::get_ui_update_notify() {
	
	return d->ui_update_notify;
}


void Editor::bar_length_set(int p_at_beat,int p_len) {
	
	if (p_len<2 || p_len>16)
		return; //no go!
	
	d->undo_stream.begin("Insert Bar Length");
	
	if (d->song->get_bar_map().get_bar_idx_at_beat( p_at_beat )>=0) 
		bar_length_remove( p_at_beat );
	d->undo_stream.add_command(Command2(&commands,&EditorCommands::bar_length_add,p_at_beat,p_len));
	
	d->undo_stream.end();
	
}
void Editor::bar_length_remove(int p_at_beat) {
	
	d->undo_stream.begin("Remove Bar Length");
	
	d->undo_stream.add_command(Command1(&commands,&EditorCommands::bar_length_remove,p_at_beat));
	
	d->undo_stream.end();
	
}


Editor::Editor(Song *p_song,UI_UpdateNotify *p_ui_update_notify) {
	
	d = new EditorData(p_song,p_ui_update_notify);
	commands.d=d;
	commands.editor=this;	

	d->global_edit.current_blocklist=-1;
	d->pattern_edit.column=0;
	d->pattern_edit.field=0;
	d->pattern_edit.note_edit_mode=EditorData::MODE_NOTE;
	d->global_edit.editing_octave=4;
	d->selection.enabled=false;
}


Editor::~Editor()
{
}


}
