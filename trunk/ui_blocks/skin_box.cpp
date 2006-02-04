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

int SkinBox::get_center_w() {
	
	return center_w;
}
int SkinBox::get_center_h() {
	
	return center_h;
}

void SkinBox::paint_into(QPainter&p, int p_x, int p_y, int p_w, int p_h) {

	int center_w=p_w-left-right;
	int center_h=p_h-top-bottom;
	
	
	p.save();
	
	bool disable_clipping=true;
	if (p.hasClipping()) {
		
		p.setClipRect(p_x,p_y,p_w,p_h,Qt::IntersectClip);
		disable_clipping=false;
	} else {
		p.setClipping(true);
		p.setClipRect(p_x,p_y,p_w,p_h);
	}
	/* Corner TOP LEFT */
	p.drawPixmap(p_x,p_y,pix_top_left);	
	
	/* center */
	if (!no_center && !use_center_color) {
		if (stretch) 
			p.drawPixmap(p_x+left,p_y+top,center_w,center_h,pix_center);
		else		
			p.drawTiledPixmap(p_x+left,p_y+top,center_w,center_h,pix_center);
	}
	
	if (use_center_color) {
		
		p.fillRect(p_x+left,p_y+top,center_w,center_h,center_color);
	}
	
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
	

	/* corners not topleft */
	p.drawPixmap(p_x+left+center_w,p_y,pix_top_right);	
	p.drawPixmap(p_x,p_y+top+center_h,pix_bottom_left);	
	p.drawPixmap(p_x+left+center_w,p_y+top+center_h,pix_bottom_right);	
	
	if (disable_clipping)
		p.setClipping(false);
	
	p.restore();
	
	
}

void SkinBox::set_no_center_hint(bool p_no_center) {
	
	no_center=p_no_center;
}

void SkinBox::load_from_pixmap(const QPixmap& p_pixmap,int p_top, int p_left, int p_bottom, int p_right, bool p_stretch) {
	
	int center_width=p_pixmap.width()-p_left-p_right;
	int center_height=p_pixmap.height()-p_top-p_bottom;
	
	pix_top_left=p_pixmap.copy(0,0,p_left,p_top);
	if (center_width>0)
		pix_top=p_pixmap.copy(p_left,0,center_width,p_top);
	pix_top_right=p_pixmap.copy(p_left+center_width,0,p_right,p_top);
	if (center_height>0)
		
		pix_left=p_pixmap.copy(0,p_top,p_left,center_height);
	if (center_width>0 && center_height>0) {
		
		
		pix_center=p_pixmap.copy(p_left,p_top,center_width,center_height);
		
	}else if (center_width<=0 && center_height>0) {
			
		pix_center=p_pixmap.copy(p_left,p_top,1,center_height);
	} else if (center_width>0 && center_height<=0) {
			
		pix_center=p_pixmap.copy(p_left,p_top,1,center_height);
	}
		
	if (center_height>0)
		pix_right=p_pixmap.copy(p_left+center_width,p_top,p_right,center_height);
	
	pix_bottom_left=p_pixmap.copy(0,p_top+center_height,p_left,p_bottom);
	if (center_width>0)
		pix_bottom=p_pixmap.copy(p_left,p_top+center_height,center_width,p_bottom);
	pix_bottom_right=p_pixmap.copy(p_left+center_width,p_top+center_height,p_right,p_bottom);
	
	top=p_top;
	left=p_left;
	right=p_right;
	bottom=p_bottom;
	stretch=p_stretch;
	
	center_w=center_width;
	center_h=center_height;
	if (center_w<0)
		center_w=0;
	if (center_h<0)
		center_h=0;
	
}

void SkinBox::load_from_xpm(const char** p_xpm,int p_top, int p_left, int p_bottom, int p_right, bool p_stretch) {

	QPixmap tmp(p_xpm);
	load_from_pixmap(tmp,p_top,p_left,p_bottom,p_right,p_stretch);
	
	
}

void SkinBox::set_center_color(QColor p_color) {
	
	center_color=p_color;
}
void SkinBox::set_center_use_color(bool p_color) {
	
	use_center_color=p_color;
}


SkinBox::SkinBox(const char ** p_xpm,int p_top, int p_left, int p_bottom, int p_right, bool p_stretch) {
	
	load_from_xpm(p_xpm,p_top,p_left,p_bottom,p_right,p_stretch);
	no_center=false;
	use_center_color=false;
}
SkinBox::SkinBox()
{
	top=left=right=bottom=1;
	no_center=false;
	use_center_color=false;
	
}

SkinBox::~SkinBox()
{
}


}
