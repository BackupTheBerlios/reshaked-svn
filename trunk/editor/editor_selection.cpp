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


void Editor::begin_check_shift_selection() {
	
	if (d->selection.enabled==false || d->selection.end!=get_cursor_selection_pos() || d->selection.begin!=get_cursor_selection_pos()) {
		
		d->selection.begin=get_cursor_selection_pos();
		d->selection.end=get_cursor_selection_pos();
		d->selection.enabled=true;
	}
}
void Editor::end_check_shift_selection() {
	
	
}


bool Editor::test_key_press_selecting(int &p_keypress) {
	
	SWITCH(p_keypress)
	
		CASE( KEYBIND("editor/selection_up") ) p_keypress=KEYBIND("up");
		CASE( KEYBIND("editor/selection_down") ) p_keypress=KEYBIND("down");
		CASE( KEYBIND("editor/selection_left") ) p_keypress=KEYBIND("left");
		CASE( KEYBIND("editor/selection_right") ) p_keypress=KEYBIND("right");
		CASE( KEYBIND("editor/selection_page_up") ) p_keypress=KEYBIND("page_up");
		CASE( KEYBIND("editor/selection_page_down") ) p_keypress=KEYBIND("page_down");
		CASE( KEYBIND("editor/selection_home") ) p_keypress=KEYBIND("home");
		CASE( KEYBIND("editor/selection_end") ) p_keypress=KEYBIND("end");
		DEFAULT
			return false;
		
	END_SWITCH
			
	return true;
	
	
}

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