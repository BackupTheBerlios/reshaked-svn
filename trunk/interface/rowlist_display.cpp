//
// C++ Implementation: rowlist_display
//
// Description:
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "rowlist_display.h"
#include <Qt/qpainter.h>
#include "ui_blocks/visual_settings.h"
#include "ui_blocks/helpers.h"

namespace ReShaked {


void RowListDisplay::paint_marker(QPainter& p, int p_marker_idx,int p_row,bool p_paint_arrow,int p_check_next) {
	
	String marker_text=song->get_marker_list().get_index_value(p_marker_idx);
	int beat=song->get_marker_list().get_index_pos(p_marker_idx);
	int fontwidth=VisualSettings::get_singleton()->get_pattern_font()->get_width()*2/3;
	int rowsize=VisualSettings::get_singleton()->get_editing_row_height();	
	int pos=rowsize*p_row;
	int beat_h=rowsize;
	
	
	QFont f;
	f.setPixelSize(fontwidth*2);
	QFontMetrics m(f);
	
	if (p_check_next>=0 && (p_check_next*rowsize)<m.width(QStrify(marker_text)))
		return;
	
	
	p.save();
	
	/* Draw Arrow */
	if (p_paint_arrow) {
		p.setPen(QColor(150,150,150));
		p.setBrush(QColor(150,150,150));
		QPolygon arrow;
		arrow.push_back(QPoint(0,pos-beat_h/2));
		arrow.push_back(QPoint(0,pos+beat_h/2));
		arrow.push_back(QPoint(m.descent(),pos));
		
		p.drawPolygon(arrow,Qt::WindingFill);
	}	
	/* Draw Text */
	
	
	
	p.setFont(f);
	p.rotate(90.0); //90 degrees!
	p.setPen(QColor(255,255,255));
	p.drawText(pos+beat_h,-m.descent(), QStrify( marker_text ) );
	
	p.restore();
	
}


void RowListDisplay::paintEvent(QPaintEvent *pe) {

	cursor->set_window_size( height() / VisualSettings::get_singleton()->get_editing_row_height() );
	
	PixmapFont *pfont = VisualSettings::get_singleton()->get_pattern_font();
	PixmapFont *rfont = VisualSettings::get_singleton()->get_rowcount_font();
	PixmapFont *bfont = VisualSettings::get_singleton()->get_rowcount_subbeat_font();
	
	QPainter p(this);
	p.fillRect(0,0,width(),height(),QColor(0,0,0));
	int rowsize=VisualSettings::get_singleton()->get_editing_row_height();
	int visible_rows=height()/rowsize;
	int fontofs=(rowsize-pfont->get_height())/2;
	int first_marker_at=-1;

	for (int i=0;i<visible_rows;i++) {

		
		
		int beat=cursor->get_snapped_window_beat( i );
		int subbeat=cursor->get_snapped_window_subsnap( i );

		/* SUB BEAT */
			
		QString str;
		int xofs;
		
		
		/* BAR */
		
		if (subbeat>0) { /*subbeat*/
			
			str=QString::number(subbeat+1);
		
			xofs=width()-(str.length()+1)*pfont->get_width();
			bfont->render_string( p, xofs,i*rowsize+fontofs , str.toAscii().data() );
		} else if (song->get_bar_map().get_bar_beat( beat)==0) { //bar
			str=QString::number(song->get_bar_map().get_bar_at_beat( beat)+1 );
			str="*"+str+"*";
			xofs=2*pfont->get_width();
			rfont->render_string( p, xofs,i*rowsize+fontofs , str.toAscii().data() );
		} else  {
				
			/* BEAT */
			
			str=QString::number(song->get_bar_map().get_bar_beat( beat) +1);
			str+="-";	
			xofs=width()-(str.length()+1)*pfont->get_width();
			pfont->render_string( p, xofs,i*rowsize+fontofs , str.toAscii().data() );
		} 
		
		if (subbeat==0) { //just paint subbeat
		
			
			p.fillRect(0,i*rowsize,width(),1,GET_QCOLOR(COLORLIST_PATTERN_EDIT_BEAT_LINE));
			if (song->get_bar_map().get_bar_beat( beat)==0)  {
				p.fillRect(0,i*rowsize,width(),1,GET_QCOLOR(COLORLIST_PATTERN_EDIT_BAR));
			}
			
		} else {
			p.fillRect(0,i*rowsize,width(),1,GET_QCOLOR(COLORLIST_PATTERN_EDIT_SUBBEAT_LINE));
		}
		
		
		if (subbeat==0) {
			
			int marker_idx=song->get_marker_list().get_exact_index(beat); //marker here?
			if (marker_idx!=INVALID_STREAM_INDEX) {
				
				if (first_marker_at==-1)
					first_marker_at=i;
				
				paint_marker(p,marker_idx,i);
				
			}
			
		}
		
	}
	
	/* Paint current marker */		
	int beat=cursor->get_snapped_window_beat( 0 );
	int marker_idx=song->get_marker_list().get_prev_index(beat);
	if (marker_idx>=0) {
			
		paint_marker(p, marker_idx,0,false,first_marker_at);
	}
	

}
RowListDisplay::RowListDisplay(QWidget *p_parent,Editor *p_editor) : QWidget(p_parent) {

	setFixedWidth(VisualSettings::get_singleton()->get_rowcount_font()->get_width()*CHAR_WIDTH);

	setBackgroundRole(QPalette::NoRole);

	cursor=&p_editor->get_cursor();
	song=p_editor->get_song();
}


RowListDisplay::~RowListDisplay()
{
}


}
