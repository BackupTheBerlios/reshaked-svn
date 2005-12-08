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


BlockListUI_Base::BlockListUI_Base(QWidget *p_parent) : QWidget(p_parent) {
	
	current_line=0;
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

BlockListUI_Base::~BlockListUI_Base() {
	
}


}
