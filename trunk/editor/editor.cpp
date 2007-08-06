////
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
	d->ui_update_notify->track_list_changed();
	
}
void Editor::marker_remove(int p_index) {
	
	if (p_index<0 || p_index>=d->song->get_marker_list().get_stream_size())
		return;
	
	d->song->get_marker_list().erase_index(p_index);
}


bool Editor::handle_navigation_key_press(BlockList *p_blocklist,int &p_event) {
	
	bool handled=true;
	
	test_key_press_selecting(p_event);
	
	SWITCH(p_event)
	
		CASE( KEYBIND("up") ) {

			begin_check_shift_selection();
			d->cursor.set_pos( d->cursor.get_pos() - d->global_edit.cursor_step );
			end_check_shift_selection();
			

		}
		CASE( KEYBIND("down") ) {

			begin_check_shift_selection();
			d->cursor.set_pos( d->cursor.get_pos() + d->global_edit.cursor_step );
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

		CASE( KEYBIND("next_column") ) {
		
			
			if (d->pattern_edit.column<(get_current_track_columns()-1)) {
				d->pattern_edit.column++;
				d->pattern_edit.field=0;
					
			} else if (d->global_edit.current_blocklist<(get_blocklist_count()-1)) {
					
				d->global_edit.current_blocklist++;
				enter_blocklist(EditorData::ENTER_LEFT);
					
			}			
		
		}
		CASE( KEYBIND("prev_column") ) {
		
		
			if (d->pattern_edit.column>0 && d->pattern_edit.column<get_current_track_columns()) {
				d->pattern_edit.column--;
				
				
			} else if (d->global_edit.current_blocklist>0) {
				
				d->global_edit.current_blocklist--;
				enter_blocklist(EditorData::ENTER_RIGHT);
			}				
		
			d->pattern_edit.field=0;
		}
		
		CASE( KEYBIND("editor/next_marker")) {
				
			int beat_count=d->song->get_marker_list().get_stream_size();
			int beat=d->cursor.get_beat();
			int marker_idx=d->song->get_marker_list().get_next_index( beat );
			if (marker_idx>=beat_count)
				break;
			
			if (d->song->get_marker_list().get_index_pos( marker_idx ) == beat )
				marker_idx++;
			
			if (marker_idx>=beat_count)
				break;
			
			d->cursor.set_pos( d->song->get_marker_list().get_index_pos( marker_idx ) * d->cursor.get_snap() );
			d->cursor.set_window_offset( d->cursor.get_pos() - d->cursor.get_window_size()/2 );
			
			
		}
	

		CASE( KEYBIND("editor/previous_marker")) {
			
			int beat=d->cursor.get_beat();
			int marker_idx=d->song->get_marker_list().get_prev_index( beat );
			if (marker_idx<0)
				break;
			
			if (d->song->get_marker_list().get_index_pos( marker_idx ) == beat )
				marker_idx--;
			
			if (marker_idx<0)
				break;
			
			d->cursor.set_pos( d->song->get_marker_list().get_index_pos( marker_idx ) * d->cursor.get_snap() );
			d->cursor.set_window_offset( d->cursor.get_pos() - d->cursor.get_window_size()/2 );

			
			
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
		CASE( KEYBIND("editor/select_column_block") ) {
		
			selection_column_all();			
		}
		CASE( KEYBIND("editor/selection_zap") ) {
		
			selection_zap();			
		}
		CASE( KEYBIND("editor/selection_disable") ) {
		
			d->selection.enabled=false;
			d->ui_update_notify->edit_window_changed();			
		}
		
		CASE( KEYBIND("editor/edit_marker") ) {
		
			d->ui_update_notify->editor_marker_edit_request();
		}
		CASE( KEYBIND("editor/set_loop_begin") ) {
		
			set_loop_begin_at_cursor();
		}
		CASE( KEYBIND("editor/set_loop_end") ) {
		
			set_loop_end_at_cursor();
		}
		CASE( KEYBIND("editor/selection_to_loop") ) {
		
			selection_to_loop();
			
		}
		CASE( KEYBIND("editor/selection_apply_volume_mask") ) {
		
			selection_set_volumes_to_mask();			
		}
		CASE( KEYBIND("editor/selection_scale_volumes") ) {
		
			d->ui_update_notify->editor_volume_scale_request();			
		}
		
		CASE( KEYBIND("editor/selection_create_blocks") ) {
		
			selection_create_block();
		}		
		
		CASE( KEYBIND("editor/toggle_block_repeat") ) {
		
			int bl_idx=get_current_blocklist();
			if (bl_idx<0)
				break;
			
			BlockList *bl = get_blocklist( bl_idx );
			
			if (bl==NULL)
				break;
			
			BlockList::Block *blk=bl->get_block_at_pos( d->cursor.get_tick_pos() );
			
			if (blk==NULL)
				break;
			
			d->undo_stream.begin("Toggle Block Repeat");
			d->undo_stream.add_command( Command2(&commands,&EditorCommands::blocklist_set_block_repeat,blk,!blk->is_repeat_active()));
			d->undo_stream.end();
			d->ui_update_notify->notify_action( d->undo_stream.get_current_action_text() );
			
				
		}		

		CASE( KEYBIND("note_entry/cursor_step_0") ) { cursor_set_step(0); }
		CASE( KEYBIND("note_entry/cursor_step_1") ) { cursor_set_step(1); }
		CASE( KEYBIND("note_entry/cursor_step_2") ) { cursor_set_step(2); }
		CASE( KEYBIND("note_entry/cursor_step_3") ) { cursor_set_step(3); }
		CASE( KEYBIND("note_entry/cursor_step_4") ) { cursor_set_step(4); }
		CASE( KEYBIND("note_entry/cursor_step_5") ) { cursor_set_step(5); }
		CASE( KEYBIND("note_entry/cursor_step_6") ) { cursor_set_step(6); }
		CASE( KEYBIND("note_entry/cursor_step_7") ) { cursor_set_step(7); }
		CASE( KEYBIND("note_entry/cursor_step_8") ) { cursor_set_step(8); }
		CASE( KEYBIND("note_entry/cursor_step_9") ) { cursor_set_step(9); }
		
		CASE( KEYBIND("editor/window_snap_1") ) { set_snap(1); }
		CASE( KEYBIND("editor/window_snap_2") ) { set_snap(2); }
		CASE( KEYBIND("editor/window_snap_3") ) { set_snap(3); }
		CASE( KEYBIND("editor/window_snap_4") ) { set_snap(4); }
		CASE( KEYBIND("editor/window_snap_6") ) { set_snap(6); }
		CASE( KEYBIND("editor/window_snap_8") ) { set_snap(8); }
		CASE( KEYBIND("editor/window_snap_12") ) { set_snap(12); }
		CASE( KEYBIND("editor/window_snap_16") ) { set_snap(16); }
		CASE( KEYBIND("editor/window_snap_24") ) { set_snap(24); }
		CASE( KEYBIND("editor/window_snap_32") ) { set_snap(32); }
		CASE( KEYBIND("editor/window_snap_48") ) { set_snap(48); }
		CASE( KEYBIND("editor/window_snap_64") ) { set_snap(64); }

		CASE( KEYBIND("editor/window_snap_half") ) { 
			
			int current_snap=d->cursor.get_snap();
			
			if (current_snap!=1 && current_snap !=3) {
				
				current_snap/=2;
				set_snap(current_snap);
			}
			
			
		}
		
		CASE( KEYBIND("editor/window_snap_double") ) { 
			
			int current_snap=d->cursor.get_snap();
			
			if (current_snap!=48 && current_snap !=64) {
				
				current_snap*=2;
				set_snap(current_snap);
			}
			
		}
		
		CASE( KEYBIND("editor/quantize_up") ) { 
			
			selection_quantize( QUANTIZE_UP );
		}
		CASE( KEYBIND("editor/quantize_nearest") ) { 
			
			selection_quantize( QUANTIZE_NEAREST );
		}
		CASE( KEYBIND("editor/quantize_down") ) { 
			
			selection_quantize( QUANTIZE_DOWN );
		}
		
		CASE( KEYBIND("editor/track_mute")) {
			
			int track_idx=get_current_track();
			if (track_idx==-1)
				break;
			Track *t = d->song->get_track( track_idx );
			
			set_track_mute(t,!t->is_mute());
		}
		
		CASE( KEYBIND("editor/track_solo")) {
			
			int track_idx=get_current_track();
			if (track_idx==-1)
				break;
		
			set_track_solo(track_idx);
	
		}
		
		DEFAULT
			
			handled=false;
	
	END_SWITCH;
			
	return handled;
}

void Editor::blocklist_block_set_repeat(BlockList::Block *p_block,bool p_repeat) {
	
	d->undo_stream.begin("Toggle Block Repeat");
	d->undo_stream.add_command( Command2(&commands,&EditorCommands::blocklist_set_block_repeat,p_block,p_repeat));
	d->undo_stream.end();
	d->ui_update_notify->notify_action( d->undo_stream.get_current_action_text() );
		
	
}

int Editor::get_current_track_columns() {
	
	BlockList *bl=get_blocklist( get_current_blocklist() );
	if (dynamic_cast<Track_Pattern*>(bl)) {
		
		return dynamic_cast<Track_Pattern*>(bl)->get_visible_columns();
	}
	
	return 1;
}

void Editor::revalidate_cursor() {
	
	
	if (get_blocklist_count()==0) { //special case
		
		d->global_edit.current_blocklist=0; //and that's all
	
	} else {
		
		if (d->global_edit.current_blocklist>=get_blocklist_count()) 
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
	d->ui_update_notify->cursor_step_changed();
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
	d->ui_update_notify->notify_action( d->undo_stream.get_current_action_text() );
}

void Editor::lock_undo_stream() {
	
	d->undo_stream.lock();
	
}

void Editor::unlock_undo_stream() {
	
	d->undo_stream.unlock();
	d->ui_update_notify->notify_action( d->undo_stream.get_current_action_text() );
	
}

void Editor::undo() {
	
	d->ui_update_notify->notify_action( "UNDO: "+d->undo_stream.get_current_action_text() );
	d->undo_stream.undo();
}
void Editor::redo() {
	
	d->undo_stream.redo();
	d->ui_update_notify->notify_action( "REDO: "+d->undo_stream.get_current_action_text() );

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


void Editor::play_note_at_cursor(int p_custom_column) {
	
	if (p_custom_column==-1)
		p_custom_column=d->pattern_edit.column;
	
	int ti=get_current_track();
	if (ti<0)
		return;
	
	Track * track=d->song->get_track(ti);
	if (!track)
		return;
	
	Track_Pattern *pattern_track=dynamic_cast<Track_Pattern*>(track);
	if (!pattern_track)
		return;
	
	Track_Pattern::Note note=pattern_track->get_note( Track_Pattern::Position( d->cursor.get_tick_pos(), p_custom_column ) );
	if (note.is_empty())
		return;
					
	EventMidi em;
	em.channel=0;
	em.midi_type=note.is_note_off()?EventMidi::MIDI_NOTE_OFF:EventMidi::MIDI_NOTE_ON;
	em.data.note.note=note.note;
	em.data.note.velocity=note.is_note_off()?0:note.volume;
						
	pattern_track->add_edit_event( em, p_custom_column );
	pattern_track->offline_process_automations( d->cursor.get_tick_pos() );
	
	
	
}

void Editor::bar_length_set(int p_at_beat,int p_len) {
	
	if (p_len<2 || p_len>16)
		return; //no go!
	
	d->undo_stream.begin("Insert Bar Length");
	
	if (d->song->get_bar_map().get_bar_idx_at_beat( p_at_beat )>=0) 
		bar_length_remove( p_at_beat );
	d->undo_stream.add_command(Command2(&commands,&EditorCommands::bar_length_add,p_at_beat,p_len));
	
	d->undo_stream.end();
	d->ui_update_notify->notify_action( d->undo_stream.get_current_action_text() );
	
}
void Editor::bar_length_remove(int p_at_beat) {
	
	d->undo_stream.begin("Remove Bar Length");
	
	d->undo_stream.add_command(Command1(&commands,&EditorCommands::bar_length_remove,p_at_beat));
	
	d->undo_stream.end();
	d->ui_update_notify->notify_action( d->undo_stream.get_current_action_text() );
	
}


int Editor::get_volume_mask() {
	
	return d->pattern_edit.volume_mask;
}
void Editor::set_volume_mask(int p_val) {
	ERR_FAIL_INDEX(p_val,100);
	d->pattern_edit.volume_mask=p_val;
}
bool Editor::is_volume_mask_active() {
	
	return d->pattern_edit.volume_mask_active;
}
void Editor::set_volume_mask_active(bool p_active){ 
	
	d->pattern_edit.volume_mask_active=p_active;
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

void Editor::reset() {
	
	d->cursor.set_pos(0);
	d->cursor.set_window_offset(0);
	d->global_edit.current_blocklist=0;
	d->pattern_edit.column=0;
	d->pattern_edit.field=0;
	d->selection.enabled=false;
	d->selection.data.clear();
	d->undo_stream.clear();
}

void Editor::set_pattern_midi_input_enabled(bool p_enabled) {
	
	
	d->global_edit.pattern_midi_input_enabled=p_enabled;	
}

bool Editor::is_pattern_midi_input_enabled() {
	
	return d->global_edit.pattern_midi_input_enabled;
}


void Editor::set_current_rack_track(int p_track) {
	
	d->global_edit.current_rack_track=p_track;
}
int Editor::get_current_rack_track() {
	
	return d->global_edit.current_rack_track;
}

void Editor::cursor_set_step(int p_step) {
	
	d->global_edit.cursor_step=p_step;
	d->ui_update_notify->cursor_step_changed();
}
int Editor::cursor_get_step() {
	
	return d->global_edit.cursor_step;
}

void Editor::set_loop_begin_at_cursor() {
	
	int beat=get_cursor().get_tick_pos()/TICKS_PER_BEAT;
	if ( get_song()->get_loop_end() < beat )
		get_song()->set_loop_end( beat+1 );
			
	get_song()->set_loop_begin( beat );
	
	d->ui_update_notify->edit_window_changed();

}
void Editor::set_loop_end_at_cursor() {
	
	int beat=get_cursor().get_tick_pos()/TICKS_PER_BEAT;
	if ( get_song()->get_loop_begin() < beat )
		get_song()->set_loop_end( beat );
			
	get_song()->set_loop_end( beat+1 );
	d->ui_update_notify->edit_window_changed();
	
}


Editor::~Editor() {
	
	delete d;
}


}
