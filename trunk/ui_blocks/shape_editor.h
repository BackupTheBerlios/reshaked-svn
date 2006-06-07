//
// C++ Interface: shape_editor
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSHAPE_EDITOR_H
#define RESHAKEDSHAPE_EDITOR_H


#include <Qt/qwidget.h>
#include <Qt/qpixmap.h>
#include "engine/shape.h"

namespace ReShaked {

/**
	@author Juan Linietsky <reshaked@gmail.com>
*/
class ShapeEditor : public QWidget {
	
	Q_OBJECT
	
	enum {
		
		GRAB_NEAR_TO=5
	};
	
public:	
	
	struct Skin {
		
		QPixmap bg;
		QColor bg_col;
		QColor line_col;
		QColor point_col;
		QColor zero_col;
		float zero_pos;
		
		Skin() { zero_pos=0; }
	};
	
	
	
private:		
	
	
	
	struct Moving {
		
		int move_idx;
		
	} moving;
	
	Skin skin;
	Shape *shape;
	
	void paintEvent(QPaintEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);

			
	int get_closest_point( QPoint p_screen );
	QPoint get_screen_pos(float p_pos,float p_val);
	void get_shape_pos(QPoint p_pos,float *r_val, float *r_pos);
	

signals:	
	

	void shape_changed_signal();
public:
    
	void set_shape(Shape *p_shape);
	ShapeEditor(QWidget *p_parent,const Skin& p_skin);
	~ShapeEditor();

};

}

#endif
