//
// C++ Implementation: pixmap_slider_vu
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "pixmap_slider_vu.h"
#include <Qt/qpainter.h>
#include <Qt/qevent.h>
namespace ReShaked {




void PixmapSliderVU::set_value(float p_value) {
	
	if (p_value<0)
		p_value=0;
	if (p_value>1)
		p_value=1;
	
	value=p_value;
	if (value>visual_value)
		visual_value=value;
	update();
}

void PixmapSliderVU::set_fall_time(float p_speed) {
	
	fall_time=p_speed;
	
	if (fall_time==0)
		updater->stop();
	else
		updater->start(UPDATE_INTERVAL);
	
}

void PixmapSliderVU::updater_slot() {
	if (fall_time==0)
		return; // guess it can happen?
	float old_val=visual_value;
	visual_value-=((float)UPDATE_INTERVAL/1000.0)/fall_time;
	if (visual_value<value)
		visual_value=value;
	if (visual_value<0)
		visual_value=0;
	
	if (old_val!=visual_value)
		update();
}
void PixmapSliderVU::paintEvent(QPaintEvent *e) {
	
	
	
	int steps=height()/BLOCK_HEIGHT;
	QPainter p(this);
	p.setPen(QColor(0,0,0));
	
	for (int i=0;i<steps;i++) {
		
		int from_y=height()-(i+1)*height()/steps;
		int to_y=height()-i*height()/steps;
		
		float v=(float)i/steps;
		
		bool lit=v<visual_value;
		
		v=v*v; //make the curve smoother to red
		
		QColor c( (int)(v*255.0) /  (lit?1:3), (255-(int)(v*255.0)) / (lit?1:3), 40 );
		
		p.fillRect(0,from_y,width(),to_y-from_y,c);
		
		
		p.drawLine(0,to_y,width(),to_y);

	}
	
	p.drawRect(0,0,width()-1,height()-1);
	
	int grabber=slider_grabber.height();
	
	float area=height();

	area-=grabber;
	int grabber_offset=(int)(slider_value*area);

	int fill_size=grabber_offset+grabber/2;
	p.drawPixmap((width()-slider_grabber.width())/2,(height()-fill_size-grabber/2),slider_grabber);
	
	/*	
	QPainter p(this);
	
	int area=(type==TYPE_VERTICAL)?height():width();
	area-=margin_end-margin_beg;
	area=int((float)area*vu_val_seek)+margin_beg;
	
	
	if (type==TYPE_VERTICAL) {
		
		p.drawPixmap(0,height()-area, skin.bottom_full , 0, height()-area, width(), area);
		p.drawPixmap(0,0, skin.bottom , 0, 0, width(), height()-area);
		
	} else {
		
		
	}	
	paint_grabber(p); */
	
}

float PixmapSliderVU::get_click_pos(QPoint p_mouse_pos) {
	
	
	int pos=height()-p_mouse_pos.y();
	int range=height();
	int grabber=slider_grabber.height();
	
	range-=grabber;
	
	return (float)pos/(float)range; //may look strange, but it's very useful
	
}

void PixmapSliderVU::mousePressEvent(QMouseEvent *e) {
	
	if (e->button()!=Qt::LeftButton)
		return;
	
	drag.active=true;
	drag.pos_at_click=get_click_pos(e->pos());
	drag.value_at_click=slider_value;
		
}

void PixmapSliderVU::slider_value_changed(float p_new_val) {
	
	
}


void PixmapSliderVU::mouseMoveEvent(QMouseEvent *e) {
	
	if (!drag.active)
		return;
			
	float value_ofs=drag.value_at_click+(get_click_pos(e->pos())-drag.pos_at_click);
	if (value_ofs<0)
		value_ofs=0;
	if (value_ofs>1)
		value_ofs=1;
	if (value==value_ofs)
		return; //dont bother if the value is the same
	slider_value=value_ofs;
	slider_value_changed(value_ofs);
	slider_value_changed_signal(value_ofs);
	update();

	
}

void PixmapSliderVU::wheelEvent ( QWheelEvent * e ) {
	
	int range=height();
	int grabber=slider_grabber.height();
	range-=grabber;
	
	if (range==0)
		range=0.0001;
	float step=1.0/(float)range;
	
	if (!e->modifiers()&Qt::ShiftModifier)
		step*=5.0;
	
	if (e->delta()>0)
		set_slider_value( slider_value + step );
	else if (e->delta()<0)
		set_slider_value( slider_value - step );
	
	slider_value_changed( slider_value );
	slider_value_changed_signal(slider_value);
}

void PixmapSliderVU::mouseReleaseEvent(QMouseEvent *e) {
	
	if (e->button()!=Qt::LeftButton)
		return;
	
	drag.active=false;
}


void PixmapSliderVU::set_slider_value(float p_value) {
	
	
	if (p_value<0)
		p_value=0;
	if (p_value>1)
		p_value=1;
	
	slider_value=p_value;
	if (!drag.active)
		update();
}


PixmapSliderVU::PixmapSliderVU(QWidget *p_parent,QPixmap p_slider_grabber) : QWidget(p_parent) {

	visual_value=0;
	slider_grabber=p_slider_grabber;
	value=0;
	setFixedWidth(p_slider_grabber.width());
	slider_value=0;
	drag.active=false;
	
	fall_time=0;
	updater = new QTimer(this);
	QObject::connect(updater,SIGNAL(timeout()),this,SLOT(updater_slot()));	
}


PixmapSliderVU::~PixmapSliderVU()
{
}


}
