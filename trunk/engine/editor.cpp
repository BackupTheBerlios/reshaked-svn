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


Cursor &Editor::get_cursor() {
	
	return cursor;
}

void Editor::set_pattern_note_edit_mode(PatternNoteEditMode p_mode) {
	
	pattern_edit.note_edit_mode=p_mode;
}
Editor::PatternNoteEditMode Editor::get_pattern_note_edit_mode() {
	
	return pattern_edit.note_edit_mode;
}

void Editor::set_pattern_edit_column(int p_column) {
	
	pattern_edit.column=p_column;	
}

int Editor::get_pattern_edit_column() {
	
	
	return pattern_edit.column;
}

void Editor::set_pattern_edit_field(int p_field) {
	
	pattern_edit.field=p_field;	
}
int Editor::get_pattern_edit_field() {
	
	return pattern_edit.field;
	
}

void Editor::enter_blocklist(EnterBLDir p_dir) {
	
	BlockList* bl = get_blocklist(global_edit.current_blocklist);
	ERR_FAIL_COND(bl == NULL);
	if (bl->get_type_name()=="pattern") {
		
		Track_Pattern *tp=dynamic_cast<Track_Pattern*>(bl);
		ERR_FAIL_COND(tp==NULL);
		if (p_dir==ENTER_LEFT) {
				
			pattern_edit.column=0;
			pattern_edit.field=0;
			
		} else {
			
			pattern_edit.column=tp->get_visible_columns()-1;
			pattern_edit.field=1;
		}
	}

	ui_update_notify->cursor_changed_blocklist();
}

bool Editor::automation_edit_key_press(int p_key_value) {
	
	Automation *automation=dynamic_cast<Automation*>(get_blocklist( get_current_blocklist() ) );
	ERR_FAIL_COND_V(automation==NULL,false);
	
	if (handle_navigation_key_press( automation, p_key_value))
		return true;
	bool handled=true;
	
	SWITCH(p_key_value)
			
		CASE( KEYBIND("left") ) {

			if (global_edit.current_blocklist>0) {
				global_edit.current_blocklist--;
				enter_blocklist(ENTER_RIGHT);
			}				
		}			
		CASE( KEYBIND("right") ) {
			if (global_edit.current_blocklist<(get_blocklist_count()-1)) {
				global_edit.current_blocklist++;
				enter_blocklist(ENTER_LEFT);
			}				
		}
		DEFAULT
		
			handled=false;
	
		
	END_SWITCH;			
	return handled;	
}

bool Editor::handle_navigation_key_press(BlockList *p_blocklist,int p_event) {
	
	bool handled=true;
	SWITCH(p_event)
	
		CASE( KEYBIND("up") ) {

			cursor.set_pos( cursor.get_pos() -1 );

		}
		CASE( KEYBIND("down") ) {

			cursor.set_pos( cursor.get_pos() +1 );

		}
		CASE( KEYBIND("page_up") ) {

			cursor.set_pos( cursor.get_pos() - cursor.get_window_size()/2 );

		}
		CASE( KEYBIND("page_down") ) {

			cursor.set_pos( cursor.get_pos() + cursor.get_window_size()/2 );

		}
		CASE( KEYBIND("home") ) {
		
			if (global_edit.current_blocklist==0) {
				cursor.set_pos( 0 );
			} else {
				global_edit.current_blocklist=0;
				enter_blocklist(ENTER_LEFT);
			}

		}
		CASE( KEYBIND("end") ) {
		
			global_edit.current_blocklist=get_blocklist_count()-1;
			enter_blocklist(ENTER_RIGHT);
		
		}

		CASE( KEYBIND("next_track") ) {
		
			if (global_edit.current_blocklist<(get_blocklist_count()-1)) {
				global_edit.current_blocklist++;
				enter_blocklist(ENTER_LEFT);
			}				
		
		
		}
		CASE( KEYBIND("prev_track") ) {
		
		
			if (global_edit.current_blocklist>0) {
				global_edit.current_blocklist--;
				enter_blocklist(ENTER_RIGHT);
			}				
		
		}


		CASE( KEYBIND("global/raise_octave") ) {
		
			set_editing_octave( get_editing_octave() +1 );
		}

		CASE( KEYBIND("global/lower_octave") ) {
		
			set_editing_octave( get_editing_octave() -1 );
		}


		CASE( KEYBIND("page_up_blocksnap") ) {
		
			Tick cursor_pos=cursor.get_tick_pos();

			int idx=p_blocklist->get_prev_block_from_idx( cursor.get_tick_pos() );
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
						cursor_pos-=(TICKS_PER_BEAT/cursor.get_snap());						
					}
				} else {
				
					cursor_pos=begin;	
				}
			} else {
			
				cursor_pos=end-(TICKS_PER_BEAT/cursor.get_snap());
			}
		
		
			cursor.set_from_tick_pos( cursor_pos );
		}
		CASE( KEYBIND("page_down_blocksnap") ) {
		
			Tick cursor_pos=cursor.get_tick_pos();

			int idx=p_blocklist->get_block_idx_at_pos( cursor_pos );
			if (idx<0)
				idx=p_blocklist->get_next_block_from_idx( cursor.get_tick_pos() );
			if (idx==p_blocklist->get_block_count())
				return false;
		
			Tick begin=p_blocklist->get_block_pos(idx);
			Tick end=begin+p_blocklist->get_block(idx)->get_length();
			end-=(TICKS_PER_BEAT/cursor.get_snap());
		
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
		
		
			cursor.set_from_tick_pos( cursor_pos );
		}
		
		DEFAULT
			
			handled=false;
	
	END_SWITCH;
			
	return handled;
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

			if (pattern_edit.field==1)
				pattern_edit.field=0;
			else if (pattern_edit.column>0) {
				pattern_edit.column--;
				pattern_edit.field=1;
			} else {
				
				if (global_edit.current_blocklist>0) {
					global_edit.current_blocklist--;
					enter_blocklist(ENTER_RIGHT);
				}				
				
			}
				//song_edit->move_editing_left();
		}
		CASE( KEYBIND("right") ) {
			if (pattern_edit.field==0)
				pattern_edit.field=1;
			else if (pattern_edit.column<(pattern_track->get_visible_columns()-1)) {
				pattern_edit.column++;
				pattern_edit.field=0;
			}  else {
				if (global_edit.current_blocklist<(get_blocklist_count()-1)) {
					global_edit.current_blocklist++;
					enter_blocklist(ENTER_LEFT);
				}				
				
				
			}
				//song_edit->move_editing_right();
		}
		CASE( KEYBIND("note_entry/clear_field") ) {

			pattern_track->set_note(Track_Pattern::Position(cursor.get_tick_pos(),pattern_edit.column),Track_Pattern::Note(Track_Pattern::Note::NO_NOTE));
			cursor.set_pos( cursor.get_pos() +1 );

		}
		CASE( KEYBIND("note_entry/note_off") ) {

			pattern_track->set_note(Track_Pattern::Position(cursor.get_tick_pos(),pattern_edit.column),Track_Pattern::Note(Track_Pattern::Note::NOTE_OFF));
			cursor.set_pos( cursor.get_pos() +1 );

		}
		
		CASE( KEYBIND("move_note_down") ) {
		
			Tick cursor_pos=cursor.get_tick_pos();
			Tick next_pos=cursor.get_tick_pos()+cursor.get_snap_tick_size();
			if (!pattern_track->is_pos_editable( cursor_pos ) ||
						  !pattern_track->is_pos_editable( next_pos ) )
				return false;
			Track_Pattern::Note note=pattern_track->get_note( Track_Pattern::Position( cursor_pos, pattern_edit.column ) );
			if (note.is_empty())
				return false;
			
			if (!pattern_track->get_note( Track_Pattern::Position( next_pos, pattern_edit.column ) ).is_empty() )
				return false; //cant move here
			
			pattern_track->set_note( Track_Pattern::Position( next_pos, pattern_edit.column) , note );
			pattern_track->set_note( Track_Pattern::Position( cursor_pos, pattern_edit.column) ,Track_Pattern::Note()  );
			cursor.set_pos( cursor.get_pos() +1 );
			
		}
		CASE( KEYBIND("move_note_up") ) {
		
			Tick cursor_pos=cursor.get_tick_pos();
			Tick prev_pos=cursor.get_tick_pos()-cursor.get_snap_tick_size();
			if (!pattern_track->is_pos_editable( cursor_pos ) ||
						  !pattern_track->is_pos_editable( prev_pos ) )
				return false;
			Track_Pattern::Note note=pattern_track->get_note( Track_Pattern::Position( cursor_pos, pattern_edit.column ) );
			if (note.is_empty())
				return false;
			
			if (!pattern_track->get_note( Track_Pattern::Position( prev_pos, pattern_edit.column ) ).is_empty() )
				return false; //cant move here
			
			pattern_track->set_note( Track_Pattern::Position( prev_pos, pattern_edit.column) , note );
			pattern_track->set_note( Track_Pattern::Position( cursor_pos, pattern_edit.column) ,Track_Pattern::Note()  );
			cursor.set_pos( cursor.get_pos() -1 );
			
		}
		
		CASE( KEYBIND("insert") ) {
		
			
			Tick cursor_pos=cursor.get_tick_pos();

			int idx=pattern_track->get_block_idx_at_pos( cursor_pos );
			if (idx==-1)
				return false;
			
			Tick block_pos=pattern_track->get_block_pos( idx );
			Tick cursor_block_pos=cursor_pos-block_pos;
			
			Track_Pattern::PatternBlock *p = pattern_track->get_block(idx);
			
			for (int i=(p->get_note_count()-1);i>=0;i--) {
				
				Track_Pattern::Position pos=p->get_note_pos(i);
				if (pos.tick<cursor_block_pos)
					continue;
				if (pos.column!=pattern_edit.column)
					continue;
				
				Track_Pattern::Note note=p->get_note(i);
				
				//erase
				pattern_track->set_note( Track_Pattern::Position( pos.tick+block_pos, pos.column), Track_Pattern::Note() );
				//set again above
				Tick new_pos=pos.tick+block_pos+cursor.get_snap_tick_size();
				if (pattern_track->is_pos_editable( new_pos ))
					pattern_track->set_note( Track_Pattern::Position( new_pos, pos.column), note );
				
				
			}
						
		}
		CASE( KEYBIND("delete") ) {
		
			Tick cursor_pos=cursor.get_tick_pos();

			int idx=pattern_track->get_block_idx_at_pos( cursor_pos );
			if (idx==-1)
				return false;
			
			Tick block_pos=pattern_track->get_block_pos( idx );
			Tick cursor_block_pos=cursor_pos-block_pos;
			
			Track_Pattern::PatternBlock *p = pattern_track->get_block(idx);
			
			for (int i=0;i<p->get_note_count();i++) {
				
				Track_Pattern::Position pos=p->get_note_pos(i);
				if (pos.tick<cursor_block_pos)
					continue;
				if (pos.column!=pattern_edit.column)
					continue;
				
				Track_Pattern::Note note=p->get_note(i);
				
				//erase
				pattern_track->set_note( Track_Pattern::Position( pos.tick+block_pos, pos.column), Track_Pattern::Note() );
				//set again above
				Tick new_pos=pos.tick+block_pos-cursor.get_snap_tick_size();
				
				if (new_pos<cursor_block_pos)
					continue;
					
				if (pattern_track->is_pos_editable( new_pos ))
					pattern_track->set_note( Track_Pattern::Position( new_pos, pos.column), note );
				
				
			}
			
		}
		
		CASE( KEYBIND("note_entry/toggle_note_edit") ) {


			switch (pattern_edit.note_edit_mode) {

				case MODE_NOTE: pattern_edit.note_edit_mode=MODE_VOLUME; break;
				case MODE_VOLUME: pattern_edit.note_edit_mode=MODE_NOTE; break;
			}
			
			ui_update_notify->edit_window_changed();

		}
		
		DEFAULT
				
			repaint=false;
		
	END_SWITCH;
	
	/* NOTE EDIT */
	if (pattern_track->is_pos_editable( cursor.get_tick_pos() ) ) {
		   
		   if (pattern_edit.field==0 && pattern_edit.note_edit_mode==MODE_NOTE) {
	
			
			for (int i=0;i<MAX_KEYS;i++) {
				if (IS_KEYBIND((String)"note_entry/"+key_name[i],p_event)) {
					/* @TODO default volume */
					int note=global_edit.editing_octave*12+i;
					Tick tickpos=cursor.get_tick_pos();
					pattern_track->set_note(Track_Pattern::Position(tickpos,pattern_edit.column),Track_Pattern::Note(note,60));
					cursor.set_pos( cursor.get_pos() +1 );
	
				}
			}
		}
	
		if (pattern_edit.note_edit_mode==MODE_VOLUME && Keyboard_Input::get_singleton_instance()->is_number(p_event)) {
	
			int number=Keyboard_Input::get_singleton_instance()->get_number(p_event);
			Tick tick=cursor.get_tick_pos(); //@TODO make in many notes
			Track_Pattern::Note note = pattern_track->get_note(Track_Pattern::Position(tick,pattern_edit.column));
			if (note.is_empty())
				return false;
			if (pattern_edit.field==0) {
				note.volume=(note.volume%10)+10*number;
				pattern_edit.field=1;
			} else {
				note.volume=(note.volume/10)*10+number;
				cursor.set_pos( cursor.get_pos() +1 );
				pattern_edit.field=0;
			}
	
			pattern_track->set_note(Track_Pattern::Position(tick,pattern_edit.column),note);
	
		} else if (pattern_edit.note_edit_mode==MODE_NOTE && pattern_edit.field==1 && Keyboard_Input::get_singleton_instance()->is_number(p_event)) {
	
			int octave=Keyboard_Input::get_singleton_instance()->get_number(p_event);
			Tick tick=cursor.get_tick_pos(); //@TODO make in many notes
			Track_Pattern::Note note = pattern_track->get_note(Track_Pattern::Position(tick,pattern_edit.column));
			if (note.is_empty())
				return false;
			//set octave
			note.note=note.note%12+octave*12;
			pattern_track->set_note(Track_Pattern::Position(tick,pattern_edit.column),note);
			cursor.set_pos( cursor.get_pos() +1 );
		};

		ui_update_notify->edit_window_changed(); //there may be linked patterns, update all

	}	
	return repaint;
}



BlockList* Editor::get_blocklist(int p_idx) {
	
	int blocks=0;
	for (int i=0;i<song->get_track_count();i++) {
		Track * t = song->get_track(i);
		if (blocks==p_idx)
			return t;
		blocks++;
		
		for (int j=0;j<t->get_automation_count();j++) {
			
			if (blocks==p_idx)
				return t->get_automation(j);
			blocks++;
		}
	}
	
	return NULL;
	
}
int Editor::get_blocklist_count() {
	
	int blocks=0;
	for (int i=0;i<song->get_track_count();i++) {
		Track * t = song->get_track(i);
		blocks++;
		
		for (int j=0;j<t->get_automation_count();j++) {
			
			blocks++;
		}
	}
	
	return blocks;
}


void Editor::track_pattern_add_column(int p_which) {
	
	int track_idx=(p_which==-1)?get_current_track():p_which;
	Track_Pattern * p = dynamic_cast<Track_Pattern*>(song->get_track(track_idx));	
	if (!p)
		return;
	
	p->set_visible_columns( p->get_visible_columns() +1 );
	ui_update_notify->track_list_changed();

}


void Editor::track_pattern_remove_column(int p_which) {
	
	int track_idx=(p_which==-1)?get_current_track():p_which;
	Track_Pattern * p = dynamic_cast<Track_Pattern*>(song->get_track(track_idx));	
	if (!p)
		return;
	
	p->set_visible_columns( p->get_visible_columns() -1 );
	ui_update_notify->track_list_changed();
	
}


int Editor::get_track_blocklist(int p_track) {
	
	ERR_FAIL_INDEX_V(p_track,song->get_track_count(),-1);
	int blocks=0;
	for (int i=0;i<song->get_track_count();i++) {
		Track * t = song->get_track(i);
		if (p_track==i)
			return blocks;
		
		blocks++;
		
		for (int j=0;j<t->get_automation_count();j++) {
			
			blocks++;
		}
	}
	
	return -1;
}

int Editor::get_blocklist_track(int p_blocklist) {
	
	int blocks=0;
	for (int i=0;i<song->get_track_count();i++) {
		Track * t = song->get_track(i);
		if (blocks==p_blocklist)
			return i;
		
		blocks++;
		
		for (int j=0;j<t->get_automation_count();j++) {
			
			if (blocks==p_blocklist)
				return i;
			blocks++;
		}
	}
	
	return -1;
}


int Editor::get_current_blocklist() {
	
	return global_edit.current_blocklist;
}

int Editor::find_Track(Track *p_track) {
	
	for (int i=0;i<song->get_track_count();i++) 
		if (song->get_track(i)==p_track)
			return i;
	
	return -1;
}
void Editor::set_track_name(int p_track,String p_name) {
	
	Track *t=song->get_track(p_track);
	ERR_FAIL_COND(t==NULL);
	t->set_name( p_name );
}
int Editor::get_current_track() {
	
	int blocks=0;
	for (int i=0;i<song->get_track_count();i++) {
		Track * t = song->get_track(i);
		if (global_edit.current_blocklist==blocks)
			return i;
		blocks++;
		
		for (int j=0;j<t->get_automation_count();j++) {
			
			if (global_edit.current_blocklist==blocks)
				return i;
			
			blocks++;
		}
	}
	
	return -1;
	
}
void Editor::set_current_blocklist(int p_which) {
	
	if (p_which==global_edit.current_blocklist)
		return; //nothing happens
	ERR_FAIL_INDEX(p_which,get_blocklist_count());
	global_edit.current_blocklist=p_which;
	ui_update_notify->cursor_changed_blocklist();
	
}

void Editor::add_automation(String p_path) {
	
	int selected_track=get_current_track();
	Track *t = song->get_track(selected_track);
	t->add_automation(p_path);
	ui_update_notify->track_list_changed();
	
}

Song* Editor::get_song() {
	
	return song;
}

void Editor::add_track(TrackType p_type,int p_channels,String p_name) {

	
	Track *track;
	switch (p_type) {
		
		case TRACK_TYPE_PATTERN: {
			
			
			track =  new Track_Pattern(p_channels,song->get_pattern_pool());
			track->set_name(p_name);
			track->set_automation_pool( song->get_automation_pool() );

		} break;
	}
	
	song->add_track(track);
	
	for (int i=0;i<song->get_track_count();i++) {
		
		if (song->get_track(i)!=track)
			continue;
		int blocklist=get_track_blocklist( i );
		ERR_FAIL_COND(blocklist==-1);
		global_edit.current_blocklist=blocklist;
		enter_blocklist( ENTER_LEFT );
	}
}

void Editor::set_editing_octave(int p_octave) {
	
	if (p_octave<0 || p_octave>8)
		return;
	
	if (global_edit.editing_octave==p_octave)
		return; //pointless otherwise
	global_edit.editing_octave=p_octave;
	ui_update_notify->editing_octave_changed();
}
int Editor::get_editing_octave() {
	
	return global_edit.editing_octave;
}

void Editor::set_snap(int p_new_snap) {
	
	cursor.set_snap( p_new_snap );	
}

Tick Editor::get_block_list_max_len(BlockList *p_bl) {
	
	int block_count=p_bl->get_block_count();
	if (block_count==0)
		return 0;
	Tick len=0;
	switch (p_bl->get_block_type()) {
		
		case BlockList::BLOCK_TYPE_FIXED_TO_BEAT: {		
			
			len=p_bl->get_block_pos(block_count-1)+p_bl->get_block(block_count-1)->get_length();
		} break;
		case BlockList::BLOCK_TYPE_FREE_MOVING: {		
			
		} break;
	}
	
	return len;
}

Tick Editor::get_song_max_len() {
	Tick max_len=0;
	
	for (int i=0;i<song->get_track_count();i++) {
		Track * t = song->get_track(i);
		
		if (get_block_list_max_len(t)>max_len)
			max_len=get_block_list_max_len(t);
		for (int j=0;j<t->get_automation_count();j++) {
			
			Automation *a=t->get_automation(j);;
			if (get_block_list_max_len(a)>max_len)
				max_len=get_block_list_max_len(a);
			
		}
	}
	
	return max_len;
}
Editor::Editor(Song *p_song,UI_UpdateNotify *p_ui_update_notify) : cursor(p_ui_update_notify) {
	
	ui_update_notify=p_ui_update_notify;
	global_edit.current_blocklist=-1;
	pattern_edit.column=0;
	pattern_edit.field=0;
	pattern_edit.note_edit_mode=MODE_NOTE;
	song = p_song;
	global_edit.editing_octave=4;
}


Editor::~Editor()
{
}


}
