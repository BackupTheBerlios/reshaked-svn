//
// C++ Implementation: pixmap_updown
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "pixmap_updown.h"
#include <Qt/qpainter.h>
#include <Qt/qevent.h>


namespace ReShaked {


void PixmapUpDown::paintEvent(QPaintEvent *e) {
	
	QPainter p(this);
	
	if (status.pressing_up && !skin.up.isNull())
		p.drawPixmap(0,0,skin.up);
	else if (status.pressing_down && !skin.down.isNull())
		p.drawPixmap(0,0,skin.down);
	else
		p.drawPixmap(0,0,skin.normal);
		
	
}
void PixmapUpDown::mousePressEvent(QMouseEvent *e) {
	
	bool maxmin_pressed;
	if (e->button()==Qt::LeftButton)
		maxmin_pressed=false;
	else if (e->button()==Qt::RightButton)
		maxmin_pressed=true;
	else
		return; //other buttons
		   
	if (e->y()<(height()/2)) {
			
		status.pressing_up=true;	
		if (maxmin_pressed)
			max_pressed_signal();
		else
			up_pressed_signal();
			
	} else {
		
		status.pressing_down=true;	
		if (maxmin_pressed)
			min_pressed_signal();
		else
			down_pressed_signal();
	}
	
	update();
	
}
void PixmapUpDown::mouseReleaseEvent(QMouseEvent *e) {
	
	status.pressing_up=false;
	status.pressing_down=false;
	update();
	
}

PixmapUpDown::PixmapUpDown(QWidget *p_parent,const Skin &p_skin) : QWidget(p_parent) {
	
	skin=p_skin;
	setFixedSize(p_skin.normal.size());
	status.pressing_up=false;
	status.pressing_down=false;
	
}


PixmapUpDown::~PixmapUpDown()
{
}


}
