//
// C++ Implementation: editor_selection
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
#include "key_bindings.h"

void Editor::selection_verify() {

	if (!selection.active)
		return;
		
	if (selection.begin.track > selection.end.track )
		SWAP( selection.begin.track, selection.end.track );
		
	if (selection.begin.col > selection.end.col )
		SWAP( selection.begin.col, selection.end.col );
		
	if (selection.begin.tick > selection.end.tick )
		SWAP( selection.begin.tick, selection.end.tick );
		
}


void Editor::shift_selection_check_begin(unsigned int &p_code) {

	static const char* dirs[]={
		"up","down","left","right","page_up","page_down","home","end",0};
		
		
	int idx=0;
	
	while (dirs[idx]) {
	
		if (p_code==KEYBIND( "editor/selection_"+String(dirs[idx]))) {
		
			if (!shift_selection.active || (cursor.tick+get_ticks_per_row()-1) != shift_selection.end.tick || cursor.col != shift_selection.end.col || cursor.track != shift_selection.end.track ) {
			
				shift_selection.begin.track=cursor.track;
				shift_selection.begin.col=cursor.col;
				shift_selection.begin.tick=cursor.tick;
				
				shift_selection.end=shift_selection.begin;
				shift_selection.end.tick+=get_ticks_per_row()-1; // selection musnt include the next row
				shift_selection.active=true;
			}
			p_code=KEYBIND( String(dirs[idx]) );
			return;
		}
	
		idx++;
	}
	
	shift_selection.active=false;
	
}
void Editor::shift_selection_check_end() {

	if (!shift_selection.active)
		return;
	shift_selection.end.track=cursor.track;
	shift_selection.end.col=cursor.col;
	shift_selection.end.tick=cursor.tick+get_ticks_per_row()-1;
	
	selection=shift_selection;
	selection_verify();	
	UpdateNotify::get_singleton()->selection_changed();
	
}

void Editor::set_mouse_selection_begin(int p_track,int p_col, int p_row) {

	// use shift selection for mouse

	shift_selection.begin.track=p_track;
	shift_selection.begin.col=p_col;cursor.col;
	shift_selection.begin.tick=get_row_ticks( p_row );
	
	shift_selection.end=shift_selection.begin;
	shift_selection.end.tick+=get_ticks_per_row()-1; // selection musnt include the next row
	shift_selection.active=true;
	

}
void Editor::set_mouse_selection_end(int p_track,int p_col, int p_row) {

	if (!shift_selection.active)
		return;
		
	shift_selection.end.track=p_track;
	shift_selection.end.col=p_col;
	shift_selection.end.tick=get_row_ticks( p_row )+get_ticks_per_row()-1;
	
	selection=shift_selection;
	selection_verify();	
	UpdateNotify::get_singleton()->selection_changed();


}


void Editor::set_selection_begin_at_cursor() {

	selection.begin.track=cursor.track;
	selection.begin.col=cursor.col;
	selection.begin.tick=cursor.tick;
	if (!selection.active) {
	
		selection.end=selection.begin;
		selection.end.tick+=get_ticks_per_row()-1; // selection musnt include the next row
		selection.active=true;
	}
	
	selection_verify();
	
	UpdateNotify::get_singleton()->selection_changed();
}
void Editor::set_selection_end_at_cursor() {

	if (!selection.active) {
		set_selection_begin_at_cursor();
		return;
	}

	selection.end.track=cursor.track;
	selection.end.col=cursor.col;
	selection.end.tick=cursor.tick+get_ticks_per_row()-1;;
	
	selection_verify();
	
	UpdateNotify::get_singleton()->selection_changed();
}

void Editor::select_column_block_all() {

	if (cursor.track<0 || cursor.track>=song->get_track_count())
		return;
		
	Track *track = song->get_track(cursor.track);
	int block = track->get_block_at_pos( cursor.tick );
	
	if (block<0)
		return;
		
/*	if (!selection.active || selection.begin.tick!=track->get_block_pos(block) || selection.begin.track!=song->find_track_pos(track) || selection.begin.col!=cursor.col || 
	selection.end.tick!=(track->get_block_pos(block)+track->get_block(block)->get_length()-1) || selection.end.track!=song->find_track_pos(track) || selection.end.col!=cursor.col ) {*/
	
	if (selection.active && selection.begin.tick==track->get_block_pos(block) && selection.begin.track==cursor.track && 				selection.begin.col==0 && 
		selection.end.tick==(track->get_block_pos(block)+track->get_block(block)->get_length()-1) && selection.end.track==cursor.track && selection.end.col==(track->get_visible_columns()-1)) {
		// select block	
		printf("detected it must select all\n");
		selection.begin.track=0;
		selection.begin.col=0;
		selection.begin.tick=track->get_block_pos(block);
		selection.end.track=song->get_track_count()-1;
		selection.end.col=song->get_track( song->get_track_count()-1)->get_visible_columns();
		selection.end.tick=track->get_block_pos(block)+track->get_block(block)->get_length()-1;
	
		
	} else if (selection.active && selection.begin.tick==track->get_block_pos(block) && selection.begin.track==cursor.track && 		selection.begin.col==cursor.col && 
		selection.end.tick==(track->get_block_pos(block)+track->get_block(block)->get_length()-1) && selection.end.track==cursor.track && selection.end.col==cursor.col) {
		
		printf("detected it must select all\n");
		// select block	
		selection.begin.track=cursor.track;
		selection.begin.col=0;
		selection.begin.tick=track->get_block_pos(block);
		selection.end.track=cursor.track;
		selection.end.col=track->get_visible_columns()-1;
		selection.end.tick=track->get_block_pos(block)+track->get_block(block)->get_length()-1;
		
		
	  } else {	  
		// select column
		printf("detected it must select column\n");
		selection.active=true;
		selection.begin.track=cursor.track;
		selection.begin.col=cursor.col;
		selection.begin.tick=track->get_block_pos(block);
		selection.end.track=cursor.track;
		selection.end.col=cursor.col;
		selection.end.tick=track->get_block_pos(block)+track->get_block(block)->get_length()-1;
	}	
	
	

	selection_verify();
	UpdateNotify::get_singleton()->selection_changed();
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

	return selection.begin.tick/get_ticks_per_row();
}
int Editor::get_selection_end_track() const {

	return selection.end.track;
}
int Editor::get_selection_end_column() const {

	return selection.end.col;
}
int Editor::get_selection_end_row() const {

	return selection.end.tick/get_ticks_per_row();
}



void Editor::set_selection_disabled() {

	selection.active=false;
	UpdateNotify::get_singleton()->selection_changed();
}