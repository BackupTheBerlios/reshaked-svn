//
// C++ Implementation: pixmap_slider
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "pixmap_vu.h"
#include <Qt/qpainter.h>

namespace ReShaked {


PixmapVU::Skin::Skin(QPixmap p_bottom, QPixmap p_bottom_full) {
	
	bottom=p_bottom;
	bottom_full=p_bottom_full;

	
}

/***********/

void PixmapVU::paintEvent(QPaintEvent *pe) {

	
	QPainter p(this);
	
	int area=(type==TYPE_VERTICAL)?height():width();
	
	area-=margin_beg+margin_end;

	
	int value_ofs=(int)(value*area);
	
	int fill_size=value_ofs+margin_beg;
	
	
	
	if (type==TYPE_VERTICAL) {
		
		p.drawPixmap(0,height()-fill_size, skin.bottom_full , 0, height()-fill_size, width(), fill_size);
		p.drawPixmap(0,0, skin.bottom , 0, 0, width(), height()-fill_size);
		
	} else {
		
		
		
	}
			

}

PixmapVU::PixmapVU(QWidget *p_parent,const Skin& p_skin,Type p_type,int p_margin_beg,int p_margin_end) :QWidget(p_parent) {
	
	skin=p_skin;
	value=0;
	setFixedSize(p_skin.bottom.size());
	type=p_type;
	margin_beg=p_margin_beg;
	margin_end=p_margin_end;
	value=0.5;
}


PixmapVU::~PixmapVU()
{
}


}
