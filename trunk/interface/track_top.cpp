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
#include <Qt/qpainter.h>

namespace ReShaked {

void TrackTop::paintEvent(QPaintEvent *e) {
	
	QPainter p(this);
	p.fillRect(0,0,width(),height(),QColor(0,0,0));
	QPixmap px = VisualSettings::get_singleton()->get_pixmap( PIXMAP_TRACK_OPTIONS );
	p.drawPixmap(0,0,px);
	
}

TrackTop::TrackTop(QWidget *p_parent,Editor *p_editor,int p_track_idx) :QWidget(p_parent) {
	editor=p_editor;
	track_idx=p_track_idx;
	int wheight=VisualSettings::get_singleton()->get_pixmap( PIXMAP_TRACK_OPTIONS ).height();
	setFixedHeight(wheight);
	setBackgroundRole(QPalette::NoRole);
	
}


TrackTop::~TrackTop()
{
}


}
