//
// C++ Implementation: knob
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "knob.h"
#include "visual_settings.h"
#include <math.h>
#include <Qt/qevent.h>

namespace ReShaked {

void Knob::value_changed(float p_new_value) {
	
	
}

void Knob::mousePressEvent ( QMouseEvent * e ) {
	
	printf("press\n");
	if (e->button()==Qt::LeftButton) {
		oldpos=e->pos();
		grabbing=true;		
	}
		
	
}

void Knob::mouseMoveEvent ( QMouseEvent * e ) {
	
	
	if (!(e->buttons()&Qt::LeftButton)) {
		oldpos=e->pos();
		return; //not dragging, pointless
	}
	QPoint moved=e->pos()-oldpos;
	
	float turnamount=moved.x();
	turnamount/=100; //?
	if (e->modifiers()&Qt::ShiftModifier)
		turnamount/=8.0;
	set_value_internal( value+turnamount );				
	
	value_changed_signal(value);
	value_changed(value);
	
	oldpos=e->pos();
}

void Knob::mouseReleaseEvent ( QMouseEvent * e ) {
	
	if (e->button()==Qt::LeftButton) {
		grabbing=false;		
	}
	
}

void Knob::set_value(float p_value) {
	
	if (grabbing)
		return;
				
	set_value_internal(p_value);

}
void Knob::set_value_internal(float p_value) {
	
	if (p_value<0)
		p_value=0;
	if (p_value>1)
		p_value=1;
	if (value==p_value)
		return;
	
	value=p_value;
	update();
	
}
float Knob::get_value() {
	
	return value;
}

void Knob::paintEvent(QPaintEvent *pe) {
	
	/* paint base */
	QPainter p(this);
	p.drawPixmap(0,0,base);
	
	/* paint handle */
	
	float ang_begin_rad=(((float)angle_begin)*(M_PI*2.0)/360.0);
	float angle_range=M_PI*2.0-ang_begin_rad*2.0;
	float angle=ang_begin_rad+angle_range*value;
	
	int handle_y= height()/2-(int)(-cos(angle)*(float)handle_at_radius)-handle.height()/2;
	int handle_x= width()/2+(int)(-sin(angle)*(float)handle_at_radius)-handle.width()/2;

	
	p.drawPixmap(handle_x,handle_y,handle);
	
	
	
}

Knob::Knob(QWidget *p_parent): QWidget(p_parent) {
	
	base=VisualSettings::get_singleton()->get_pixmap( PIXMAP_KNOB_BASE );
	handle=VisualSettings::get_singleton()->get_pixmap( PIXMAP_KNOB_HANDLE );
	
	setFixedSize(base.width(),base.height());
	value=0;
	angle_begin=30;
	handle_at_radius=16;
}

Knob::Knob(QWidget *p_parent,const Skin& p_skin) : QWidget(p_parent)
{
	angle_begin=p_skin.angle_begin;
	handle_at_radius=p_skin.handle_at_distance;
	base=p_skin.base;
	handle=p_skin.handle;
	
	setFixedSize(base.size());
	value=0;
	
	grabbing=false;
}


Knob::~Knob()
{
}


}
