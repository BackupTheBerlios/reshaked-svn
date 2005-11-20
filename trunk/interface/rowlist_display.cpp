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

namespace ReShaked {



void RowListDisplay::paintEvent(QPaintEvent *pe) {

	PixmapFont *pfont = VisualSettings::get_singleton()->get_pattern_font();
	PixmapFont *rfont = VisualSettings::get_singleton()->get_rowcount_font();
	QPainter p(this);
	p.fillRect(0,0,width(),height(),QColor(0,0,0));
	int rowsize=VisualSettings::get_singleton()->get_editing_row_height();
	int visible_rows=height()/rowsize;
	int fontofs=(rowsize-pfont->get_height())/2;

	for (int i=0;i<visible_rows;i++) {

		int beat=cursor->get_snapped_window_beat( i );
		int subbeat=cursor->get_snapped_window_subsnap( i );

		char str[5]={' ',' ',' ',' ',0};

		int displaynum=(subbeat==0)?beat:subbeat;
		PixmapFont *displayfont=(subbeat==0)?rfont:pfont;
		int x=0;
		if (subbeat>0)
			x-=rfont->get_width();

		if (displaynum/1000 || subbeat==0)
			str[0]='0'+DIGIT_4(displaynum);
		if (displaynum/100 || subbeat==0)
			str[1]='0'+DIGIT_3(displaynum);
		if (displaynum/10 || subbeat==0)
			str[2]='0'+DIGIT_2(displaynum);
		if (displaynum || subbeat==0)
			str[3]='0'+DIGIT_1(displaynum);

		displayfont->render_string( p, x,i*rowsize+fontofs , str );

		if ( subbeat==0 ) //beat
			p.fillRect(0,i*rowsize,width(),1,GET_QCOLOR(COLORLIST_PATTERN_EDIT_BEAT_LINE));
		else
			p.fillRect(0,i*rowsize,width(),1,GET_QCOLOR(COLORLIST_PATTERN_EDIT_SUBBEAT_LINE));

	}

}
RowListDisplay::RowListDisplay(QWidget *p_parent,Cursor *p_cursor) : QWidget(p_parent) {

	setFixedWidth(VisualSettings::get_singleton()->get_rowcount_font()->get_width()*CHAR_WIDTH);

	setBackgroundRole(QPalette::NoRole);

	cursor=p_cursor;
}


RowListDisplay::~RowListDisplay()
{
}


}
