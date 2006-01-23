//
// C++ Implementation: pixmap_button
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "pixmap_button.h"

#include <Qt/qpainter.h>
#include <Qt/qevent.h>
#include "typedefs.h"
#include <Qt/qaction.h>
namespace ReShaked {


void PixmapButton::paintEvent(QPaintEvent *e) {
	
	QPainter p(this);
	
	
	if (status.press_attempt==false && status.hovering && !skin.hover.isNull()) {
		
		
		p.drawPixmap(0,0,skin.hover);
	} else {
		/* determine if pressed or not */
				
		bool pressing;
		if (status.press_attempt) {
			
			pressing=status.pressing_inside;
			if (status.pressed)
				pressing=!pressing;
		} else {
			
			pressing=status.pressed;
		}
		
		if (pressing) 
			p.drawPixmap(0,0,skin.pressed);
		else			
			p.drawPixmap(0,0,skin.normal);
	}
}
void PixmapButton::mousePressEvent(QMouseEvent *e) {
	
	if (e->button()!=Qt::LeftButton)
		return;
	
	status.press_attempt=true;
	status.pressing_inside=true;
	update();
	
}


void PixmapButton::emit_qactions() {

	
	QList<QAction*> action_list=actions();
	foreach(I,action_list) {
		
		
		(*I)->trigger();
	}
}
		
void PixmapButton::mouseReleaseEvent(QMouseEvent *e) {
	if (e->button()!=Qt::LeftButton)
		return;
	
	

	if (status.press_attempt &&status.pressing_inside) {

		status.press_attempt=false;
		
		if (type==TYPE_PUSH) {
			
			mouse_press_event_override();
			mouse_pressed_signal();
			emit_qactions();
		} else if (type==TYPE_TOGGLE) {
			
			status.pressed=!status.pressed;
			mouse_pressed_signal();
			mouse_toggled_signal(status.pressed);
			if (status.pressed)
				emit_qactions();
			
		}
		
		
	}
	
	status.press_attempt=false;
	update();
}

void PixmapButton::enterEvent ( QEvent * event ) {
	
	if (status.press_attempt)
		status.pressing_inside=true;
	status.hovering=true;
	update();
	
}
void PixmapButton::leaveEvent ( QEvent * event ) {
	
	if (status.press_attempt)
		status.pressing_inside=false;
	
	status.hovering=false;
	update();
}

void PixmapButton::mouse_press_event_override() {
	
	
}

bool PixmapButton::is_pressed() {
	
	return status.pressed;
}
void PixmapButton::set_pressed(bool p_pressed) {
	
	if (status.pressed==p_pressed)
		return; //pointless
	ERR_FAIL_COND(type == TYPE_PUSH);
	status.press_attempt=false;
	status.pressed=p_pressed;
	update();
	
}


PixmapButton::PixmapButton(QWidget *p_parent,const Skin& p_skin,Type p_type) : QWidget(p_parent) {
	
	skin=p_skin;
	type=p_type;
	setFixedSize(p_skin.normal.size());
	status.pressed=false;
	status.press_attempt=false;
	status.hovering=false;
	status.pressing_inside=false;
	
	
}


PixmapButton::~PixmapButton()
{
}


}
