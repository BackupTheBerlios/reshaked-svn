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

	bool grabbing;
	
	QPoint oldpos;
	
	void set_value_internal(float p_value);
	
protected:	
	
	void mouseMoveEvent ( QMouseEvent * e );
	void mousePressEvent ( QMouseEvent * e );
	void mouseReleaseEvent ( QMouseEvent * e );
	
	virtual void value_changed(float p_new_value);
signals:	
	
	
	void value_changed_signal(float p_new_value);
public:
	
	struct Skin {
		
		QPixmap base;
		QPixmap handle;
		int handle_at_distance;
		int angle_begin;
	};
	
	void set_value(float p_value);
	float get_value();
	Knob(QWidget *p_parent);
	Knob(QWidget *p_parent,const Skin& p_skin); //custom knob
	~Knob();

};

}

#endif
