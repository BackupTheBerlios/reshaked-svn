//
// C++ Interface: range_level_editor
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDRANGE_LEVEL_EDITOR_H
#define RESHAKEDRANGE_LEVEL_EDITOR_H

#include <Qt/qwidget.h>
#include "property.h"
#include <Qt/qpixmap.h>
namespace ReShaked {

/**
	@author red <red@killy>
*/
class RangeLevelEditor : public QWidget {
public:	
	struct Skin {
			
		QPixmap	bg;
		QPixmap	point_from;
		QPixmap	point_to;
		QColor line_color;
		int border_w;
		
		Skin(QPixmap p_bg=QPixmap(),QPixmap p_point_from=QPixmap(),QPixmap p_point_to=QPixmap(),QColor p_line_color=QColor(0,0,0),int p_border_w=0) { bg=p_bg; point_from=p_point_from; point_to=p_point_to; line_color=p_line_color; border_w=p_border_w; }
	};
private:	
	struct Click {
		QPoint point;
		double scale;
		double range;
		int drag_point; //drag point, 0 for none, 1 or 2
	} click;

	Skin skin;

	void paintEvent(QPaintEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void changed();
	
	Property *scale_1;
	Property *scale_2;
	Property *range_1;
	Property *range_2;
public:

	void set_properties(Property *p_scale_1,Property *p_scale_2,Property *p_range_1,Property *p_range_2);

	RangeLevelEditor(QWidget *p_parent,const Skin &p_skin);
	~RangeLevelEditor();

};

}

#endif
