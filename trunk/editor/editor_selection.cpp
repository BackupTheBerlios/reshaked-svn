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
#include "edit_commands.h"

void Editor::selection_verify() {

	if (!selection.active)
		return;
		
	if (selection.begin.track > selection.end.track ) {
		SWAP( selection.begin.track, selection.end.track );
		SWAP( selection.begin.col, selection.end.col );
		
	} else if (selection.begin.track == selection.end.track ) {
			
		if (selection.begin.col > selection.end.col )
			SWAP( selection.begin.col, selection.end.col );
	}
		
	if (selection.begin.tick > selection.end.tick )
		SWAP( selection.begin.tick, selection.end.tick );
		
	selection.begin.tick-=selection.begin.tick%get_ticks_per_row(); // make sure it starts at zero
	selection.end.tick=selection.end.tick-(selection.end.tick%get_ticks_per_row()) + get_ticks_per_row()-1; // make sure it starts at get_ticks_per_row() -1
		
	
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
	shift_selection.begin.col=p_col;
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


void Editor::selection_command(SelectionCommand p_command,int p_param) {

	Selection s=selection;
	
	String target="Selection";
	
	if (!s.active) {
	
		s.begin.tick=cursor.tick;
		s.end.tick=cursor.tick;
		s.begin.track=cursor.track;
		s.end.track=cursor.track;
		s.begin.col=cursor.col;
		s.end.col=cursor.col;
		s.active=true;
		target="Cursor";
	}
	switch(p_command) {
	
		case SELECTION_SCALE_VOLUMES: {
		
			EditCommands::get_singleton()->begin_group(target+" - Scale Volumes");
		} break;
		case SELECTION_APPLY_VOLUME_MASK: {
		
			EditCommands::get_singleton()->begin_group(target+" - Apply Volume Mask");		
		} break;
		case SELECTION_QUANTIZE_UP: {
		
			EditCommands::get_singleton()->begin_group(target+" - Quantize Up");		
		} break;
		case SELECTION_QUANTIZE_NEAREST: {
		
		
			EditCommands::get_singleton()->begin_group(target+" - Quantize Nearest");		
		} break;
		case SELECTION_QUANTIZE_DOWN: {
		
			EditCommands::get_singleton()->begin_group(target+" - Quantize Down");		
		} break;
		case SELECTION_TRANSPOSE_UP: {
		
			EditCommands::get_singleton()->begin_group(target+" - Transpose Up Semitone");		
		} break;
		case SELECTION_TRANSPOSE_UP_OCTAVE: {
		
			EditCommands::get_singleton()->begin_group(target+" - Transpose Up Octave");		
		} break;
		case SELECTION_TRANSPOSE_DOWN: {
		
			EditCommands::get_singleton()->begin_group(target+" - Transpose Down");		
		} break;
		case SELECTION_TRANSPOSE_DOWN_OCTAVE: {
		
			EditCommands::get_singleton()->begin_group(target+" - Transpose Octave");		
		} break;
	
	
	}


	

	for(int i=s.begin.track;i<=s.end.track;i++) {
	
		if (i<0 || i>=song->get_track_count())
			continue;
	
		Track *t = song->get_track(i);
		if (t->is_collapsed()) // don't work in collapsed tracks
			continue;
			
										
		if (dynamic_cast<PatternTrack*>(t)) {
			// its-a-pattern mario!
			
			std::list<NoteListElement> nelist;
			
			int fromblock,toblock;
			
			if (t->get_blocks_in_rage(s.begin.tick,s.end.tick,&fromblock,&toblock)) {
				
				for (int j=fromblock;j<=toblock;j++) {
				
					PatternTrack::PatternBlock *b = (PatternTrack::PatternBlock*)t->get_block(j);
					
					Tick bfrom=s.begin.tick-t->get_block_pos(j);
					Tick bto=s.end.tick-t->get_block_pos(j);
					
					int fromblock2,toblock2;
					
					if (b->get_notes_in_local_range(bfrom,bto,&fromblock2,&toblock2)) {
					
						for (int k=fromblock2;k<=toblock2;k++) {
						
							PatternTrack::Position pos = b->get_note_pos(k);
							
							if ( i==s.begin.track && pos.column<s.begin.col )
								continue;
							if ( i==s.end.track &&pos.column>s.end.col )
								continue;
								
							NoteListElement nle;
							nle.pos=pos;
								
							nle.pos.tick+=t->get_block_pos(j);
															
							nle.note=b->get_note(k);
							
							nelist.push_back(nle);
						}
					}
				}
			}
			
			std::list<NoteListElement>::iterator I;
			
			
			for(I=nelist.begin();I!=nelist.end();I++) {
			
				switch(p_command) {
				
					case SELECTION_SCALE_VOLUMES: {
					
						if (!I->note.is_note())
							break;
							
						int vol = p_param * (int)I->note.volume / PatternTrack::Note::MAX_VOLUME;
						if (vol<0)
							vol=0;
						if (vol>=PatternTrack::Note::MAX_VOLUME)
							vol=PatternTrack::Note::MAX_VOLUME;
						
						I->note.volume=vol;
						
					} break;
					case SELECTION_APPLY_VOLUME_MASK: {
					
						if (!I->note.is_note())
							break;
							
						I->note.volume=insert.volume_mask_active;
					
					} break;
					case SELECTION_QUANTIZE_UP: {
					
						I->pos.tick -= I->pos.tick % get_ticks_per_row();
					
					} break;
					case SELECTION_QUANTIZE_NEAREST: {
					
						Tick tick = I->pos.tick;
						I->pos.tick -= I->pos.tick % get_ticks_per_row();
					
						if (tick % get_ticks_per_row() > get_ticks_per_row()/2 ) 
							I->pos.tick+=get_ticks_per_row();
					
					} break;
					case SELECTION_QUANTIZE_DOWN: {
					
						if (I->pos.tick % get_ticks_per_row()) {
							I->pos.tick+=get_ticks_per_row();
						}
					
					} break;
					case SELECTION_TRANSPOSE_UP: {
					
						if (!I->note.is_note())
							break;
							
						if (I->note.note<(PatternTrack::Note::MAX_NOTES-1))
							I->note.note++;
						
					
					} break;
					case SELECTION_TRANSPOSE_UP_OCTAVE: {
					
						if (!I->note.is_note())
							break;
					
						if (I->note.note<(PatternTrack::Note::MAX_NOTES-13))
							I->note.note+=12;
					
					} break;
					case SELECTION_TRANSPOSE_DOWN: {
					
						if (!I->note.is_note())
							break;
					
						if (I->note.note>0)
							I->note.note--;
					
					} break;
					case SELECTION_TRANSPOSE_DOWN_OCTAVE: {
					
						if (!I->note.is_note())
							break;
					
						if (I->note.note>=12)
							I->note.note-=12;
					
					} break;
				
				
				}
				
				
				pattern_track_set_note( i, I->pos, I->note );
			}
			
		}
	}

	EditCommands::get_singleton()->end_group();


}
