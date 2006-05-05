//
// C++ Interface: panner_editor
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDPANNER_EDITOR_H
#define RESHAKEDPANNER_EDITOR_H

#include "property.h"
#include <Qt/qwidget.h>
#include <Qt/qpixmap.h>

namespace ReShaked {

/**
	@author red <red@killy>
 */
class PannerEditor : public QWidget, public MultiPropertyEditor {


	Q_OBJECT
public:	
	struct Skin {
	
		QPixmap	bg;
		QPixmap	pos;
		int border_w;
		
		Skin(QPixmap p_bg=QPixmap(),QPixmap p_pos=QPixmap(),int p_border_w=0) { bg=p_bg ; pos=p_pos; border_w=p_border_w; }
	};
private:	
	
	enum {
		PROP_PAN,
		PROP_DEPTH
	};
	
	bool quad_mode;
	
	struct Click {
		bool drag;
	} click;

	Skin skin;

	void paintEvent(QPaintEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);

	void changed(int p_which);
	
	Property *pos;
	Property *depth;
public:

	void set_properties(Property *p_pos,Property *p_depth);
	PannerEditor(QWidget *p_parent,bool p_quad,const Skin &p_skin);
	~PannerEditor();

};


}

#endif
