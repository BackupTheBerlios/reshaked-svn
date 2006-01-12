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
#ifndef RESHAKEDPIXMAP_VU_H
#define RESHAKEDPIXMAP_VU_H


#include <Qt/qwidget.h>
#include <Qt/qpixmap.h>

namespace ReShaked {

/**
	@author red <red@killy>
*/
class PixmapVU : public QWidget {
public:
	
	enum Type {
		
		TYPE_VERTICAL,
		TYPE_HORIZONTAL,
		
	};
	
	struct Skin {
		
		QPixmap bottom;	
		QPixmap bottom_full;	
		
		Skin() {};
		Skin(QPixmap p_bottom, QPixmap p_bottom_full);
	};
	
private:
			
	int margin_beg;
	int margin_end;
	float value;
	Skin skin;		
	void paintEvent(QPaintEvent *pe);
	Type type;
public:	
		
	void set_value(float p_value);	
	
	PixmapVU(QWidget *p_parent,const Skin& p_skin,Type p_type=TYPE_VERTICAL,int p_margin_beg=5,int p_margin_end=5);
	~PixmapVU();

};

}

#endif
