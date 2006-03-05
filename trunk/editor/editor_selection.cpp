//
// C++ Implementation: editor_selection
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
#include "keyboard_input.h"
#include "editor_data.h"

namespace ReShaked {

static Track_Pattern::Note _fix_macro(const Track_Pattern::Note &p_note) { return p_note; } //fix template annoyance
#define SET_NOTE(m_pos,m_note) d->undo_stream.add_command( Command3(&commands,&EditorCommands::set_note,pattern_track,m_pos,_fix_macro(m_note)) )


void Editor::selection_mouse_drag_begin(int p_blocklist,int p_column,int p_row) {
	
	Tick tick=get_cursor().snap_to_ticks( p_row );
	d->selection.mouse_drag_begin=EditorData::Selection::Pos(p_blocklist, p_column, tick );
	d->selection.mouse_drag_end=d->selection.mouse_drag_begin;

	
}
void Editor::selection_mouse_drag_end(int p_blocklist,int p_column,int p_row) {
	
	Tick tick=get_cursor().snap_to_ticks( p_row );
	d->selection.mouse_drag_end=EditorData::Selection::Pos(p_blocklist, p_column, tick );
	
	d->selection.begin=d->selection.mouse_drag_begin;
	d->selection.end=d->selection.mouse_drag_end;
	d->selection.enabled=true;
	
	fix_selection();	

}


EditorData::Selection::Pos Editor::get_selection_end_from_pos(EditorData::Selection::Pos p_pos) {
	
	EditorData::Selection::Pos end_pos=p_pos;
	
	ERR_FAIL_COND_V( d->selection.data.empty(), end_pos );
	
	
	int column=p_pos.column;
	int blocklist=p_pos.blocklist;
	
	end_pos.tick=p_pos.tick+d->selection.data.get_length();
	
	
	
	
	ERR_FAIL_COND_V( blocklist>=get_blocklist_count(), end_pos );
	
	for (int i=0;i<d->selection.data.get_column_count();i++) {
		
		ERR_FAIL_COND_V( blocklist>=get_blocklist_count(), end_pos );
		
		BlockList *bl=get_blocklist(blocklist);
		
		if (dynamic_cast<Track_Pattern*>(bl)) {
		
			ERR_FAIL_COND_V( d->selection.data.get_column( i)->get_type()!=ColumnData::TYPE_PATTERN, end_pos );
			end_pos.column=column;
			end_pos.blocklist=blocklist;
			
			column++;	
			
			if (column>=dynamic_cast<Track_Pattern*>(bl)->get_visible_columns()) {
				column=0;
				blocklist++;
			}
			
		} else if (dynamic_cast<Automation*>(bl)) {
			
			ERR_FAIL_COND_V( d->selection.data.get_column( i)->get_type()!=ColumnData::TYPE_AUTOMATION, end_pos );
			
			end_pos.column=column;
			end_pos.blocklist=blocklist;
			column=0;
			blocklist++;
		}
	}
	
	return end_pos;
}


bool Editor::selection_can_paste_at(EditorData::Selection::Pos p_pos) {
	
	if (d->selection.data.empty())
		return false;
	
	int column=p_pos.column;
	int blocklist=p_pos.blocklist;
	
	if (blocklist<0)
		return false;
	if (blocklist>=get_blocklist_count())
		return false;
	
	for (int i=0;i<d->selection.data.get_column_count();i++) {
		
		if (blocklist>=get_blocklist_count())
			return false;
		
		BlockList *bl=get_blocklist(blocklist);
		
		if (dynamic_cast<Track_Pattern*>(bl)) {
		
			if (d->selection.data.get_column( i)->get_type()!=ColumnData::TYPE_PATTERN)
				return false;
			column++;	
			if (column>=dynamic_cast<Track_Pattern*>(bl)->get_visible_columns()) {
				column=0;
				blocklist++;
			}
			
		} else if (dynamic_cast<Automation*>(bl)) {
			
			if (d->selection.data.get_column( i)->get_type()!=ColumnData::TYPE_AUTOMATION)
				return false;
			column=0;
			blocklist++;
		}
	}
	
	return true;
}

void Editor::selection_copy() {
	
	Tick tick_from=d->selection.begin.tick;
	Tick tick_to=d->selection.end.tick+TICKS_PER_BEAT/d->cursor.get_snap();
	
	d->selection.data.clear(); //byebye old data
	for (int i=d->selection.begin.blocklist;i<=d->selection.end.blocklist;i++) {
		
		BlockList *bl=get_blocklist(i);
		
		if (dynamic_cast<Track_Pattern*>(bl)) {
			Track_Pattern *tp=dynamic_cast<Track_Pattern*>(bl);
			int columns=tp->get_visible_columns();
			int column_from=(i==0)?d->selection.begin.column:0;
			int column_to=(i==d->selection.end.blocklist)?d->selection.end.column:(columns-1);
						
			for (int j=column_from;j<=column_to;j++) {
				
				ColumnDataPattern *cdp = new ColumnDataPattern;
				d->selection.data.add_column_data( cdp );
				
				int block_from,block_to;
				if (tp->get_blocks_in_rage( tick_from, tick_to, &block_from,&block_to))
					continue;
				for (int k=block_from;k<=block_to;k++) {
					
					Track_Pattern::PatternBlock *pb=tp->get_block( k );
					
					for (int l=0;l<pb->get_note_count();l++) {
								
				
						ColumnDataPattern::NoteData nd;
						nd.note=pb->get_note( l );
						nd.tick=pb->get_note_pos( l ).tick+tp->get_block_pos(k);
						if (pb->get_note_pos( l ).column!=j)
							continue;
						if (nd.tick<tick_from || nd.tick >tick_to)
							continue;
						
						nd.tick-=tick_from;
						cdp->notes.push_back(nd);						
					}
				}
			}
			
		} else if (dynamic_cast<Automation*>(bl)) {
			
			Automation *au=dynamic_cast<Automation*>(bl);
			
			ColumnDataAutomation *cda = new ColumnDataAutomation;
			d->selection.data.add_column_data( cda );
			
			int block_from,block_to;
			if (au->get_blocks_in_rage( tick_from, tick_to, &block_from,&block_to))
				continue;
			
			for (int k=block_from;k<=block_to;k++) {
					
				Automation::AutomationData *ad=au->get_block( k )->get_data();
					
				for (int l=0;l<ad->get_stream_size();l++) {
								
				
					ColumnDataAutomation::PointData pd;
					pd.point=ad->get_index_value( l );
					pd.tick=ad->get_index_pos( l )+au->get_block_pos(k);
					if (pd.tick<tick_from || pd.tick >tick_to)
						continue;
					pd.tick-=tick_from;
					cda->points.push_back(pd);						
				}
			}
		} else {
			d->selection.data.clear();
			ERR_PRINT("Unknown BlockList Type");
			return;
			
		}
	}
	
	d->selection.data.set_length( tick_to-tick_from );
}

void Editor::selection_clear_area(EditorData::Selection::Pos p_from,EditorData::Selection::Pos p_to) {
	
	
	Tick tick_from=p_from.tick;
	Tick tick_to=p_to.tick+TICKS_PER_BEAT/d->cursor.get_snap()-1; //until next row, but not next row
	d->undo_stream.begin("Selection Zap");
	

	for (int i=p_from.blocklist;i<=p_to.blocklist;i++) {
		
		BlockList *bl=get_blocklist(i);
		
		if (dynamic_cast<Track_Pattern*>(bl)) {
			
			Track_Pattern *tp=dynamic_cast<Track_Pattern*>(bl);
			int columns=tp->get_visible_columns();
			int column_from=(i==p_from.blocklist)?p_from.column:0;
			int column_to=(i==p_to.blocklist)?p_to.column:(columns-1);
						
			std::list<Track_Pattern::Position> note_list;
			
			for (int j=column_from;j<=column_to;j++) {
				
		
				int block_from,block_to;
				if (tp->get_blocks_in_rage( tick_from, tick_to, &block_from,&block_to))
					continue;
				for (int k=block_from;k<=block_to;k++) {
					
					Track_Pattern::PatternBlock *pb=tp->get_block( k );
					
					for (int l=0;l<pb->get_note_count();l++) {
								
				
						Track_Pattern::Position notepos;
						notepos=pb->get_note_pos( l );
						notepos.tick+=tp->get_block_pos(k);
						if (notepos.tick<tick_from || notepos.tick >tick_to || notepos.column!=j)
							continue;
						
						note_list.push_back(notepos);
					}
				}
			}
			
			Track_Pattern *pattern_track=tp; //needed for macro
			std::list<Track_Pattern::Position>::iterator I=note_list.begin();
			for (;I!=note_list.end();I++) {
				
				SET_NOTE(*I,Track_Pattern::Note());
			}
			
		} else if (dynamic_cast<Automation*>(bl)) {
			
			Automation *au=dynamic_cast<Automation*>(bl);
			
			
			int block_from,block_to;
			if (au->get_blocks_in_rage( tick_from, tick_to, &block_from,&block_to))
				continue;
			
			for (int k=block_from;k<=block_to;k++) {
					
				Automation::AutomationData *ad=au->get_block( k )->get_data();
					
				for (int l=0;l<ad->get_stream_size();l++) {
								
					Tick tick=ad->get_index_pos( l );
					tick+=au->get_block_pos(k);
					if (tick<tick_from || tick >tick_to)
						continue;
					d->undo_stream.add_command( Command3(&commands,&EditorCommands::remove_automation_point,au,k,l) );

					l--; //since we deleted, size is minus one
				}
			}
			
		} else {
			
			d->undo_stream.end();
			ERR_PRINT("Unknown BlockList Type");
			return;
			
		}
	}
	
	d->undo_stream.end();
	d->ui_update_notify->notify_action( d->undo_stream.get_current_action_text() );
	
}


void Editor::selection_transpose(bool p_up) {
	
	if (!d->selection.enabled)
		return;
        printf("Transpose Begin\n");
	
	Tick tick_from=d->selection.begin.tick;
	Tick tick_to=d->selection.end.tick+TICKS_PER_BEAT/d->cursor.get_snap();
	
	d->undo_stream.begin(String("Selection Transpose ")+(p_up?"Up":"Down"));
	
	for (int i=d->selection.begin.blocklist;i<=d->selection.end.blocklist;i++) {
		
		BlockList *bl=get_blocklist(i);
		
		if (dynamic_cast<Track_Pattern*>(bl)) {
			Track_Pattern *tp=dynamic_cast<Track_Pattern*>(bl);
			int columns=tp->get_visible_columns();
			int column_from=(i==0)?d->selection.begin.column:0;
			int column_to=(i==d->selection.end.blocklist)?d->selection.end.column:(columns-1);
						
			for (int j=column_from;j<=column_to;j++) {
				
				int block_from,block_to;
				if (tp->get_blocks_in_rage( tick_from, tick_to, &block_from,&block_to))
					continue;
				for (int k=block_from;k<=block_to;k++) {
					
					Track_Pattern::PatternBlock *pb=tp->get_block( k );
					
					for (int l=0;l<pb->get_note_count();l++) {
								
						Track_Pattern::Note note=pb->get_note( l );;
						Track_Pattern::Position pos=pb->get_note_pos( l );
						pos.tick+=tp->get_block_pos(k);	
				
						if (pos.column!=j)
							continue;
						if (pos.tick<tick_from || pos.tick >tick_to)
							continue;
						
						Track_Pattern *pattern_track=tp;
							
						if (p_up) {
							
							if (note.note>=(Track_Pattern::Note::MAX_NOTES-1))
								continue;
							note.note++;
							
							SET_NOTE(pos,note);	
							
						} else {
							
							if (note.note==0)
								continue;
							
							note.note--;
							SET_NOTE(pos,note);	
						}
					}
				}
			}
		}
	}
	
	d->undo_stream.end();
        d->ui_update_notify->notify_action( d->undo_stream.get_current_action_text() );
        printf("Transpose End\n");

}


void Editor::selection_paste_at(EditorData::Selection::Pos p_pos) {
	
	if (d->selection.data.empty())
		return;
	
	int column=p_pos.column;
	int blocklist=p_pos.blocklist;
	
	if (blocklist>=get_blocklist_count())
		return; 
	
	d->undo_stream.begin("Selection Paste");
	
	for (int i=0;i<d->selection.data.get_column_count();i++) {
		
		if (blocklist>=get_blocklist_count())
			break;
		
		BlockList *bl=get_blocklist(blocklist);
		
		if (dynamic_cast<Track_Pattern*>(bl)) {
		
			Track_Pattern *pattern_track=dynamic_cast<Track_Pattern*>(bl);
			ERR_CONTINUE(!pattern_track);
			
			ColumnDataPattern *cdp = dynamic_cast<ColumnDataPattern *>(d->selection.data.get_column( i ));
			
			ERR_CONTINUE( cdp==NULL );
			
			foreach(I,cdp->notes) {
				
				if (pattern_track->get_block_idx_at_pos( I->tick+p_pos.tick )<0)
					continue; //nowhere to set note
				Track_Pattern::Position pp;
				pp.tick=I->tick+p_pos.tick;
				pp.column=column;
				SET_NOTE( pp, I->note);
			}
			
			column++;	
			if (column>=dynamic_cast<Track_Pattern*>(bl)->get_visible_columns()) {
				column=0;
				blocklist++;
			}
			
		} else if (dynamic_cast<Automation*>(bl)) {
			
			
			Automation *au=dynamic_cast<Automation*>(bl);
			ERR_CONTINUE(au==NULL);
			
			ColumnDataAutomation *cda = dynamic_cast<ColumnDataAutomation *>(d->selection.data.get_column( i ));
			
			ERR_CONTINUE( cda==NULL );
			
			foreach(I,cda->points) {
				
				int block_idx=au->get_block_idx_at_pos( I->tick+p_pos.tick );
				if (block_idx<0)
					continue; //nowhere to set note

				add_automation_point(au,I->tick+p_pos.tick,I->point.value,I->point.lfo_depth);
			}
			
			column=0;
			blocklist++;
		}
	}
	
	d->undo_stream.end();
	d->ui_update_notify->notify_action( d->undo_stream.get_current_action_text() );
	
}

void Editor::begin_check_shift_selection() {
	
	printf("begin shift selecting %i\n",d->selection.shift_selecting);
	
	
	if (d->selection.enabled) {
		/* if the cursor is in some place where shift selection wont work, invalidate it */
		
		EditorData::Selection::Pos top_right=d->selection.end;
		top_right.tick=d->selection.begin.tick;
		EditorData::Selection::Pos bottom_left=d->selection.begin;
		bottom_left.tick=d->selection.end.tick;
		EditorData::Selection::Pos cursor=get_cursor_selection_pos();
		
		if ((d->selection.end!=cursor && d->selection.begin!=cursor && top_right!=cursor && bottom_left!=cursor)) {
			printf("changing valid to false from %i\n",d->selection.shift_selection_valid);
			d->selection.shift_selection_valid=false;		
		}		
	}
	
	if (!d->selection.shift_selecting)
		return;
	
	if (!d->selection.enabled || !d->selection.shift_selection_valid) {
		
		d->selection.begin=get_cursor_selection_pos();
		d->selection.end=get_cursor_selection_pos();
		d->selection.enabled=true;
		d->selection.shift_selection_begin=get_cursor_selection_pos();
		d->selection.shift_selection_valid=true;
	}
}
void Editor::end_check_shift_selection() {
	
	printf("end shift selecting %i\n",d->selection.shift_selecting);
	if (!d->selection.shift_selecting || !d->selection.shift_selection_valid)
		return;
	
	d->selection.begin=d->selection.shift_selection_begin;
	d->selection.end=get_cursor_selection_pos();
	
	fix_selection();
	
	d->ui_update_notify->edit_window_changed();
}


bool Editor::test_key_press_selecting(int &p_keypress) {
	
	d->selection.shift_selecting=false;	
	SWITCH(p_keypress)
	
		CASE( KEYBIND("editor/selection_up") ) p_keypress=KEYBIND("up");
		CASE( KEYBIND("editor/selection_down") ) p_keypress=KEYBIND("down");
		CASE( KEYBIND("editor/selection_left") ) p_keypress=KEYBIND("left");
		CASE( KEYBIND("editor/selection_right") ) p_keypress=KEYBIND("right");
		CASE( KEYBIND("editor/selection_page_up") ) p_keypress=KEYBIND("page_up");
		CASE( KEYBIND("editor/selection_page_down") ) p_keypress=KEYBIND("page_down");
		CASE( KEYBIND("editor/selection_move_home") ) p_keypress=KEYBIND("home");
		CASE( KEYBIND("editor/selection_move_end") ) p_keypress=KEYBIND("end");
		DEFAULT
				return false; //not shift-selecting
		
	END_SWITCH
	d->selection.shift_selecting=true; //shift selecting

	return true;
	
	
}

void Editor::fix_selection() {
	
	if (!d->selection.enabled)
		return;
	
	if ( CMP2_MIN( d->selection.end.blocklist,d->selection.end.column,d->selection.begin.blocklist,d->selection.begin.column) ) { //invert
		
		int aux_bl,aux_col;
		
		aux_bl=d->selection.end.blocklist;
		aux_col=d->selection.end.column;
		
		d->selection.end.blocklist=d->selection.begin.blocklist;
		d->selection.end.column=d->selection.begin.column;
		
		d->selection.begin.blocklist=aux_bl;
		d->selection.begin.column=aux_col;
	
	}
		
	if (d->selection.end.tick<d->selection.begin.tick) {
		
		Tick aux_tick=d->selection.begin.tick;
		d->selection.begin.tick=d->selection.end.tick;
		d->selection.end.tick=aux_tick;
	}
}

int Editor::get_selection_begin_column() {
	
	return d->selection.begin.column;
	
}
int Editor::get_selection_begin_row() {
	
	return d->cursor.ticks_to_snap( d->selection.begin.tick );
	
	
}
int Editor::get_selection_begin_blocklist() {
	
	return d->selection.begin.blocklist;
	
	
}

int Editor::get_selection_end_column() {
	
	return d->selection.end.column;
	
	
}
int Editor::get_selection_end_row() {
	
	return d->cursor.ticks_to_snap( d->selection.end.tick );
	
}
int Editor::get_selection_end_blocklist() {
	
	
	return d->selection.end.blocklist;
}


bool Editor::is_selection_active() {
	
	return d->selection.enabled;
				
}
void Editor::disable_selection() {
	
	if (!d->selection.enabled)
		return;
	d->selection.enabled=false;
	d->ui_update_notify->edit_window_changed();
	//update stuff
}


EditorData::Selection::Pos Editor::get_cursor_selection_pos() {
	
	EditorData::Selection::Pos p;
	p.tick=d->cursor.get_tick_pos();
	p.blocklist=d->global_edit.current_blocklist;
		
	if (dynamic_cast<Track_Pattern*>(get_blocklist(get_current_blocklist()))) {
		/* Pattern */
		
		p.column=d->pattern_edit.column;
		
	} else {
		
		p.column=0;
	}
	
	return p;
}



void Editor::selection_begin() {
	
	
	if (!d->selection.enabled) {
				
		d->selection.begin=get_cursor_selection_pos();
		d->selection.end=get_cursor_selection_pos();
		d->selection.enabled=true;
	} else {
				
		d->selection.begin=get_cursor_selection_pos();
	}
			
	fix_selection();			
	d->ui_update_notify->edit_window_changed();
	
}
void Editor::selection_end() {
	
	
	if (!d->selection.enabled) {
				
		d->selection.begin=get_cursor_selection_pos();
		d->selection.end=get_cursor_selection_pos();
		d->selection.enabled=true;
				
	} else {
				
		d->selection.end=get_cursor_selection_pos();
	}
			
	fix_selection();
	d->ui_update_notify->edit_window_changed();

}
void Editor::selection_column_all() {
	
	
}
void Editor::selection_zap() {
	
	if (get_blocklist_count()==0)
		return;
	if (d->selection.enabled) {
				
		selection_clear_area( d->selection.begin, d->selection.end );
	}
	
}
void Editor::selection_paste() {
	
	
}
void Editor::selection_paste_insert() {
	
	
}
void Editor::selection_paste_overwrite() {
	
	if (selection_can_paste_at( get_cursor_selection_pos() )) {
				
		d->undo_stream.begin("Paste Overwrite");
		selection_clear_area( get_cursor_selection_pos(), get_selection_end_from_pos( get_cursor_selection_pos()) );
		selection_paste_at( get_cursor_selection_pos() );
		d->undo_stream.end();
		d->ui_update_notify->notify_action( d->undo_stream.get_current_action_text() );
	}
	
}
void Editor::selection_paste_mix() {
	
			
	if (selection_can_paste_at( get_cursor_selection_pos() )) {
			
		d->undo_stream.begin("Paste Mix");
		selection_paste_at( get_cursor_selection_pos() );
		d->undo_stream.end();
		d->ui_update_notify->notify_action( d->undo_stream.get_current_action_text() );
	}
	
}

bool Editor::selection_can_paste_at_cursor() {
	
	return selection_can_paste_at( get_cursor_selection_pos() );
}

} //end of namespace
