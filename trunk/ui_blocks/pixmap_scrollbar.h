//
// C++ Interface: pixmap_scrollbar
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDPIXMAP_SCROLLBAR_H
#define RESHAKEDPIXMAP_SCROLLBAR_H

#include <Qt/qwidget.h>
#include "ui_blocks/skin_box.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
class PixmapScrollBar : public QWidget {
			    
	Q_OBJECT
public:	
	enum Type {

		TYPE_VERTICAL,
		TYPE_HORIZONTAL,

	};

	struct Skin {

		SkinBox * bottom;
		SkinBox *grabber;	

		
		Skin(SkinBox *p_bottom=NULL, SkinBox *p_grabber=NULL);
	};

protected:			
	int value;
	int max;
	int pagesize;
	
	Skin skin;		
	Type type;

	struct Drag {

		bool active;
		int pos_at_click;
		int value_at_click;
	} drag;

	int get_click_pos(QPoint p_mouse_pos);
	void paintEvent(QPaintEvent *pe);
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);

	virtual void value_changed(int p_new_val);

	int get_grabber_offset();
	int get_grabber_size();
	void paint_grabber(QPainter&p);
signals:	

	void value_changed_signal(int p_new_val);
public:	

	void set_value(int p_value);
	void set_max(int p_max);
	void set_pagesize(int p_pagesize);
	
	int get_value();
	int get_max();
	int get_pagesize();

	PixmapScrollBar(QWidget *p_parent,const Skin& p_skin,Type p_type=TYPE_VERTICAL);
	~PixmapScrollBar();

};

}

#endif
