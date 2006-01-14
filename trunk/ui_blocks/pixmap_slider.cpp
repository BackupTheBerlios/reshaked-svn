//
// C++ Implementation: pixmap_slider
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "pixmap_slider.h"
#include <Qt/qpainter.h>
#include <Qt/qevent.h>
namespace ReShaked {


PixmapSlider::Skin::Skin(QPixmap p_bottom, QPixmap p_bottom_full, QPixmap p_grabber) {
	
	bottom=p_bottom;
	bottom_full=p_bottom_full;
	grabber=p_grabber;

}

/***********/

void PixmapSlider::paintEvent(QPaintEvent *pe) {

	
	QPainter p(this);
	
	int area=(type==TYPE_VERTICAL)?height():width();
	int grabber=(type==TYPE_VERTICAL)?skin.grabber.height():skin.grabber.width();
	
	area-=margin_beg+margin_end+grabber;

	
	int slider_ofs=(int)(value*area);
	
	int fill_size=slider_ofs+margin_beg+grabber/2;
	
	
	
	if (type==TYPE_VERTICAL) {
		
		p.drawPixmap(0,height()-fill_size, skin.bottom_full , 0, height()-fill_size, width(), fill_size);
		p.drawPixmap(0,0, skin.bottom , 0, 0, width(), height()-fill_size);
		p.drawPixmap((width()-skin.grabber.width())/2,(height()-fill_size-grabber/2),skin.grabber);
		
	} else {
		
		
	}
			

}


float PixmapSlider::get_click_pos(QPoint p_mouse_pos) {
	
	
	int pos=(type==TYPE_VERTICAL)?(height()-p_mouse_pos.y()):p_mouse_pos.x();
	int range=(type==TYPE_VERTICAL)?height():width();
	int grabber=(type==TYPE_VERTICAL)?skin.grabber.height():skin.grabber.width();
	
	range-=margin_beg+margin_end+grabber;
	
	return (float)pos/(float)range; //may look strange, but it's very useful
	
}

void PixmapSlider::mousePressEvent(QMouseEvent *e) {
	
	if (e->button()!=Qt::LeftButton)
		return;
	
	drag.active=true;
	drag.pos_at_click=get_click_pos(e->pos());
	drag.value_at_click=value;
		
}

void PixmapSlider::value_changed(float p_new_val) {
	
	
}


void PixmapSlider::mouseMoveEvent(QMouseEvent *e) {
	
	if (!drag.active)
		return;
			
	float value_ofs=drag.value_at_click+(get_click_pos(e->pos())-drag.pos_at_click);
	if (value_ofs<0)
		value_ofs=0;
	if (value_ofs>1)
		value_ofs=1;
	if (value==value_ofs)
		return; //dont bother if the value is the same
	value=value_ofs;
	value_changed(value_ofs);
	value_changed_signal(value_ofs);
	update();

	
}
void PixmapSlider::mouseReleaseEvent(QMouseEvent *e) {
	
	if (e->button()!=Qt::LeftButton)
		return;
	
	drag.active=false;
}


void PixmapSlider::set_value(float p_value) {
	
	
	if (p_value<0)
		p_value=0;
	if (p_value>1)
		p_value=1;
	
	value=p_value;
	if (!drag.active)
		update();
}

PixmapSlider::PixmapSlider(QWidget *p_parent,const Skin& p_skin,Type p_type,int p_margin_beg,int p_margin_end) :QWidget(p_parent) {
	
	skin=p_skin;
	value=0;
	setFixedSize(p_skin.bottom.size());
	type=p_type;
	margin_beg=p_margin_beg;
	margin_end=p_margin_end;
	value=0.5;
	drag.active=false;
}


PixmapSlider::~PixmapSlider()
{
}


}
