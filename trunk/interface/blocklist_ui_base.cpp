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
#include "blocklist_ui_base.h"
#include "ui_blocks/visual_settings.h"
namespace ReShaked {



void BlockListUI_Base::set_blocklist_ui_under_cursor(BlockListUI_Base *p_ui, QPoint p_pos) {
	
	
	int bl,column,row;
	p_ui->get_pos_at_pointer(p_pos, &bl,&column,&row);
	
	_editor->selection_mouse_drag_end(bl,column,row);
	
}


void BlockListUI_Base::mouse_selection_begin(QPoint p_pos) {
	
	_mouse_selecting=true;	
	int bl,column,row;
	get_pos_at_pointer(p_pos, &bl,&column,&row);
	
	_editor->selection_mouse_drag_begin(bl,column,row);
	
}


void BlockListUI_Base::mouse_selection_update_check() {
	
	if (!_mouse_selecting)
		return;
	
	blocklist_ui_under_cursor_request_signal(this);
	_editor->get_ui_update_notify()->block_layout_changed();	
}


void BlockListUI_Base::mouse_selection_end() {
	
	_mouse_selecting=false;	
	
}


void BlockListUI_Base::fix_pre_scroll(int p_scroll) {
	
	
	
}

bool BlockListUI_Base::can_scroll() {
	
	return false;
}

int BlockListUI_Base::get_current_line() {
	
	return current_line;	
}

void BlockListUI_Base::set_current_line(int p_line) {
	
	
	current_line=p_line;
}


void BlockListUI_Base::update_viewport_pos(int p_new_line) {
	
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
	int scroll_pixels=-VisualSettings::get_singleton()->get_editing_row_height()*diff;
//	printf("scroll %i pixels\n",scroll_pixels);
	scroll(0,scroll_pixels);
	
	current_line=p_new_line;
	
}


BlockListUI_Base::BlockListUI_Base(QWidget *p_parent, Editor *p_editor) : QWidget(p_parent) {
	
	_editor=p_editor;
	current_line=0;
	_mouse_selecting=false;	
}


BlockListUI_Base::~BlockListUI_Base() {
	
}


}
