//
// C++ Implementation: track_editor
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "track_editor.h"
#include "base/painter.h"
#include "gui_common/common_skin.h"

int TrackEditor::get_row_height() {
	
	int height=get_painter()->get_font_height( font(FONT_TRACK_EDITOR) )+constant(C_TRACK_EDITOR_VOL_NOTE_BAR_HEIGHT)+constant(C_TRACK_EDITOR_ROW_MARGIN)*2;

	return height;
	
}

void TrackEditor::pre_parse_mouse_button_event(const GUI::Point& p_pos, int p_button,bool p_press,int p_modifier_mask) {
	
	if (!p_press)
		return;
	
	if (p_button==GUI::BUTTON_WHEEL_UP)
		Editor::get_singleton()->set_window_offset( Editor::get_singleton()->get_window_offset() -1 );
	else if (p_button==GUI::BUTTON_WHEEL_UP)
		Editor::get_singleton()->set_window_offset( Editor::get_singleton()->get_window_offset() +1 );
	
}

void TrackEditor::set_track_editor_under_cursor(TrackEditor *p_track_editor, const GUI::Point& p_pos) {
	
	
	int column,row;
	p_track_editor->get_column_and_row_at_pos(p_pos, &column,&row);
	
	//_editor->selection_mouse_drag_end(p_track_editor,column,row);
	
	int track = Editor::get_singleton()->get_song()->find_track_pos( _track );
	Editor::get_singleton()->set_cursor_track( track );
	Editor::get_singleton()->set_cursor_col( column );
	Editor::get_singleton()->set_cursor_row( row );
	
	update();	
}


void TrackEditor::mouse_selection_begin(const GUI::Point p_pos) {
	
	_mouse_selecting=true;	
	int column,row;
	get_column_and_row_at_pos(p_pos, &column,&row);
		
	//Editor::get_singleton()->selection_mouse_drag_begin(bl,column,row);
	

}


void TrackEditor::mouse_selection_update_check() {
	
	if (!_mouse_selecting)
		return;
	
	track_editor_under_cursor_request_signal.call(this);
	//Editor::get_singleton()->get_ui_update_notify()->block_layout_changed();	
	

}


void TrackEditor::mouse_selection_end() {
	
	_mouse_selecting=false;	

	
}


TrackEditor::TrackEditor(Track *p_track)  {
	

	_track=p_track;
	_mouse_selecting=false;	
}


TrackEditor::~TrackEditor() {
	
}

