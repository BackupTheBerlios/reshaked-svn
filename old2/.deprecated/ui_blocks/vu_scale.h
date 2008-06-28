//
// C++ Interface: vu_scale
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDVU_SCALE_H
#define RESHAKEDVU_SCALE_H

#include "ui_blocks/pixmap_label.h"

namespace ReShaked {

/**
	@author Juan Linietsky <reshaked@gmail.com>
*/
class VUScale : public PixmapLabel {
	
	int max;
	int min;
	
	enum {
		HEIGHT_SEPARATOR=6
	};
	
	void paintEvent(QPaintEvent *p_event);
	
	QColor zero_color;
public:
	
	void set_zero_color(QColor p_color);
	void set_max(int p_max);
	void set_min(int p_min);
	
	VUScale(QWidget *p_widget,SkinBox *p_skin_box,bool p_own_it=false);
	VUScale(QWidget *p_parent,QPixmap p_pixmap,PixmapExpandType p_type=EXPAND_NONE);
	~VUScale();
};

}

#endif
