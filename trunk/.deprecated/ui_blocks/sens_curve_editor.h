//
// C++ Interface: sens_curve_editor
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSENS_CURVE_EDITOR_H
#define RESHAKEDSENS_CURVE_EDITOR_H

#include "property.h"
#include <Qt/qwidget.h>
#include <Qt/qpixmap.h>

namespace ReShaked {

/**
	@author red <red@killy>
 */
class SensCurveEditor : public QWidget, public PropertyEditor {
public:	

	struct Skin {
	
		QPixmap	bg;
		QColor line_color;
		int border_w;

		Skin(QPixmap p_bg=QPixmap(),QColor p_line_color=QColor(0,0,0),int p_border_w=0) { bg=p_bg; line_color=p_line_color; border_w=p_border_w; }
	};
private:	
	struct Click {
		QPoint point;
		double val;
		bool drag;
	} click;

	Skin skin;

	void paintEvent(QPaintEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void changed();
public:

	SensCurveEditor(QWidget *p_parent,const Skin &p_skin);
	~SensCurveEditor();

};


}

#endif
