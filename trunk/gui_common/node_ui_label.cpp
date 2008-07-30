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
#include <stdio.h>

GUI::Size NodeUI_Label::get_minimum_size_internal() {

	if (orientation==HORIZONTAL) {
		GUI::Size minsize=get_painter()->get_stylebox_min_size( stylebox( SB_NODEUI_LABEL ) );
		minsize.y+=get_painter()->get_font_height( font( FONT_NODEUI_LABEL ) );
		return minsize;
	} else {
	
		GUI::Size minsize=get_painter()->get_stylebox_min_size( stylebox( SB_NODEUI_VLABEL ) );
		minsize.x+=get_painter()->get_font_height( font( FONT_NODEUI_LABEL ) );
		return minsize;
	
	}	
}


void NodeUI_Label::draw(const GUI::Point& p_pos,const GUI::Size& p_size,const GUI::Rect& p_exposed) {

	if (orientation==HORIZONTAL) {
		get_painter()->draw_stylebox( stylebox( SB_NODEUI_LABEL ), GUI::Point(), p_size );
		GUI::Point margin( get_painter()->get_stylebox_margin( stylebox( SB_NODEUI_LABEL ), GUI::MARGIN_LEFT ),get_painter()->get_stylebox_margin( stylebox( SB_NODEUI_LABEL ), GUI::MARGIN_TOP ) );
		int limit_x=get_size_cache().width-get_painter()->get_stylebox_min_size( stylebox( SB_NODEUI_LABEL ) ).width;
		int text_w=get_painter()->get_font_string_width( font( FONT_NODEUI_LABEL ), name );
		int ofs = (limit_x - text_w)/2;
		if (ofs<0)
			ofs=0;
			
		margin.x+=ofs;	
		margin.y+=get_painter()->get_font_ascent( font( FONT_NODEUI_LABEL ) );
		
		get_painter()->draw_text(font( FONT_NODEUI_LABEL ), margin, name,  color (COLOR_NODEUI_LABEL_FONT) );
	} else {
	
		get_painter()->draw_stylebox( stylebox( SB_NODEUI_VLABEL ), GUI::Point(), p_size );
		GUI::Point margin( get_painter()->get_stylebox_margin( stylebox( SB_NODEUI_VLABEL ), GUI::MARGIN_LEFT ),get_painter()->get_stylebox_margin( stylebox( SB_NODEUI_VLABEL ), GUI::MARGIN_TOP ) );
		
		int limit_y=get_size_cache().height-get_painter()->get_stylebox_min_size( stylebox( SB_NODEUI_VLABEL ) ).height;
		int text_h=get_painter()->get_font_string_width( font( FONT_NODEUI_LABEL ), name );
		int ofs = (limit_y - text_h)/2;
		if (ofs<0)
			ofs=0;
			
		margin.y+=ofs;	
		margin.x+=get_painter()->get_font_descent( font( FONT_NODEUI_LABEL ) );
		
		get_painter()->draw_text(font( FONT_NODEUI_LABEL ), margin, name, GUI::DOWN,  color (COLOR_NODEUI_LABEL_FONT) );
	
	}
	

}

void NodeUI_Label::set_text(String p_text) {

	name=p_text;
	set_tooltip(p_text);
	check_minimum_size();
	update();
	
}

void NodeUI_Label::set_orientation(Orientation p_orientation) {

	orientation=p_orientation;
	check_minimum_size();
	update();
}
NodeUI_Label::NodeUI_Label(Orientation p_orientation) {

	orientation=p_orientation;
}



NodeUI_Label::~NodeUI_Label()
{
}


