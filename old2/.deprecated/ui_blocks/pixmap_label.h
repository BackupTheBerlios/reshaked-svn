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
	Q_OBJECT	
public:	
	enum PixmapExpandType {
		EXPAND_NONE,
		EXPAND_TILE_H,
		EXPAND_TILE_V,
		EXPAND_STRETCH_H,
		EXPAND_STRETCH_V,
		
	};
private:

	PixmapExpandType expand_type;
	SkinBox *skin_box;
	QPixmap pixmap;
	bool owned;
	QString text;
	QFont font;
	QPoint pos;
	QColor col;
	int angle;
	
	bool centered;
	virtual void click_override();
protected:	
	void paintEvent(QPaintEvent *e);
	void mousePressEvent(QMouseEvent *e);
signals:
			
	void clicked_signal();
public:
	void set_bg(QPixmap p_bg);
	
	void set_pos(QPoint p_pos,bool p_centered=false);
	void set_color(QColor p_col);
	void set_angle(int p_phi);
	void set_font(const QFont &p_font);
	void set_text(QString p_text);
	QFont& get_font();
	QColor get_color();
	PixmapLabel(QWidget *p_parent,SkinBox *p_skin_box,bool p_own_it=false);
	PixmapLabel(QWidget *p_parent,QPixmap p_pixmap,PixmapExpandType p_type=EXPAND_NONE);

	~PixmapLabel();

};

}

#endif
