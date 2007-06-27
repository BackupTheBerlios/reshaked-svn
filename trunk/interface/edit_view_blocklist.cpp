//
// C++ Implementation: blocklist_ui_base
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "edit_view_blocklist.h"

#include "base/painter.h"
#include "gui_custom/rsskin.h"

namespace ReShaked {


int EditViewBlockList::get_row_height() {
	
	int height=get_painter()->get_font_height( font(FONT_PATTERN) )+constant(C_PATTERN_VOL_NOTE_BAR_HEIGHT)+constant(C_EDITOR_ROW_MARGIN)*2;
	

	return height;
	
}

void EditViewBlockList::pre_parse_mouse_button_event(const Point& p_pos, int p_button,bool p_press,int p_modifier_mask) {
	
	if (!p_press)
		return;
	
	if (p_button==BUTTON_WHEEL_UP)
		_editor->get_cursor().set_window_offset( _editor->get_cursor().get_window_offset() -1 );
	else if (p_button==BUTTON_WHEEL_UP)
		_editor->get_cursor().set_window_offset( _editor->get_cursor().get_window_offset() +1 );
	
}

void EditViewBlockList::set_blocklist_ui_under_cursor(EditViewBlockList *p_ui, Point p_pos) {
	
	
	int bl,column,row;
	p_ui->get_pos_at_pointer(p_pos, &bl,&column,&row);
	
	_editor->selection_mouse_drag_end(bl,column,row);
	

	_editor->set_pattern_edit_column( column );
	_editor->get_cursor().set_pos( row );
	if (_editor->get_current_blocklist()!=bl)
		_editor->set_current_blocklist( bl );
	update();
	
}


void EditViewBlockList::mouse_selection_begin(Point p_pos) {
	
	_mouse_selecting=true;	
	int bl,column,row;
	get_pos_at_pointer(p_pos, &bl,&column,&row);
	
	_editor->selection_mouse_drag_begin(bl,column,row);
	

}


void EditViewBlockList::mouse_selection_update_check() {
	
	if (!_mouse_selecting)
		return;
	
	blocklist_ui_under_cursor_request_signal.call(this);
	_editor->get_ui_update_notify()->block_layout_changed();	
	

}


void EditViewBlockList::mouse_selection_end() {
	
	_mouse_selecting=false;	

	
}


void EditViewBlockList::fix_pre_scroll(int p_scroll) {
	
	
	
}

bool EditViewBlockList::can_scroll() {
	
	return false;
}

int EditViewBlockList::get_current_line() {
	
	return current_line;	
}

void EditViewBlockList::set_current_line(int p_line) {
	
	
	current_line=p_line;
}


void EditViewBlockList::update_viewport_pos(int p_new_line) {
	
	if (p_new_line==current_line )
		return;
	
	if (!can_scroll()) {
		update();
		current_line=p_new_line;
//		printf("cant scroll!\n");
		return;
		
	}
		
	int diff=p_new_line-current_line;
	fix_pre_scroll( diff );
	int scroll_pixels=-get_row_height()*diff;
//	printf("scroll %i pixels\n",scroll_pixels);
	//scroll(0,scroll_pixels); -implement when scroll works
	
	current_line=p_new_line;
	
	update();
	
}


EditViewBlockList::EditViewBlockList(Editor *p_editor)  {
	
	_editor=p_editor;
	current_line=0;
	_mouse_selecting=false;	
}


EditViewBlockList::~EditViewBlockList() {
	
}


}
