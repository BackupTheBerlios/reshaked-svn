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


namespace ReShaked {

static Track_Pattern::Note _fix_macro(const Track_Pattern::Note &p_note) { return p_note; } //fix template annoyance
#define SET_NOTE(m_pos,m_note) d->undo_stream.add_command( Command3(&commands,&EditorCommands::set_note,pattern_track,m_pos,_fix_macro(m_note)) )

bool Editor::selection_can_paste_at(EditorData::Selection::Pos p_pos) {
	
	if (d->selection.data.empty())
		return false;
	
	int column=p_pos.column;
	int blocklist=p_pos.blocklist;
	
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
}

void Editor::selection_clear_area(EditorData::Selection::Pos p_from,EditorData::Selection::Pos p_to) {
	
	Tick tick_from=p_from.tick;
	Tick tick_to=p_to.tick+TICKS_PER_BEAT/d->cursor.get_snap()-1; //until next row, but not next row
	
	d->undo_stream.begin("Selection Zap");
	
	d->selection.data.clear(); //byebye old data
	for (int i=p_from.blocklist;i<=p_to.blocklist;i++) {
		
		BlockList *bl=get_blocklist(i);
		
		if (dynamic_cast<Track_Pattern*>(bl)) {
			
			Track_Pattern *tp=dynamic_cast<Track_Pattern*>(bl);
			int columns=tp->get_visible_columns();
			int column_from=(i==0)?p_from.column:0;
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
						if (notepos.tick<tick_from || notepos.tick >tick_to)
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
	
}


void Editor::selection_paste_at(EditorData::Selection::Pos p_pos) {
	/*
	if (d->selection.data.empty())
		return;
	
	int column=p_pos.column;
	int blocklist=p_pos.blocklist;
	
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
	*/
	
}

void Editor::begin_check_shift_selection() {
	
	if (!d->selection.shift_selecting)
		return;
	
	EditorData::Selection::Pos top_right=d->selection.end;
	top_right.tick=d->selection.begin.tick;
	EditorData::Selection::Pos bottom_left=d->selection.begin;
	bottom_left.tick=d->selection.end.tick;
	EditorData::Selection::Pos cursor=get_cursor_selection_pos();
	
	if (d->selection.enabled==false || (d->selection.end!=cursor && d->selection.begin!=cursor && top_right!=cursor && bottom_left!=cursor)) {
			
		d->selection.begin=cursor;
		d->selection.end=cursor;
		d->selection.enabled=true;
	
	}
		
	d->selection.shift_selecton_change_blocklist_column=(cursor.blocklist==d->selection.begin.blocklist && cursor.column==d->selection.begin.column)?EditorData::Selection::CHANGE_BEGIN:EditorData::Selection::CHANGE_END;
	d->selection.shift_selecton_change_tick=(cursor.tick==d->selection.begin.tick)?EditorData::Selection::CHANGE_BEGIN:EditorData::Selection::CHANGE_END;
}
void Editor::end_check_shift_selection() {
	
	if (!d->selection.shift_selecting)
		return;
	
	EditorData::Selection::Pos cursor=get_cursor_selection_pos();
	
	if (d->selection.shift_selecton_change_blocklist_column==EditorData::Selection::CHANGE_BEGIN) {
		d->selection.begin.blocklist=cursor.blocklist;
		d->selection.begin.column=cursor.column;
		
	} else {
		d->selection.end.blocklist=cursor.blocklist;
		d->selection.end.column=cursor.column;
		
	}
	
	if (d->selection.shift_selecton_change_tick==EditorData::Selection::CHANGE_BEGIN) {
		d->selection.begin.tick=cursor.tick;

		
	} else {
		d->selection.end.tick=cursor.tick;	
	}
	
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






} //end of namespace