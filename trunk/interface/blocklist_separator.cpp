//
// C++ Implementation: blocklist_separator
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "blocklist_separator.h"
#include <Qt/qpainter.h>
#include <Qt/qfontmetrics.h>

namespace ReShaked {



void BlockList_Separator::paintEvent(QPaintEvent *pe) {
	
	QPainter p(this);
	p.fillRect(0,0,width(),height(),QColor(0,0,0));
	

	QFont f;
	f.setPixelSize(width()-3); //will be vertical so..
	f.setBold(true);
	p.setFont(f);
	QFontMetrics m(p.font());
	
	p.rotate(90);
	p.setPen(QColor(255,255,255,200));
	p.drawText(OFFSET,-(width()-m.ascent()),string);
	
}

BlockList_Separator::BlockList_Separator(QWidget *p_parent,QString p_string) : QWidget(p_parent) {
	
	string=p_string;
	setFixedWidth(WIDTH);
	setMouseTracking(true);

}


BlockList_Separator::~BlockList_Separator()
{
}


}
