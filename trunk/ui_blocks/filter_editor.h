//
// C++ Interface: filter_editor
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDFILTER_EDITOR_H
#define RESHAKEDFILTER_EDITOR_H

#include <Qt/qwidget.h>
#include "property.h"
#include <Qt/qpixmap.h>

namespace ReShaked {

/**
	@author red <red@killy>
*/

class FilterEditor : public QWidget, public MultiPropertyEditor {
public:	
	
	enum {
		MIN_FREQ = 20,
		AMP_STEPS = 3
	};
	
	enum {
		PROPERTY_CUTOFF,
		PROPERTY_RESONANCE,
	};
	
	struct Skin {
			
		QPixmap	bg;
		QPixmap	point;
		QColor line_color;
		int border_w;
		
		Skin(QPixmap p_bg=QPixmap(),QPixmap p_point=QPixmap(),QColor p_line_color=QColor(0,0,0),int p_border_w=0) { bg=p_bg; point=p_point;  line_color=p_line_color; border_w=p_border_w; }
	};
private:	
	struct Click {
		bool drag;
	} click;

	Skin skin;

	void paintEvent(QPaintEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void changed();
	
	Property *mode;
	Property *cutoff;
	Property *resonance;
	
	void changed(int);

public:

	void set_properties(Property *p_mode,Property *p_cutoff,Property *p_resonance);

	FilterEditor(QWidget *p_parent,const Skin &p_skin);
	~FilterEditor();

};

}



#endif
