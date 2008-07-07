
//
// C++ Implementation: node_ui_value
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "node_ui_value.h"
#include "gui_common/common_skin.h"
#include "base/painter.h"

GUI::Size NodeUI_Value::get_minimum_size_internal() {

	GUI::Size minsize=get_painter()->get_stylebox_min_size( stylebox( SB_NODEUI_VALUE ) );
	minsize.y+=get_painter()->get_font_height( font( FONT_NODEUI_VALUE ) );
	
	return minsize;
}

void NodeUI_Value::draw(const GUI::Point& p_pos,const GUI::Size& p_size,const GUI::Rect& p_exposed) {

	get_painter()->draw_stylebox( stylebox( SB_NODEUI_VALUE ), GUI::Point(), p_size );
	GUI::Point margin( get_painter()->get_stylebox_margin( stylebox( SB_NODEUI_VALUE ), GUI::MARGIN_LEFT ),get_painter()->get_stylebox_margin( stylebox( SB_NODEUI_VALUE ), GUI::MARGIN_TOP ) );
	String text=get_range()->get_as_text();
	
	int limit_x=get_size_cache().width-get_painter()->get_stylebox_min_size( stylebox( SB_NODEUI_VALUE ) ).width;
	int text_w=get_painter()->get_font_string_width( font( FONT_NODEUI_VALUE ), text );
	int ofs = (limit_x - text_w)/2;
	if (ofs<0)
		ofs=0;
		
	margin.x+=ofs;
	margin.y+=get_painter()->get_font_ascent( font( FONT_NODEUI_VALUE ) );
	
	get_painter()->draw_text( font( FONT_NODEUI_VALUE ), margin, text, color( COLOR_NODEUI_VALUE_FONT ), limit_x );

}

void NodeUI_Value::range_changed() {

	update();
}
void NodeUI_Value::value_changed(double p_new_val) {

	update();
}


NodeUI_Value::NodeUI_Value() {

}


NodeUI_Value::~NodeUI_Value()
{
}


