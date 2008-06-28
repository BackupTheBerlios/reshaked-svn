//
// C++ Implementation: global_beatbar_column
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "global_beatbar_column.h"


#include "base/painter.h"
#include "gui_custom/rsskin.h"
namespace ReShaked {

void LoopColumn::set_global_view(GlobalView *p_global_view) {
	
	
	global_view=p_global_view;
	
	
}

Size LoopColumn::get_minimum_size_internal() {
	
	return Size( constant( C_GLOBAL_VIEW_LOOP_COLUMN_WIDTH ), 0 );
}

void LoopColumn::numeric_input_dialog_beat(double p_val) {
	
	editor->bar_length_set( beat_at_click,(int)p_val);
				
	
	redraw_global_view_signal.call();
}

void LoopColumn::option_selected(int p_option) {
	
	int beat=beat_at_click;
	
	switch (p_option) {
		
		case OPTION_SET_LOOP_BEGIN: {
			editor->get_song()->set_loop_begin( beat );
		} break;
		case OPTION_SET_LOOP_END: {
			editor->get_song()->set_loop_end( beat );
		} break;
		case OPTION_CHANGE_BAR_LENGTH: {
					
			int current=editor->get_song()->get_bar_map().get_bar_idx( editor->get_song()->get_bar_map().get_bar_idx_from_beat( beat ) );
						
			input_dialog->show("Enter Bar Length (Beats):",2,16,1,current);
			
		} break;
		case OPTION_REMOVE_BAR_LENGTH: {
			
			editor->bar_length_remove( beat);
								
			
		} break;
	}
		
	update();
	
}
void LoopColumn::mouse_button(const Point& p_pos, int p_button,bool p_press,int p_modifier_mask) {

	if (!p_press || p_button!=BUTTON_LEFT)
		return;
	
	beat_at_click=global_view->get_beat_at_pixel(p_pos.y+global_view->get_beat_pixel_size()/2);

	menu->clear();
	menu->add_item("Set Loop Begin",OPTION_SET_LOOP_BEGIN);
	menu->add_item("Set Loop End",OPTION_SET_LOOP_END);
	menu->add_separator();
	menu->add_item("Change Bar Length",OPTION_CHANGE_BAR_LENGTH);
	menu->add_item("Remove Bar Length",OPTION_REMOVE_BAR_LENGTH);
		
	menu->popup( get_global_pos() + p_pos );
	
}

void LoopColumn::draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed) {

	
	Painter &p=*get_painter();
	p.draw_fill_rect(Point(), p_size,color(COLOR_GLOBAL_VIEW_BG));
	
	int beat_from=global_view->get_beat_at_pixel(0);
	int beat_to=global_view->get_beat_at_pixel(p_size.height);
	
	FontID barfont=font( FONT_GLOBAL_VIEW_BAR );
	FontID beatfont=font( FONT_GLOBAL_VIEW_BEAT );
	Color barcol=color( COLOR_GLOBAL_VIEW_BAR_FONT );
	Color beatcol=color( COLOR_GLOBAL_VIEW_BEAT_FONT );
	
	int begin_pixel=global_view->get_beat_pixel( editor->get_song()->get_loop_begin() );
	int end_pixel=global_view->get_beat_pixel( editor->get_song()->get_loop_end() );
	
	p.draw_fill_rect(Point(0,begin_pixel),Size(p_size.width,end_pixel-begin_pixel),color( COLOR_GLOBAL_VIEW_LOOP ));
		
	
	p.draw_arrow( Point(0,begin_pixel), Size(p_size.width,p_size.width/2),DOWN,color( COLOR_GLOBAL_VIEW_LOOP_MARKER ),false);
	
	p.draw_arrow( Point(0,end_pixel-p_size.width/2), Size(p_size.width,p_size.width/2),UP,color( COLOR_GLOBAL_VIEW_LOOP_MARKER ),false);
	
	if (inside) {
			
		int h=global_view->get_beat_pixel( inside_beat );
		p.draw_fill_rect(Point(0,h),Size(p_size.width,1),color(COLOR_GLOBAL_VIEW_MOUSE_OVER_BEAT));
	}
	
	for (int i=0;i<editor->get_song()->get_bar_map().get_bar_idx_count();i++) {
		
		int beat=editor->get_song()->get_bar_map().get_bar_idx_pos(i);
		if (beat<beat_from || beat>beat_to)
			continue;
		
		
		
		int h=global_view->get_beat_pixel( beat );
		p.draw_fill_rect(Point(0,h),Size(p_size.width,1),color(COLOR_GLOBAL_VIEW_BARBEAT_CHANGE));
		
	}
	
	int line_pos=p_size.width-1;
	p.draw_fill_rect(Point(line_pos,0),Size(1,p_size.height),color(COLOR_GLOBAL_VIEW_SEPARATOR));
}

void LoopColumn::mouse_motion(const Point& p_pos, const Point& p_rel, int p_button_mask) {
	
	inside_beat=global_view->get_beat_at_pixel(p_pos.y+global_view->get_beat_pixel_size()/2);

	inside=true;
	update();
}

void LoopColumn::mouse_leave() {
	
	inside=false;
	update();
	
}

void LoopColumn::set_in_window() {

	input_dialog = new NumericInputDialog( get_window() );
	input_dialog->entered_number_signal.connect( this, &LoopColumn::numeric_input_dialog_beat );
	menu = new PopUpMenu(get_window());
	
	
	menu->selected_id_signal.connect( this, &LoopColumn::option_selected );
}
LoopColumn::LoopColumn(Editor *p_editor)  {
	
	editor=p_editor;
	
	set_bg_on_updates(false);
	//setToolTip("Loop, Bar Length");
	//setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);

	input_dialog=0;
}

LoopColumn::~LoopColumn() {
	
	if (input_dialog)
		delete input_dialog;
}
/***/

void MarkerColumn::set_global_view(GlobalView *p_global_view) {
	
	
	global_view=p_global_view;

	
}

Size MarkerColumn::get_minimum_size_internal() {
	
	int margin=constant(C_GLOBAL_VIEW_MARKER_COLUMN_MARGIN);
	int font_h=get_painter()->get_font_height( font( FONT_GLOBAL_VIEW_MARKER ) );
	return Size(margin*2+font_h,0);
}

void MarkerColumn::paint_marker(Painter& p, int p_marker_idx,int p_w) {
	
	String marker_text=marker_list->get_index_value(p_marker_idx);
	
	FontID fnt=font(FONT_GLOBAL_VIEW_MARKER);
	Color col=color(COLOR_GLOBAL_VIEW_MARKER_FONT);
	
	
	int beat=marker_list->get_index_pos(p_marker_idx);
	int pos=global_view->get_beat_pixel( beat );
	int beat_h=global_view->get_beat_pixel( beat+1 )-pos;
	
	
	
	int margin=constant(C_GLOBAL_VIEW_MARKER_COLUMN_MARGIN);
	p.draw_arrow( Point(0,pos-p_w/2+constant(margin)), Size( p_w/2, p_w ), RIGHT, color( COLOR_GLOBAL_VIEW_LOOP));
	
	// Draw Text 
	p.draw_text( fnt,Point(p.get_font_descent(fnt)+margin,pos+p_w/2), marker_text, DOWN, col );
	
}

void MarkerColumn::mouse_button(const Point& p_pos, int p_button,bool p_press,int p_modifier_mask) {
	
	int beat=global_view->get_beat_at_pixel(p_pos.y+global_view->get_beat_pixel_size()/2);
	int marker_idx=marker_list->get_exact_index( beat );
	
	if (p_button==BUTTON_RIGHT || (p_button==BUTTON_LEFT && p_modifier_mask&KEY_MASK_CTRL)) {
		
		if (marker_idx!=INVALID_STREAM_INDEX) {
			
			//printf("size %i\n",marker_list->get_stream_size());
			editor->marker_remove(marker_idx);
			//printf("removed %i, size %i\n",marker_idx,marker_list->get_stream_size());
			update();
		}
		return;
	}
	
	if (p_button==BUTTON_LEFT) {
		
		String current_text= String(marker_idx>=0?marker_list->get_index_value_w( marker_idx):"" );
		
		marker_beat=beat;
		
		
		input_dialog->show(current_text==""?"Insert Marker:":"Rename Marker",current_text);
	}
	
	
}

void MarkerColumn::marker_edited(String p_text) {
	
	if (p_text=="")
		return;
	
	update();
	editor->marker_set(marker_beat,p_text);

	
}


void MarkerColumn::draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed) {
	
	
	Painter &p=*get_painter();
	p.draw_fill_rect(Point(),p_size,color(COLOR_GLOBAL_VIEW_BG));
	int beat_from=global_view->get_beat_at_pixel(0);
	int beat_to=global_view->get_beat_at_pixel(p_size.height);
	
	
	for (int i=beat_from;i<=beat_to;i++) {
		
		
		// DRAW MARK 
					
		if (i==inside_beat && inside) {
			
			
			int h=global_view->get_beat_pixel( i );
			p.draw_fill_rect(Point(0,h),Size(p_size.width,1),color(COLOR_GLOBAL_VIEW_MOUSE_OVER_BEAT));
			
		}
		
		int marker_idx=marker_list->get_exact_index( i );
		if (marker_idx>=0) { //marker at pos
			paint_marker(p,marker_idx,p_size.width);
			
		}
		
		
	}
	
	// Paint markers that may not be visible 
	if (marker_list->get_exact_index( beat_from )==INVALID_STREAM_INDEX && marker_list->get_prev_index( beat_from )>=0) {
		
		paint_marker(p, marker_list->get_prev_index( beat_from ),p_size.width );
		
	}
	
	int line_pos=p_size.width-1;
	p.draw_fill_rect(Point(line_pos,0),Size(1,p_size.height),color(COLOR_GLOBAL_VIEW_SEPARATOR));
}

void MarkerColumn::mouse_motion(const Point& p_pos, const Point& p_rel, int p_button_mask) {
	
	inside_beat=global_view->get_beat_at_pixel(p_pos.y+global_view->get_beat_pixel_size()/2);

	inside=true;
	update();
}

void MarkerColumn::mouse_leave() {
	
	inside=false;
	update();
	
}

void MarkerColumn::set_in_window() {
	
	input_dialog = new StringInputDialog(get_window());
	input_dialog->entered_string_signal.connect(this,&MarkerColumn::marker_edited);
}

MarkerColumn::MarkerColumn(Editor *p_editor)  {
	
	editor=p_editor;
	bar_map=&p_editor->get_song()->get_bar_map();
	marker_list = &p_editor->get_song()->get_marker_list();
	set_bg_on_updates(false);
	//setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);
//	setToolTip("Markers");
	input_dialog=0;

}

MarkerColumn::~MarkerColumn()  {
	
	if (input_dialog)
		delete input_dialog;
}

/*********************/
/*********************/
/*********************/

void GlobalBeatBarColumn::set_global_view(GlobalView *p_global_view) {
	
	
	global_view=p_global_view;
	
}

Size GlobalBeatBarColumn::get_minimum_size_internal() {

	return Size(get_painter()->get_font_char_width(font( FONT_GLOBAL_VIEW_BAR ),'0')*9,0);
}

void GlobalBeatBarColumn::draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed) {
	
	Painter &p=*get_painter();
	p.draw_fill_rect(Point(), p_size,color(COLOR_GLOBAL_VIEW_BG));
	
	int beat_from=global_view->get_beat_at_pixel(0);
	int beat_to=global_view->get_beat_at_pixel(p_size.height);
	
	FontID barfont=font( FONT_GLOBAL_VIEW_BAR );
	FontID beatfont=font( FONT_GLOBAL_VIEW_BEAT );
	Color barcol=color( COLOR_GLOBAL_VIEW_BAR_FONT );
	Color beatcol=color( COLOR_GLOBAL_VIEW_BEAT_FONT );
	int bar_ascent=p.get_font_ascent( barfont );
	int beat_ascent=p.get_font_ascent( beatfont );
	
	
	for (int i=beat_from;i<=beat_to;i++) {
		
		// DRAW BEAT and optionally BAR *
		
		if (global_view->get_beat_pixel_size()>p.get_font_height(beatfont)) { //paint beat

				
			String s = String::num( bar_map->get_bar_beat( i)+1 );
			int ofs=p_size.width-(s.length()+1)*p.get_font_char_width(beatfont,'0');
			
			p.draw_text( beatfont, Point( ofs, global_view->get_beat_pixel( i )+beat_ascent ), s, beatcol );
		}
		
		if (!bar_map->get_bar_beat( i )) { // paint bar
			
			String s = String::num(bar_map->get_bar_at_beat( i)+1 );
			int ofs=p_size.width-(s.length()+3)*p.get_font_char_width(barfont,'0');
			p.draw_text( barfont, Point(ofs, global_view->get_beat_pixel( i )+beat_ascent), s, barcol );
		}
		
			
	}
	
	
	int line_pos=p_size.width-1;
	p.draw_fill_rect(Point(line_pos,0),Size(1,p_size.height),color(COLOR_GLOBAL_VIEW_SEPARATOR));
}

GlobalBeatBarColumn::GlobalBeatBarColumn(Editor *p_editor)  {
	
	editor=p_editor;
	bar_map=&p_editor->get_song()->get_bar_map();
	set_bg_on_updates(false);
//	setToolTip("Bar/Beat");

}


GlobalBeatBarColumn::~GlobalBeatBarColumn()
{
}


}
