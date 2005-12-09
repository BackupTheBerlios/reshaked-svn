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


namespace ReShaked {


void Editor::fix_selection() {
	
		
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
	
	d->selection.enabled=false;
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