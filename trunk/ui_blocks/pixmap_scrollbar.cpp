//
// C++ Implementation: pixmap_scrollbar
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "pixmap_scrollbar.h"
#include <Qt/qpainter.h>
#include <Qt/qevent.h>
#include "typedefs.h"
namespace ReShaked {

PixmapScrollBar::Skin::Skin(SkinBox* p_bottom, SkinBox* p_grabber) {
	
	bottom=p_bottom;
	grabber=p_grabber;
}

/***********/

int PixmapScrollBar::get_grabber_size() {
	
	int area=(type==TYPE_VERTICAL)?height():width();

	
	if (max==0) 
		return area;
	
	int grabber_size=pagesize*area/max;
	
	
	int minsize=(type==TYPE_VERTICAL)?(skin.grabber->get_top()+skin.grabber->get_center_h()+skin.grabber->get_bottom()):(skin.grabber->get_left()+skin.grabber->get_center_w()+skin.grabber->get_right());
	
	if (grabber_size<minsize)
		grabber_size=minsize;
	
	return grabber_size;
	
}

int PixmapScrollBar::get_grabber_offset() {
	
	int area=(type==TYPE_VERTICAL)?height():width();
	int grabber=get_grabber_size();
	
	area-=grabber;
	if (area<=0)
		return 0;
	
	float range=(float)value/(float)max;
	return (int)(range*area);
}

void PixmapScrollBar::paint_grabber(QPainter&p) {
	
	int size=get_grabber_size();
	int ofs=get_grabber_offset();
	
	
	if (type==TYPE_VERTICAL) {
		
		skin.grabber->paint_into(p,0,ofs,width(),size);
		
	} else {
		skin.grabber->paint_into(p,ofs,0,size,height());
		
	}
	
}

void PixmapScrollBar::paintEvent(QPaintEvent *pe) {

	
	QPainter p(this);
	
	skin.bottom->paint_into(p,0,0,width(),height());
	
	paint_grabber( p);
			

}


int PixmapScrollBar::get_click_pos(QPoint p_mouse_pos) {
	
	
	int pos=(type==TYPE_VERTICAL)?(p_mouse_pos.y()):p_mouse_pos.x();
	int range=(type==TYPE_VERTICAL)?height():width();
	int grabber=get_grabber_size();
	
	range-=grabber;
	if (range==0)
		return 0;
	return int(((float)pos/(float)range)*(float)max); 
	
}

void PixmapScrollBar::mousePressEvent(QMouseEvent *e) {
	
	if (e->button()!=Qt::LeftButton)
		return;
	
	if (pagesize==max)
		return;
	drag.active=true;
	drag.pos_at_click=get_click_pos(e->pos());
	drag.value_at_click=value;
		
}

void PixmapScrollBar::value_changed(int p_new_val) {
	
	
}


void PixmapScrollBar::mouseMoveEvent(QMouseEvent *e) {
	
	if (!drag.active)
		return;
			
	int value_ofs=drag.value_at_click+(get_click_pos(e->pos())-drag.pos_at_click);
	if (value_ofs<0)
		value_ofs=0;
	if (value_ofs>max)
		value_ofs=max;
	if (value==value_ofs)
		return; //dont bother if the value is the same
	value=value_ofs;
	value_changed(value_ofs);
	value_changed_signal(value_ofs);
	update();

	
}
void PixmapScrollBar::mouseReleaseEvent(QMouseEvent *e) {
	
	if (e->button()!=Qt::LeftButton)
		return;
	
	drag.active=false;
}

void PixmapScrollBar::wheelEvent ( QWheelEvent * e ) {
	
	if (e->delta()>0)
		set_value( get_value() - step );
	else
		set_value( get_value() + step );
	
	value_changed_signal(value);
}
void PixmapScrollBar::set_value(int p_value) {
	
	
	if (p_value<0)
		p_value=0;
	if (p_value>max)
		p_value=max;
	
	value=p_value;
	if (!drag.active)
		update();
}


void PixmapScrollBar::set_max(int p_max) {
	if (p_max<0)
		p_max=0;
	if (value>p_max)
		value=p_max;
	if (pagesize>p_max)
		pagesize=p_max;
	max=p_max;
	update();
	
}
void PixmapScrollBar::set_pagesize(int p_pagesize) {
	
	if (p_pagesize<0)
		p_pagesize=0;	
	if (p_pagesize>max)
		p_pagesize=max;
	
	pagesize=p_pagesize;
	update();
}

int PixmapScrollBar::get_value() {
	
	return value;	
}
int PixmapScrollBar::get_max() {
	
	return max;
}
int PixmapScrollBar::get_pagesize() {
	
	return pagesize;
}


void PixmapScrollBar::set_stepsize(int p_step) {
	
	ERR_FAIL_INDEX(p_step,max);
	step=p_step;
	
}
PixmapScrollBar::PixmapScrollBar(QWidget *p_parent,const Skin& p_skin,Type p_type) : QWidget(p_parent) {
	


	skin=p_skin;
	value=0;
	max=1;
	pagesize=1;
	step=1;
	type=p_type;
	drag.active=false;
	if (p_type==TYPE_HORIZONTAL)
		setFixedHeight(p_skin.bottom->get_top()+p_skin.bottom->get_bottom()+p_skin.bottom->get_center_h());
	else
		setFixedWidth(p_skin.bottom->get_left()+p_skin.bottom->get_right()+p_skin.bottom->get_center_w());
		
}


PixmapScrollBar::~PixmapScrollBar()
{
}


}
