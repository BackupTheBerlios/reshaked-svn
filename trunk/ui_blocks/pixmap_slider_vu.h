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

#include "ui_blocks/pixmap_slider.h"

namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class PixmapSliderVU : public PixmapSlider {
				       
	
				       
	void paintEvent(QPaintEvent *e);
	
	float vu_val;
	float vu_val_seek;
public:

	void update_vu(float p_to_val);
	
	PixmapSliderVU(QWidget *p_parent,const Skin& p_skin,Type p_type=TYPE_VERTICAL,int p_margin_beg=5,int p_margin_end=5);
	~PixmapSliderVU();

};

}

#endif
