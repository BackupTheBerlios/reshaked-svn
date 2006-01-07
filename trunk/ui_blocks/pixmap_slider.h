//
// C++ Interface: pixmap_slider
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDPIXMAP_SLIDER_H
#define RESHAKEDPIXMAP_SLIDER_H


#include <Qt/qwidget.h>
#include <Qt/qpixmap.h>

namespace ReShaked {

/**
	@author red <red@killy>
*/
class PixmapSlider : public QWidget {

	
	Q_OBJECT
public:	
	enum Type {
		
		TYPE_VERTICAL,
		TYPE_HORIZONTAL,
		
	};
	
	struct Skin {
		
		QPixmap bottom;	
		QPixmap bottom_full;	
		QPixmap grabber;	
		
		Skin() {};
		Skin(QPixmap p_bottom, QPixmap p_bottom_full, QPixmap p_grabber);
	};
	
private:
			
	int margin_beg;
	int margin_end;
	float value;
	Skin skin;		
	Type type;
	
	struct Drag {
		
		bool active;
		float pos_at_click;
		float value_at_click;
	} drag;
	
	
	float get_click_pos(QPoint p_mouse_pos);
	void paintEvent(QPaintEvent *pe);
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	
	virtual void value_changed(float p_new_val);
signals:	
	
	void value_changed_signal(float p_new_val);
public:	
		
	void set_value(float p_value);

	
	PixmapSlider(QWidget *p_parent,const Skin& p_skin,Type p_type=TYPE_VERTICAL,int p_margin_beg=5,int p_margin_end=5);
	~PixmapSlider();

};

}

#endif
