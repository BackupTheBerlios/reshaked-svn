//
// C++ Implementation: node_ui_enum
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "node_ui_enum.h"

#include "gui_common/common_skin.h"
#include "base/painter.h"
#include <stdio.h>

void NodeUI_Enum::value_entered(double p_val) {

	get_range()->set( p_val );
	update();	
}


GUI::Size NodeUI_Enum::get_minimum_size_internal() {

	GUI::Size minsize=get_painter()->get_stylebox_min_size( stylebox( SB_NODEUI_ENUM ) );
	minsize.y+=get_painter()->get_font_height( font( FONT_NODEUI_ENUM ) );
	minsize.x+=get_painter()->get_bitmap_size( bitmap( BITMAP_ICON_ENUM_LEFT ) ).width;
	minsize.x+=get_painter()->get_bitmap_size( bitmap( BITMAP_ICON_ENUM_RIGHT ) ).width;
	minsize.x+=constant(C_GENERIC_NODE_UI_ENUM_MIN_WIDTH);

	int h = get_painter()->get_bitmap_size( bitmap( BITMAP_ICON_ENUM_RIGHT ) ).height;
	if (h>minsize.y)
		minsize.y=h;
	return minsize;
}

void NodeUI_Enum::mouse_button(const GUI::Point& p_pos, int p_button,bool p_press,int p_modifier_mask) {
	
	
	if (p_press) {
	
		if (p_button==GUI::BUTTON_WHEEL_UP) {
		
			get_range()->step_increment();
			
		} else if (p_button==GUI::BUTTON_WHEEL_DOWN) {
		
			get_range()->step_decrement();
		} else if (p_button==GUI::BUTTON_LEFT) {
				
			GUI::Size arrowsize=get_painter()->get_bitmap_size( bitmap( BITMAP_ICON_ENUM_LEFT ) );
			
			
			if ( p_pos.x < arrowsize.width ) {
			
				get_range()->step_decrement();
				pressing_left=true;
				update();
			} else if ( p_pos.x > get_size_cache().width-arrowsize.width ) {
			
				get_range()->step_increment();
				pressing_right=true;	
				update();
			}
		}
	} else {
	
		if (p_button==GUI::BUTTON_LEFT) {
			pressing_right=false;
			pressing_left=false;
			update();
		}
	}
}

void NodeUI_Enum::draw(const GUI::Point& p_pos,const GUI::Size& p_size,const GUI::Rect& p_exposed) {

	GUI::Rect text_rect( GUI::Point(), p_size);
	GUI::Size arrowsize=get_painter()->get_bitmap_size( bitmap( BITMAP_ICON_ENUM_LEFT ) );
	GUI::Size text_sb_size = get_painter()->get_stylebox_min_size( stylebox( SB_NODEUI_ENUM ) );
	text_rect.pos.x+=arrowsize.width;
	text_rect.size.x-=arrowsize.width*2;
	
	int text_h=get_painter()->get_font_height( font( FONT_NODEUI_ENUM ) )+text_sb_size.height;
	int h = MAX( text_h, arrowsize.height );
	
	int arrow_hofs=(h-arrowsize.height)/2;
	get_painter()->draw_bitmap( bitmap( pressing_left?BITMAP_ICON_ENUM_LEFT_ACTIVE:BITMAP_ICON_ENUM_LEFT ), GUI::Point(0,arrow_hofs) );
	get_painter()->draw_bitmap( bitmap( pressing_right?BITMAP_ICON_ENUM_RIGHT_ACTIVE:BITMAP_ICON_ENUM_RIGHT ), GUI::Point(p_size.x-arrowsize.x,arrow_hofs) );

	get_painter()->draw_stylebox( stylebox( SB_NODEUI_ENUM ), text_rect.pos, text_rect.size );
	
	GUI::Point margin( get_painter()->get_stylebox_margin( stylebox( SB_NODEUI_ENUM ), GUI::MARGIN_LEFT ),get_painter()->get_stylebox_margin( stylebox( SB_NODEUI_ENUM ), GUI::MARGIN_TOP ) );
	
	text_rect.pos+=margin;
	text_rect.size-=text_sb_size;
	text_rect.pos.y+=(h-text_h)/2;
	String text=get_range()->get_as_text();
	
	int limit_x=text_rect.size.x;
	int text_w=get_painter()->get_font_string_width( font( FONT_NODEUI_ENUM ), text );
	
	int ofs = (limit_x - text_w)/2;
	if (ofs<0)
		ofs=0;
		
	text_rect.pos.x+=ofs;
	text_rect.pos.y+=get_painter()->get_font_ascent( font( FONT_NODEUI_ENUM ) );
	
	get_painter()->draw_text( font( FONT_NODEUI_ENUM ), text_rect.pos, text, color( COLOR_NODEUI_ENUM_FONT ), limit_x );

}

void NodeUI_Enum::range_changed() {

	update();
}
void NodeUI_Enum::value_changed(double p_new_val) {


	update();
}

void NodeUI_Enum::set_in_window() {


}

NodeUI_Enum::NodeUI_Enum() {

	pressing_left=false;
	pressing_right=false;
	
}


NodeUI_Enum::~NodeUI_Enum()
{


}

