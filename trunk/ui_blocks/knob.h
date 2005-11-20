//
// C++ Interface: knob
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDKNOB_H
#define RESHAKEDKNOB_H


#include <Qt/qwidget.h>
#include <Qt/qpixmap.h>

namespace ReShaked {

/**
	@author red <red@killy>
*/
class Knob : public QWidget {

	Q_OBJECT
			    
	float value;	
	QPixmap base;
	QPixmap handle;
	int angle_begin;
	int handle_at_radius;
	
	void paintEvent(QPaintEvent *pe);

	void mouseMoveEvent ( QMouseEvent * e );
	void mousePressEvent ( QMouseEvent * e );
	
	QPoint oldpos;
	
	
	
signals:	
	
	
	void value_changed_signal(float p_new_value);
public:
	
	void set_value(float p_value);
	float get_value();
	Knob(QWidget *p_parent);
	Knob(QWidget *p_parent,QPixmap p_base,QPixmap p_handle,int p_handle_at_radius,int p_angle_begin=30); //custom knob
	~Knob();

};

}

#endif
