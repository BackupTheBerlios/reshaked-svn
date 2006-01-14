//
// C++ Implementation: pixmap_slider_vu
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "pixmap_slider_vu.h"
#include <Qt/qpainter.h>
namespace ReShaked {



void PixmapSliderVU::update_vu(float p_to_val) {
	

	if (p_to_val<0)
		p_to_val=0;
	if (p_to_val>1)
		p_to_val=1;
	
	vu_val=vu_val_seek=p_to_val;
	update();
}

void PixmapSliderVU::paintEvent(QPaintEvent *e) {
	
	
	QPainter p(this);
	
	int area=(type==TYPE_VERTICAL)?height():width();
	area-=margin_end-margin_beg;
	area=int((float)area*vu_val_seek)+margin_beg;
	
	
	if (type==TYPE_VERTICAL) {
		
		p.drawPixmap(0,height()-area, skin.bottom_full , 0, height()-area, width(), area);
		p.drawPixmap(0,0, skin.bottom , 0, 0, width(), height()-area);
		
	} else {
		
		
	}	
	paint_grabber(p);
	
}

PixmapSliderVU::PixmapSliderVU(QWidget *p_parent,const Skin& p_skin,Type p_type,int p_margin_beg,int p_margin_end) : PixmapSlider(p_parent,p_skin,p_type,p_margin_beg,p_margin_end)
{
	vu_val=vu_val_seek=0.5;
	
}


PixmapSliderVU::~PixmapSliderVU()
{
}


}
