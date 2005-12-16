//
// C++ Implementation: skin_box
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "skin_box.h"
#include <Qt/qpainter.h>

namespace ReShaked {


int SkinBox::get_top() {
	
	return top;
}
int SkinBox::get_left() {
	
	return left;
}
int SkinBox::get_right() {
	
	return right;
}
int SkinBox::get_bottom() {

	return bottom;
}


void SkinBox::paint_into(QPainter&p, int p_x, int p_y, int p_w, int p_h) {

	int center_w=p_w-left-right;
	int center_h=p_h-top-bottom;
	
	p.setClipping(true);
	p.setClipRect(p_x,p_y,p_w,p_h);
	/* Corners */
	p.drawPixmap(p_x,p_y,pix_top_left);	
	p.drawPixmap(p_x+left+center_w,p_y,pix_top_right);	
	p.drawPixmap(p_x,p_y+right+center_h,pix_bottom_left);	
	p.drawPixmap(p_x+left+center_w,p_y+right+center_h,pix_bottom_right);	
	
	/* Margins */
	
	if (stretch) {
		
		p.drawPixmap(p_x+left,p_y,center_w,top,pix_top);
		p.drawPixmap(p_x,p_y+top,left,center_h,pix_left);
		p.drawPixmap(p_x+left,p_y+top+center_h,center_w,bottom,pix_bottom);
		p.drawPixmap(p_x+left+center_w,p_y+top,right,center_h,pix_right);
	
	} else {
		p.drawTiledPixmap(p_x+left,p_y,center_w,top,pix_top);
		p.drawTiledPixmap(p_x,p_y+top,left,center_h,pix_left);
		p.drawTiledPixmap(p_x+left,p_y+top+center_h,center_w,bottom,pix_bottom);
		p.drawTiledPixmap(p_x+left+center_w,p_y+top,right,center_h,pix_right);
	}	
	/* center */
	if (stretch) 
		p.drawPixmap(p_x+left,p_y+right,center_w,center_h,pix_center);
	else		
		p.drawTiledPixmap(p_x+left,p_y+right,center_w,center_h,pix_center);

	p.setClipping(false);
	
	
}

void SkinBox::load_from_pixmap(const QPixmap& p_pixmap,int p_top, int p_left, int p_bottom, int p_right, bool p_stretch) {
	
	int center_width=p_pixmap.width()-p_left-p_right;
	int center_height=p_pixmap.height()-p_top-p_bottom;
	pix_top_left=p_pixmap.copy(0,0,p_left,p_top);
	pix_top=p_pixmap.copy(p_left,0,center_width,p_top);
	pix_top_right=p_pixmap.copy(p_left+center_width,0,p_left,p_top);
	pix_left=p_pixmap.copy(0,p_top,p_left,center_height);
	pix_center=p_pixmap.copy(p_left,p_top,center_width,center_height);
	pix_right=p_pixmap.copy(p_left+center_width,p_top,p_right,center_height);
	pix_bottom_left=p_pixmap.copy(0,p_top+center_height,p_left,p_bottom);
	pix_bottom=p_pixmap.copy(p_left,p_top+center_height,center_width,p_bottom);
	pix_bottom_right=p_pixmap.copy(p_left+center_width,p_top+center_height,p_right,p_bottom);
	
	top=p_top;
	left=p_left;
	right=p_right;
	bottom=p_bottom;
	stretch=p_stretch;
	
}

void SkinBox::load_from_xpm(const char** p_xpm,int p_top, int p_left, int p_bottom, int p_right, bool p_stretch) {

	QPixmap tmp(p_xpm);
	load_from_pixmap(tmp,p_top,p_left,p_bottom,p_right,p_stretch);
	
	
}

SkinBox::SkinBox(const char ** p_xpm,int p_top, int p_left, int p_bottom, int p_right, bool p_stretch) {
	
	load_from_xpm(p_xpm,p_top,p_left,p_bottom,p_right,p_stretch);
}
SkinBox::SkinBox()
{
	top=left=right=bottom=1;
	
}


SkinBox::~SkinBox()
{
}


}
