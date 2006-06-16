//
// C++ Interface: pixmap_slider_vu
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDPIXMAP_SLIDER_VU_H
#define RESHAKEDPIXMAP_SLIDER_VU_H

#include <Qt/qwidget.h>
#include <Qt/qpixmap.h>
#include <Qt/qtimer.h>

namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class PixmapSliderVU : public QWidget {
				       
	Q_OBJECT
public:
	
	struct Skin {
		
		
	};
private:	
	enum {
		
		UPDATE_INTERVAL=50,
		BLOCK_HEIGHT=5,
	};
		
	QPixmap slider_grabber;
	
	void paintEvent(QPaintEvent *e);
	
	float value;
	float fall_time;
	float visual_value;
	QTimer *updater;
	
	float slider_value;
	float get_click_pos(QPoint p_mouse_pos);
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void wheelEvent ( QWheelEvent * e ) ;
	
	
	struct Drag {
		
		bool active;
		float pos_at_click;
		float value_at_click;
	} drag;

protected:	
	virtual void slider_value_changed(float p_new_val);
	
private slots:	
	
	void updater_slot();	
signals:
		
	void slider_value_changed_signal(float p_to_val);
public:

	void set_fall_time(float p_speed);
	void set_value(float p_value);
	void set_slider_value(float p_val);
	float get_slider_value();
	
	PixmapSliderVU(QWidget *p_parent,QPixmap p_slider_grabber);
	~PixmapSliderVU();

};

}

#endif
