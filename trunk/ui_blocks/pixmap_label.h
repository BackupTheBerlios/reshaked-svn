//
// C++ Interface: pixmap_label
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDPIXMAP_LABEL_H
#define RESHAKEDPIXMAP_LABEL_H

#include <Qt/qwidget.h>
#include "ui_blocks/skin_box.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
class PixmapLabel : public QWidget {
	
	SkinBox *skin_box;
	QPixmap pixmap;
	bool owned;
	void paintEvent(QPaintEvent *e);
	QString text;
	QFont font;
	QPoint pos;
	QColor col;
	int angle;
	
	bool centered;
public:
	void set_pos(QPoint p_pos,bool p_centered=false);
	void set_color(QColor p_col);
	void set_angle(int p_phi);
	void set_font(const QFont &p_font);
	void set_text(QString p_text);
	PixmapLabel(QWidget *p_parent,SkinBox *p_skin_box,bool p_own_it=false);
	PixmapLabel(QWidget *p_parent,QPixmap p_pixmap);

	~PixmapLabel();

};

}

#endif
