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
#include "visual_settings.h"
#include <Qt/qpainter.h>

namespace ReShaked {


void GlobalBeatBarColumn::set_global_view(GlobalView *p_global_view) {
	
	
	global_view=p_global_view;
	QObject::connect(global_view,SIGNAL(drawing_signal()),this,SLOT(update()));
	
}

void GlobalBeatBarColumn::paintEvent(QPaintEvent *pe) {
	
	QPainter p(this);
	p.fillRect(0,0,width(),height(),QColor(0,0,0));
	int beat_from=global_view->get_beat_at_pixel(0);
	int beat_to=global_view->get_beat_at_pixel(height());
	
	PixmapFont *pf=VisualSettings::get_singleton()->get_global_bar_font();
	PixmapFont *bf=VisualSettings::get_singleton()->get_global_beat_font();
	
	for (int i=beat_from;i<=beat_to;i++) {
		
		
		if (global_view->get_beat_pixel_size()>bf->get_height()) { //paint beat

				
			QString s = QString::number( bar_map->get_bar_beat( i)+1 );
			int ofs=width()-(s.length()+1)*bf->get_width();
			bf->render_string( p, ofs, global_view->get_beat_pixel( i ), s.toAscii().data() );
		}
		
		if (!bar_map->get_bar_beat( i )) { // paint bar
			
			QString s = QString::number(bar_map->get_bar_at_beat( i)+1 );
			int ofs=width()-(s.length()+3)*pf->get_width();
			pf->render_string( p, ofs, global_view->get_beat_pixel( i ), s.toAscii().data() );
		}			
			
	}
	p.setPen(QColor(150,150,150));
	int line_pos=width()-bf->get_width()*5/2;
	p.drawLine(line_pos,0,line_pos,height());
}

GlobalBeatBarColumn::GlobalBeatBarColumn(QWidget *p_parent,BarMap *p_bar_map) : QWidget(p_parent) {
	
	bar_map=p_bar_map;
	setFixedWidth(VisualSettings::get_singleton()->get_global_bar_font()->get_width()*7);
	setBackgroundRole(QPalette::NoRole);
	//setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);

}


GlobalBeatBarColumn::~GlobalBeatBarColumn()
{
}


}
