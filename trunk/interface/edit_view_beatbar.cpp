//
// C++ Implementation: edit_view_beatbar
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "edit_view_beatbar.h"
#include "base/painter.h"
#include "gui_custom/rsskin.h"

namespace ReShaked {

void EditViewBeatBar::paint_marker(int p_marker_idx,int p_row,bool p_paint_arrow,int p_check_next) {

	
	Painter *p=get_painter();
	String marker_text=song->get_marker_list().get_index_value(p_marker_idx);
	int beat=song->get_marker_list().get_index_pos(p_marker_idx);
	int rowsize=get_row_height()	;
	int pos=rowsize*p_row;
	int beat_h=rowsize/4;


	FontID fontid = font( FONT_EDIT_VIEW_MARKER_FONT );

	if (p_check_next>=0 && (p_check_next*rowsize)<p->get_font_string_width(fontid,marker_text))
		return;


	// Draw Arrow 
	/*if (p_paint_arrow) {
		
		
		p.setPen(QColor(150,150,150));
		p.setBrush(QColor(150,150,150));
		QPolygon arrow;
		arrow.push_back(QPoint(0,pos-beat_h/2));
		arrow.push_back(QPoint(0,pos+beat_h/2));
		arrow.push_back(QPoint(m.descent(),pos));
	
		p.drawPolygon(arrow,Qt::WindingFill);
	} */
	// Draw Text 


	p->draw_text( fontid, Point(p->get_font_descent(fontid)+1,pos+beat_h),  marker_text, DOWN, color(COLOR_EDIT_VIEW_MARKER_FONT));

}


int EditViewBeatBar::get_row_height() {
	
	int height=get_painter()->get_font_height( font(FONT_PATTERN) )+constant(C_PATTERN_VOL_NOTE_BAR_HEIGHT)+constant(C_EDITOR_ROW_MARGIN)*2;
	
	return height;
	
}

void EditViewBeatBar::draw(const Point& p_global,const Size& p_size,const Rect& p_exposed) {

	if (song->get_track_count()==0)
		cursor->set_window_size( get_size_cache().height / get_row_height() );

	Painter *p = get_painter();
	
	FontID pfont = font(FONT_PATTERN);
	int pfont_width=p->get_font_char_width(pfont,'0');
	int pfont_ascent=p->get_font_ascent(pfont);
	int pfont_height=p->get_font_height(pfont);
	
	p->draw_fill_rect(p_exposed.pos,p_exposed.size,color(COLOR_EDITOR_BG));
	
	int rowsize=get_row_height();
	int visible_rows=get_size_cache().height/rowsize;
	int fontofs=pfont_ascent+(rowsize-p->get_font_height(pfont))/2;
	int first_marker_at=-1;

	for (int i=0;i<visible_rows;i++) {

	
	
		int beat=cursor->get_snapped_window_beat( i );
		int subbeat=cursor->get_snapped_window_subsnap( i );

		/* SUB BEAT */
		
		String str;
		int xofs;
	
	
		/* BAR */
	
		if (subbeat>0) { /*subbeat*/
		
			str=String::num(subbeat+1);
	
			xofs=get_size_cache().width-(str.length()+1)*pfont_width;
			p->draw_text( pfont, Point(xofs,i*rowsize+fontofs) , str,color(COLOR_EDIT_VIEW_ROW_SUBBEAT_FONT) );
		} else if (song->get_bar_map().get_bar_beat( beat)==0) { //bar
			str=String::num(song->get_bar_map().get_bar_at_beat( beat)+1 );
			xofs=2*pfont_width;
			p->draw_text( pfont, Point(xofs,i*rowsize+fontofs) , str,color(COLOR_EDIT_VIEW_ROW_BAR_FONT) );
			int ofs_bar=xofs+p->get_font_string_width(pfont,str)+pfont_width;
			p->draw_fill_rect( Point( ofs_bar, i*rowsize+fontofs-pfont_ascent), Size(  get_size_cache().width-(ofs_bar+pfont_width),pfont_height),color(COLOR_EDIT_VIEW_ROW_SUBBEAT_FONT));

		} else  {
			
			/* BEAT */
		
			str=String::num(song->get_bar_map().get_bar_beat( beat) +1);
			str+="-";	
			xofs=get_size_cache().width-(str.length()+1)*pfont_width;
			p->draw_text( pfont, Point(xofs,i*rowsize+fontofs) , str,color(COLOR_EDIT_VIEW_ROW_BEAT_FONT) );
			
		} 
	
		if (subbeat==0) { //just paint subbeat
	
		
			p->draw_fill_rect(Point(0,i*rowsize),Size(get_size_cache().width,1),color(COLOR_EDIT_VIEW_ROW_BEAT_FONT));
			
			if (song->get_bar_map().get_bar_beat( beat)==0)  {
				p->draw_fill_rect(Point(0,i*rowsize),Size(get_size_cache().width,1),color(COLOR_EDIT_VIEW_ROW_BAR_FONT));
			}
		
		} else {
			p->draw_fill_rect(Point(0,i*rowsize),Size(get_size_cache().width,1),color(COLOR_EDIT_VIEW_ROW_SUBBEAT_FONT));
		}
	
	
		if (subbeat==0) {
		
			int marker_idx=song->get_marker_list().get_exact_index(beat); //marker here?
			if (marker_idx!=INVALID_STREAM_INDEX) {
			
				if (first_marker_at==-1)
					first_marker_at=i;
			
				paint_marker(marker_idx,i);
			
			}
		
		}
	
	}

	/* Paint current marker */		
	int beat=cursor->get_snapped_window_beat( 0 );
	int marker_idx=song->get_marker_list().get_prev_index(beat);
	if (marker_idx>=0) {
		
		paint_marker(marker_idx,0,false,first_marker_at);
	}


}

Size EditViewBeatBar::get_minimum_size_internal() {
	
	FontID pfont = font(FONT_PATTERN);
	int pfont_width=get_painter()->get_font_char_width(pfont,'0');
	return Size(pfont_width*CHAR_WIDTH,0); /* marker height? where is it? */
	
}

EditViewBeatBar::EditViewBeatBar(Editor *p_editor)  {


	cursor=&p_editor->get_cursor();
	song=p_editor->get_song();
}


EditViewBeatBar::~EditViewBeatBar()
{
}


}
