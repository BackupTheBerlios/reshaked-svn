//
// C++ Implementation: vu_scale
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "vu_scale.h"
#include <Qt/qpainter.h>
#include <Qt/qfontmetrics.h>
namespace ReShaked {


void VUScale::paintEvent(QPaintEvent *p_event) {
	
	PixmapLabel::paintEvent(p_event);

	QPainter p(this);
	
	int f_height=20; //start with this
	
	while (f_height>0) {
		
		QFont f;
		f.setPixelSize(f_height);
		QFontMetrics fm(f);
		
		int w=fm.width( QString::number(min)+"X" ); // X for spacing
		if (w>=width()) {
			
			f_height--;
			continue;
		}
		w=fm.width( QString::number(max)+"X" ); // X for spacing
		if (w>=width()) {
			
			f_height--;
			continue;
		}

		break;
	}
	
	int step_height=f_height+HEIGHT_SEPARATOR;
	
	QFont f;
	p.setFont(f);
	f.setPixelSize(f_height);
	QFontMetrics fm(f);
	
	int zero_y=(-min)*height()/(max-min);
	
	
	int to_bottom_steps=(zero_y)/step_height;
	
	
	for (int i=0;i<to_bottom_steps;i++) {
		
		
		int pos=(zero_y-step_height*i);
		int v=min+pos*(max-min)/height();
		pos=height()-pos;
		
		if (i==0) {
			v=0;
			p.setPen(zero_color);
			QFont fb=f;
			fb.setBold(true);
			p.setFont(fb);
		} else {
			
			p.setFont(f);
			p.setPen(get_color());
		}
			
		p.drawLine(width()*3/4,pos,width(),pos);
		p.drawText( (width()-fm.width( QString::number(v) ))/2,pos-2, QString::number(v) );
		
	}
			
	p.drawLine(width()*3/4,height()-1,width(),height()-1);
	p.drawText( (width()-fm.width( QString::number(min) ))/2,height()-3, QString::number(min) );
	
	int to_top_steps=(height()-zero_y)/step_height;
	
	
	for (int i=1;i<to_top_steps;i++) {
		
		
		int pos=(zero_y+step_height*i);
		int v=min+pos*(max-min)/height();
		pos=height()-pos;
		
		p.drawLine(width()*3/4,pos,width(),pos);
		p.drawText( (width()-fm.width( QString::number(v) ))/2,pos-2, QString::number(v) );
		
	}
			
	//p.drawLine(width()*3/4,height()-1,width(),height()-1);
	//p.drawText( (width()-fm.width( QString::number(min) ))/2,height()-3, QString::number(min) );
	
}

void VUScale::set_zero_color(QColor p_color) {
	
	zero_color=p_color;
}
void VUScale::set_max(int p_max) {
	
	max=p_max;
	update();	
}
void VUScale::set_min(int p_min) {
	
	min=p_min;
	update();
}


VUScale::VUScale(QWidget *p_widget,SkinBox *p_skin_box,bool p_own_it) : PixmapLabel(p_widget,p_skin_box,p_own_it) {
	
	max=100;
	min=0;
}

VUScale::VUScale(QWidget *p_parent,QPixmap p_pixmap,PixmapExpandType p_type) : PixmapLabel(p_parent,p_pixmap,p_type) {
	
	max=100;
	min=0;
	
}


VUScale::~VUScale()
{
}


}
