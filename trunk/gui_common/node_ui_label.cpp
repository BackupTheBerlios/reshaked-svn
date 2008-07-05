//
// C++ Implementation: node_ui_label
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "node_ui_label.h"
#include "gui_common/common_skin.h"
#include "base/painter.h"

GUI::Size NodeUI_Label::get_minimum_size_internal() {

	GUI::Size minsize=get_painter()->get_stylebox_min_size( stylebox( SB_NODEUI_LABEL ) );
	minsize.x+=get_painter()->get_font_height( font( FONT_NODEUI_LABEL ) );
	
	return minsize;
}

void NodeUI_Label::draw(const GUI::Point& p_pos,const GUI::Size& p_size,const GUI::Rect& p_exposed) {

	get_painter()->draw_stylebox( stylebox( SB_NODEUI_LABEL ), GUI::Point(), p_size );
	GUI::Point margin( get_painter()->get_stylebox_margin( stylebox( SB_NODEUI_LABEL ), GUI::MARGIN_LEFT ),get_painter()->get_stylebox_margin( stylebox( SB_NODEUI_LABEL ), GUI::MARGIN_TOP ) );
	int limit_x=get_size_cache().width-get_painter()->get_stylebox_min_size( stylebox( SB_NODEUI_LABEL ) ).width;
	
	get_painter()->draw_text( font( FONT_NODEUI_LABEL ), margin, name, color( COLOR_NODEUI_LABEL_FONT ), limit_x );

}

void NodeUI_Label::set_text(String p_text) {

	name=p_text;
	set_tooltip(p_text);
	update();
}

NodeUI_Label::NodeUI_Label() {

}


NodeUI_Label::~NodeUI_Label()
{
}


