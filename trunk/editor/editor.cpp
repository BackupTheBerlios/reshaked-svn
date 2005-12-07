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


Track_Pattern::Note _fix_macro(const Track_Pattern::Note &p_note) { return p_note; }
#define SET_NOTE(m_pos,m_note) d->undo_stream.add_command( Command3(&commands,&EditorCommands::set_note,pattern_track,m_pos,_fix_macro(m_note)) )
#define SET_NOTE_AT_CURSOR(m_note) SET_NOTE( Track_Pattern::Position(d->cursor.get_tick_pos(),d->pattern_edit.column),_fix_macro(m_note))

Cursor &Editor::get_cursor() {
	
	return d->cursor;
}

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

void Editor::enter_blocklist(EditorData::EnterBLDir p_dir) {
	
	BlockList* bl = get_blocklist(d->global_edit.current_blocklist);
	ERR_FAIL_COND(bl == NULL);
	if (bl->get_type_name()=="pattern") {
		
		Track_Pattern *tp=dynamic_cast<Track_Pattern*>(bl);
		ERR_FAIL_COND(tp==NULL);
		if (p_dir==EditorData::ENTER_LEFT) {
				
			d->pattern_edit.column=0;
			d->pattern_edit.field=0;
			
		} else {
			
			d->pattern_edit.column=tp->get_visible_columns()-1;
			d->pattern_edit.field=1;
		}
	}

	d->ui_update_notify->cursor_changed_blocklist();
}

bool Editor::automation_edit_key_press(int p_key_value) {
	
	Automation *automation=dynamic_cast<Automation*>(get_blocklist( get_current_blocklist() ) );
	ERR_FAIL_COND_V(automation==NULL,false);
	
	if (handle_navigation_key_press( automation, p_key_value))
		return true;
	bool handled=true;
	
	SWITCH(p_key_value)
			
		CASE( KEYBIND("left") ) {

			if (d->global_edit.current_blocklist>0) {
				d->global_edit.current_blocklist--;
				enter_blocklist(EditorData::ENTER_RIGHT);
			}				
		}			
		CASE( KEYBIND("right") ) {
			if (d->global_edit.current_blocklist<(get_blocklist_count()-1)) {
				d->global_edit.current_blocklist++;
				enter_blocklist(EditorData::ENTER_LEFT);
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

			d->cursor.set_pos( d->cursor.get_pos() -1 );

		}
		CASE( KEYBIND("down") ) {

			d->cursor.set_pos( d->cursor.get_pos() +1 );

		}
		CASE( KEYBIND("page_up") ) {

			d->cursor.set_pos( d->cursor.get_pos() - d->cursor.get_window_size()/2 );

		}
		CASE( KEYBIND("page_down") ) {

			d->cursor.set_pos( d->cursor.get_pos() + d->cursor.get_window_size()/2 );

		}
		CASE( KEYBIND("home") ) {
		
			if (d->global_edit.current_blocklist==0) {
				d->cursor.set_pos( 0 );
			} else {
				d->global_edit.current_blocklist=0;
				enter_blocklist(EditorData::ENTER_LEFT);
			}

		}
		CASE( KEYBIND("end") ) {
		
			d->global_edit.current_blocklist=get_blocklist_count()-1;
			enter_blocklist(EditorData::ENTER_RIGHT);
		
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


		CASE( KEYBIND("page_up_blocksnap") ) {
		
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
		CASE( KEYBIND("page_down_blocksnap") ) {
		
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
				//song_edit->move_editing_left();
		}
		CASE( KEYBIND("right") ) {
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
		
		CASE( KEYBIND("insert") ) {
		
			
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
		CASE( KEYBIND("delete") ) {
		
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



BlockList* Editor::get_blocklist(int p_idx) {
	
	int blocks=0;
	for (int i=0;i<d->song->get_track_count();i++) {
		Track * t = d->song->get_track(i);
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
	for (int i=0;i<d->song->get_track_count();i++) {
		Track * t = d->song->get_track(i);
		blocks++;
		
		for (int j=0;j<t->get_automation_count();j++) {
			
			blocks++;
		}
	}
	
	return blocks;
}


void Editor::track_pattern_add_column(int p_which) {
	
	int track_idx=(p_which==-1)?get_current_track():p_which;
	Track_Pattern * p = dynamic_cast<Track_Pattern*>(d->song->get_track(track_idx));	
	if (!p)
		return;
	
	p->set_visible_columns( p->get_visible_columns() +1 );
	d->ui_update_notify->track_list_changed();

}


void Editor::track_pattern_remove_column(int p_which) {
	
	int track_idx=(p_which==-1)?get_current_track():p_which;
	Track_Pattern * p = dynamic_cast<Track_Pattern*>(d->song->get_track(track_idx));	
	if (!p)
		return;
	
	p->set_visible_columns( p->get_visible_columns() -1 );
	d->ui_update_notify->track_list_changed();
	
}


int Editor::get_track_blocklist(int p_track) {
	
	ERR_FAIL_INDEX_V(p_track,d->song->get_track_count(),-1);
	int blocks=0;
	for (int i=0;i<d->song->get_track_count();i++) {
		Track * t = d->song->get_track(i);
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
	for (int i=0;i<d->song->get_track_count();i++) {
		Track * t = d->song->get_track(i);
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


void Editor::revalidate_cursor() {
	
	if (d->global_edit.current_blocklist>=get_blocklist_count()) {
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
	}
	
}

int Editor::get_current_blocklist() {
	
	return d->global_edit.current_blocklist;
}

int Editor::find_Track(Track *p_track) {
	
	for (int i=0;i<d->song->get_track_count();i++) 
		if (d->song->get_track(i)==p_track)
			return i;
	
	return -1;
}
void Editor::set_track_name(int p_track,String p_name) {
	
	Track *t=d->song->get_track(p_track);
	ERR_FAIL_COND(t==NULL);
	t->set_name( p_name );
}
int Editor::get_current_track() {
	
	int blocks=0;
	for (int i=0;i<d->song->get_track_count();i++) {
		Track * t = d->song->get_track(i);
		if (d->global_edit.current_blocklist==blocks)
			return i;
		blocks++;
		
		for (int j=0;j<t->get_automation_count();j++) {
			
			if (d->global_edit.current_blocklist==blocks)
				return i;
			
			blocks++;
		}
	}
	
	return -1;
	
}
void Editor::set_current_blocklist(int p_which) {
	
	if (p_which==d->global_edit.current_blocklist)
		return; //nothing happens
	ERR_FAIL_INDEX(p_which,get_blocklist_count());
	d->global_edit.current_blocklist=p_which;
	d->ui_update_notify->cursor_changed_blocklist();
	
}

void Editor::get_blocklists_sharing_block(BlockList::Block * p_block,std::list<int> *p_blocklist) {
	
	
	int bl=0;
	for (int i=0;i<d->song->get_track_count();i++) {
		Track * t = d->song->get_track(i);
		
		if (t->shares_block_data(p_block))
			p_blocklist->push_back(bl);
		
		bl++;
		
		for (int j=0;j<t->get_automation_count();j++) {
			
			if (t->get_automation(j)->shares_block_data(p_block))
				p_blocklist->push_back(bl);
		
			bl++;
		}
	}
}

void Editor::add_automation(String p_path) {
	
	int selected_track=get_current_track();
	Track *t = d->song->get_track(selected_track);
	t->add_automation(p_path);
	d->ui_update_notify->track_list_changed();
	
}

Song* Editor::get_song() {
	
	return d->song;
}

void Editor::add_track(TrackType p_type,int p_channels,String p_name) {

	
	Track *track;
	switch (p_type) {
		
		case TRACK_TYPE_PATTERN: {
			
			
			track =  new Track_Pattern(p_channels,d->song->get_pattern_pool());
			track->set_name(p_name);
			track->set_automation_pool( d->song->get_automation_pool() );

		} break;
	}
	
	d->undo_stream.begin("Add Track");
	d->undo_stream.add_command(Command2(&commands,&EditorCommands::add_track,track,d->song->get_track_count()));
	d->undo_stream.end();
	
	/* select the newly added track */
	for (int i=0;i<d->song->get_track_count();i++) {
		
		if (d->song->get_track(i)!=track)
			continue;
		int blocklist=get_track_blocklist( i );
		ERR_FAIL_COND(blocklist==-1);
		d->global_edit.current_blocklist=blocklist;
		enter_blocklist( EditorData::ENTER_LEFT );
	}
}

void Editor::remove_track(int p_which) {
	
	ERR_FAIL_INDEX(p_which,d->song->get_track_count());
	
	d->undo_stream.begin("Remove Track");
	d->undo_stream.add_command(Command1(&commands,&EditorCommands::remove_track,p_which));
	d->undo_stream.end();
	
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
	
	for (int i=0;i<d->song->get_track_count();i++) {
		Track * t = d->song->get_track(i);
		
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

void Editor::undo() {
	
	d->undo_stream.undo();
}
void Editor::redo() {
	
	d->undo_stream.redo();

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
