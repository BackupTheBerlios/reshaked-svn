//
// C++ Interface: pixmap_updown
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDPIXMAP_UPDOWN_H
#define RESHAKEDPIXMAP_UPDOWN_H

#include <Qt/qwidget.h>
#include <Qt/qpixmap.h>

namespace ReShaked {

/**
	@author red <red@killy>
*/
class PixmapUpDown : public QWidget {
			    
Q_OBJECT			    
			    
public:
	
	struct Skin {
		
		QPixmap normal;
		QPixmap up;
		QPixmap down;
		
		Skin(QPixmap p_normal=QPixmap(),QPixmap p_up=QPixmap(),QPixmap p_down=QPixmap()) { normal=p_normal; up=p_up; down=p_down; }
	};
	
	
private:
		
	Skin skin;	
	
	struct Status {
		
		bool pressing_up;
		bool pressing_down;
			
	} status;
	
	void paintEvent(QPaintEvent *e);
	void mousePressEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void wheelEvent ( QWheelEvent * e ) ;
signals:		
	
	void max_pressed_signal();
	void min_pressed_signal();
	void up_pressed_signal();
	void down_pressed_signal();
public:
	
	PixmapUpDown(QWidget *p_parent,const Skin &p_skin);
	~PixmapUpDown();

};

}

#endif
