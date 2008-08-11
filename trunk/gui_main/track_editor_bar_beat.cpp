//
// C++ Implementation: track_editor_bar_beat
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "track_editor_bar_beat.h"

#include "base/painter.h"
#include "gui_common/common_skin.h"



int TrackEditorBarBeat::get_font_width() {

	return get_painter()->get_font_char_width(  font(FONT_TRACK_EDITOR),'X' );
}

GUI::Size TrackEditorBarBeat::get_minimum_size_internal() {

	return GUI::Size( get_font_width() * MAX_CHAR_WIDTH, 0 );
}

int TrackEditorBarBeat::get_row_height() {
	
	int height=get_painter()->get_font_height( font(FONT_TRACK_EDITOR) )+constant(C_TRACK_EDITOR_VOL_NOTE_BAR_HEIGHT)+constant(C_TRACK_EDITOR_ROW_MARGIN)*2+constant(C_TRACK_EDITOR_VOL_NOTE_BAR_SEPAATION);

	return height;
	
}

void TrackEditorBarBeat::draw(const GUI::Point& p_pos,const GUI::Size& p_size,const GUI::Rect& p_exposed) {


	Editor::get_singleton()->set_window_rows( p_size.height / get_row_height() );
		
	GUI::Painter &p=*get_painter();

//	p.set_clip_rect(false,p_exposed);
	//p.set_clip_rect(true,p_exposed);
	
	p.draw_fill_rect(GUI::Point(),p_size,color(COLOR_BARBEAT_BG));

	int visible_rows=Editor::get_singleton()->get_window_rows()+1;
	int row_size=get_row_height();

	int font_w=get_font_width();
	
	
	for (int i=0;i<visible_rows;i++) {
		
		Tick tick=Editor::get_singleton()->get_row_ticks( Editor::get_singleton()->get_window_offset() + i );

		int from_x; 
		int width_x; 
			
		from_x=0;
		width_x=get_size_cache().width;
		
		GUI::Color line_color;
		GUI::Color font_color;
		String text;
		int char_right=0;
				
		if ( (tick % TICKS_PER_BEAT)==0 ) {//beat
			
		
			if (Editor::get_singleton()->get_song()->get_bar_map().get_bar_beat(tick/TICKS_PER_BEAT)==0)  {
				line_color=color(COLOR_BARBEAT_BAR_LINE);
				font_color=color(COLOR_BARBEAT_BAR_FONT);
				text="="+String::num(1+Editor::get_singleton()->get_song()->get_bar_map().get_bar_at_beat(tick/TICKS_PER_BEAT))+"=";
				char_right=3;
			} else {
				line_color=color(COLOR_BARBEAT_BEAT_LINE);
				font_color=color(COLOR_BARBEAT_BEAT_FONT);
				text=String::num(1+Editor::get_singleton()->get_song()->get_bar_map().get_bar_beat(tick/TICKS_PER_BEAT))+"-";
				char_right=1;
						
			}

		} else {
			line_color=color(COLOR_BARBEAT_SUBBEAT_LINE);
			font_color=color(COLOR_BARBEAT_SUBBEAT_FONT);
			text=String::num( 2+ ( tick % TICKS_PER_BEAT ) * Editor::get_singleton()->get_cursor_zoom_divisor() / TICKS_PER_BEAT);
			char_right=1;
		}

		p.draw_fill_rect(GUI::Point(from_x,i*row_size),GUI::Size(width_x,1),line_color);
		GUI::Point text_ofs;
		text_ofs.y=i*row_size+get_painter()->get_font_ascent( font(FONT_TRACK_EDITOR) )+constant(C_TRACK_EDITOR_ROW_MARGIN);
		text_ofs.x=MAX_CHAR_WIDTH-text.length()-char_right;
		text_ofs.x*=get_font_width();
		
		p.draw_text( font( FONT_TRACK_EDITOR ), text_ofs, text, font_color );
		//p.drawGUI::Rect(0,i*row_size,size.width,0);


	}

}



TrackEditorBarBeat::TrackEditorBarBeat() {
}


TrackEditorBarBeat::~TrackEditorBarBeat()
{
}


