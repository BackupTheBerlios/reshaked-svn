//
// C++ Implementation: track_top
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "track_top.h"
#include "ui_blocks/visual_settings.h"
#include "ui_blocks/helpers.h"
#include <Qt/qpainter.h>
#include <Qt/qfont.h>
#include <Qt/qfontmetrics.h>

namespace ReShaked {

void TrackTop::paintEvent(QPaintEvent *e) {
	
	QPainter p(this);
	p.fillRect(0,0,width(),height(),QColor(0,0,0));
	QPixmap px = VisualSettings::get_singleton()->get_pixmap( PIXMAP_TRACK_OPTIONS );
	p.drawPixmap(0,0,px);
	
	if (!track)
		return;
	QFont f;
	f.setPixelSize(height()-4);
	f.setBold(true);
	p.setFont(f);
	QFontMetrics m(p.font());

	int ofs=px.width()+2;
	QString name=QStrify(track->get_name());
	
	if ( (ofs+m.boundingRect(name).width())>width()) { //doesnt fit!
		
		int eat=1;
		do {
			
			QString str=name.left( name.length() - eat )+"..";
			
			if ( (ofs+m.boundingRect(str).width()) < width() ) {
				
				name=str;
				break;
				
			}
			
			eat++;
			
		} while (eat<name.length());
	}
		
	p.setPen(QColor(200,255,200));
	p.drawText(px.width()+2,m.ascent(),name);
	
	
}

TrackTop::TrackTop(QWidget *p_parent,Track *p_track) :QWidget(p_parent) {
	track=p_track;
	int wheight=VisualSettings::get_singleton()->get_pixmap( PIXMAP_TRACK_OPTIONS ).height();
	setFixedHeight(wheight);
	setBackgroundRole(QPalette::NoRole);
	
}


TrackTop::~TrackTop()
{
}


}
