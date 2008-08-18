//
// C++ Implementation: editor_clipboard
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
#include "edit_commands.h"

void Editor::clipboard_copy() {

	if (!selection.active)
		return;
		
	clipboard.clear();
	
	for(int i=selection.begin.track;i<=selection.end.track;i++) {
	
		if (i<0 || i>=song->get_track_count())
			continue;
	
		Track *t = song->get_track(i);
		if (t->is_collapsed()) // don't work in collapsed tracks
			continue;
			
		for(int c=0;c<t->get_visible_columns();c++) {		
									
			if ( i==selection.begin.track && c<selection.begin.col )
				continue;
			if ( i==selection.end.track &&c>selection.end.col )
				continue;
			
			if (dynamic_cast<PatternTrack*>(t)) {
				// its-a-pattern mario!
				Clipboard::PatternTrackData *tdata = new Clipboard::PatternTrackData;
				clipboard.tracks.push_back(tdata);
				
				int fromblock,toblock;
				
				if (t->get_blocks_in_rage(selection.begin.tick,selection.end.tick,&fromblock,&toblock)) {
					
					for (int j=fromblock;j<=toblock;j++) {
					
						PatternTrack::PatternBlock *b = (PatternTrack::PatternBlock*)t->get_block(j);
						
						Tick bfrom=selection.begin.tick-t->get_block_pos(j);
						Tick bto=selection.end.tick-t->get_block_pos(j);
						
						int fromblock2,toblock2;
						
						if (b->get_notes_in_local_range(bfrom,bto,&fromblock2,&toblock2)) {
						
							for (int k=fromblock2;k<=toblock2;k++) {
							
								PatternTrack::Position pos = b->get_note_pos(k);
								
								if (pos.column!=c)
									continue;
									
								pos.column=0;
									
								NoteListElement nle;
								nle.pos=pos;
									
								nle.pos.tick=nle.pos.tick+t->get_block_pos(j)-selection.begin.tick;
																
								nle.note=b->get_note(k);
								tdata->notes.push_back(nle);
							}
						}
					}
				}
			}
		}
	}
	
	if (clipboard.tracks.size()>0) {
	
		clipboard.length=(selection.end.tick-selection.begin.tick) + 1;
		clipboard.active=true;
	}
		
}

void Editor::clipboard_cut() {

	EditCommands::get_singleton()->begin_group("Clipboard - Cut");
	clipboard_copy();
	clipboard_zap_area(selection);
	EditCommands::get_singleton()->end_group();
	
}

void Editor::clipboard_zap_area(const Selection& p_selection) {

	if (!p_selection.active)
		return;

	for(int i=p_selection.begin.track;i<=p_selection.end.track;i++) {
	
		if (i<0 || i>=song->get_track_count())
			continue;
	
		Track *t = song->get_track(i);
		if (t->is_collapsed()) // don't work in collapsed tracks
			continue;
			
		if (dynamic_cast<PatternTrack*>(t)) {
			// its-a-pattern mario!
			
			int fromblock,toblock;
			
			if (t->get_blocks_in_rage(p_selection.begin.tick,p_selection.end.tick,&fromblock,&toblock)) {
				
				for (int j=fromblock;j<=toblock;j++) {
				
					PatternTrack::PatternBlock *b = (PatternTrack::PatternBlock*)t->get_block(j);
					
					Tick bfrom=p_selection.begin.tick-t->get_block_pos(j);
					Tick bto=p_selection.end.tick-t->get_block_pos(j);
					
					int fromblock2,toblock2;
					
					std::list<NoteListElement> notes;
					
					if (b->get_notes_in_local_range(bfrom,bto,&fromblock2,&toblock2)) {
					
						//printf("get notes in local range from %i to %i - %i, %i\n",(int)bfrom,(int)bto,fromblock2,toblock2);
						
						for (int k=fromblock2;k<=toblock2;k++) {
						
							PatternTrack::Position pos = b->get_note_pos(k);
							
							if ( i==p_selection.begin.track && pos.column<p_selection.begin.col )
								continue;
							if ( i==p_selection.end.track && pos.column>p_selection.end.col )
								continue;
								
								
							NoteListElement nle;
							nle.pos=pos;
							nle.pos.tick+=t->get_block_pos(j);
							
							nle.note=b->get_note(k);
							
							notes.push_back(nle);
						}
					}
					
					for(std::list <NoteListElement>::iterator I=notes.begin();I!=notes.end();I++) {
					
						EditCommands::get_singleton()->pattern_set_note( dynamic_cast<PatternTrack*>(t), I->pos.column, I->pos.tick,  PatternTrack::Note() );
					}
					
				}
			}
		}
	}

}
void Editor::clipboard_paste(PasteMode p_mode) {

	if (!clipboard.active)
		return;
	/* FIRST - validate paste area */ 

	int trackidx = cursor.track;
	int colidx =  cursor.col;
	int to_track=cursor.track;
	int to_col=cursor.col;
	
	for(int i=0;i<clipboard.tracks.size();i++) {
	
		
		while(true) {
		
			if (trackidx<0 || trackidx >= song->get_track_count())
				return;
			
			if (song->get_track(trackidx)->is_collapsed()) {
			 
			 	trackidx++;
			 	continue;
			}
					
			if (colidx >= song->get_track(trackidx)->get_visible_columns()) {
			
				trackidx++;
				colidx=0;
				continue;
			}
		

			if (dynamic_cast<Clipboard::PatternTrackData*>(clipboard.tracks[i]) && dynamic_cast<PatternTrack*>(song->get_track(trackidx))) {
			
				
				
				to_track=trackidx;				
				to_col=colidx;
				colidx++;
				break;
			}
				
			return; // epic fail
		}
		
		
				
	}
	
	switch(p_mode) {
		case PASTE_INSERT: EditCommands::get_singleton()->begin_group("Clipboard - Paste Insert"); break;
		case PASTE_MIX: EditCommands::get_singleton()->begin_group("Clipboard - Paste mix"); break;
		case PASTE_OVERWRITE: EditCommands::get_singleton()->begin_group("Clipboard - Paste Overwrite"); break;
	}
	
	/* SECOND - check paste insert */ 

	if (p_mode==PASTE_INSERT) {
		// insert

		trackidx = cursor.track;
		colidx =  cursor.col;
	
		for(int i=0;i<clipboard.tracks.size();i++) {
		
			while(true) {
			
				// this shold have been validated previously
				if (trackidx<0 || trackidx >= song->get_track_count()) {
				
					EditCommands::get_singleton()->end_group(); //bug  :(
				}
				ERR_FAIL_COND(trackidx<0 || trackidx >= song->get_track_count());
				
				
				if (song->get_track(trackidx)->is_collapsed()) {
				
					trackidx++;
					continue;
				}
						
				if (colidx >= song->get_track(trackidx)->get_visible_columns()) {
				
					trackidx++;
					colidx=0;
					continue;
				}
		
				printf("inserting at track %i, col %i, tick %i\n",trackidx,colidx,(int)cursor.tick);
				insert_at_pos(trackidx, colidx, cursor.tick,clipboard.length/get_ticks_per_row());
				colidx++;
				break;
			}
		}
	}


	/* THIRD - zap area unless paste mix */ 

	if (p_mode!=PASTE_MIX) {

		Selection zaparea;
		zaparea.active=true;
		zaparea.begin.tick=cursor.tick;
		zaparea.end.tick=cursor.tick+clipboard.length-1;
		zaparea.begin.track=cursor.track;
		zaparea.end.track=to_track;
		zaparea.begin.col=cursor.col;
		zaparea.end.col=to_col;
		
		clipboard_zap_area(zaparea);
	}

	/* FOURTH - insert the notes */ 
	
	trackidx = cursor.track;
	colidx =  cursor.col;

	for(int i=0;i<clipboard.tracks.size();i++) {
	
		while(true) {
		
			// this shold have been validated previously
			if (trackidx<0 || trackidx >= song->get_track_count()) {
			
				EditCommands::get_singleton()->end_group(); // bug :(
			}
		
			// this shold have been validated previously
			ERR_FAIL_COND(trackidx<0 || trackidx >= song->get_track_count());
			
			
			if (song->get_track(trackidx)->is_collapsed()) {
			
				trackidx++;
				continue;
			}
					
			if (colidx >= song->get_track(trackidx)->get_visible_columns()) {
			
				trackidx++;
				colidx=0;
				continue;
			}
	
			if (dynamic_cast<Clipboard::PatternTrackData*>(clipboard.tracks[i]) && dynamic_cast<PatternTrack*>(song->get_track(trackidx))) {
				// is pattern
				Clipboard::PatternTrackData* ptd=dynamic_cast<Clipboard::PatternTrackData*>(clipboard.tracks[i]);
				PatternTrack*pt = dynamic_cast<PatternTrack*>(song->get_track(trackidx));
				
				std::list<NoteListElement>::iterator I;
				
				for (I=ptd->notes.begin();I!=ptd->notes.end();I++) {
					
					EditCommands::get_singleton()->pattern_set_note( pt, colidx, I->pos.tick+cursor.tick, I->note );

					
				}
			}
			
			colidx++;
			break;
		}
		
			
	}

	EditCommands::get_singleton()->end_group(); 

}
