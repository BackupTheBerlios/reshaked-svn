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
	
	int old_bar=-1;
	int next_i_allowed=-1;
	
	for (int i=0;i<height();i++) {
		
		int current_bar=global_view->get_bar_at_pixel(i);
		bool bar=false;
		if (global_view->get_pixel_v_offset()==0 && i==0) 
			bar=true;
		else if (old_bar!=current_bar && i>next_i_allowed)
			bar=true;
		
		old_bar=current_bar;
		if (!bar) 
			continue;
			
		PixmapFont *pf=VisualSettings::get_singleton()->get_global_bar_font();
		QString s = QString::number(current_bar);
		int ofs=width()-(s.length()+1)*pf->get_width();
		pf->render_string( p, ofs, i, s.toAscii().data() );
		next_i_allowed=i+pf->get_height();
	}
}

GlobalBeatBarColumn::GlobalBeatBarColumn(QWidget *p_parent) : QWidget(p_parent) {
	
	
	setFixedWidth(VisualSettings::get_singleton()->get_global_bar_font()->get_width()*5);
	setBackgroundRole(QPalette::NoRole);
	//setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);
	
	
	
}


GlobalBeatBarColumn::~GlobalBeatBarColumn()
{
}


}
