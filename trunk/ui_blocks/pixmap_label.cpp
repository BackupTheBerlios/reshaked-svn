//
// C++ Implementation: pixmap_label
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "pixmap_label.h"
#include <Qt/qpainter.h>
namespace ReShaked {


void PixmapLabel::paintEvent(QPaintEvent *e) {
	
	QPainter p(this);
	if (skin_box)
		skin_box->paint_into(p,0,0,width(),height());
	if (pixmap)
		p.drawPixmap(0,0,*pixmap);
}

void PixmapLabel::set_pos(QPoint p_pos) {
	
	pos=p_pos;
}
void PixmapLabel::set_color(QColor p_col) {
	
	col=p_col;
}


void PixmapLabel::set_angle(int p_phi) {
	
	angle=p_phi;
}

void PixmapLabel::set_font(const QFont &p_font) {
	
	font=p_font;
}

void PixmapLabel::set_text(QString p_text) {
	
	text=p_text;
	update();
	
}

PixmapLabel::PixmapLabel(QWidget *p_parent,SkinBox *p_skin_box,bool p_own_it) :QWidget(p_parent) {
	
	skin_box=p_skin_box;
	pixmap=NULL;
	owned=p_own_it;
	if (p_skin_box->get_center_h()==0)
		setFixedHeight(p_skin_box->get_top()+p_skin_box->get_bottom() );
	if (p_skin_box->get_center_h()==0)
		setFixedWidth(p_skin_box->get_left()+p_skin_box->get_right() );
	
	pos=QPoint(0,0);
	col=QColor(255,255,255);
	angle=0;
	
}

PixmapLabel::PixmapLabel(QWidget *p_parent,QPixmap *p_pixmap,bool p_own_it) :QWidget(p_parent) {
	
	skin_box=NULL;
	pixmap=p_pixmap;
	owned=p_own_it;
	
	setFixedSize(pixmap->size()); 
	pos=QPoint(0,0);
	col=QColor(255,255,255);
	angle=0;
}

PixmapLabel::~PixmapLabel()
{
	
	if (owned && skin_box)
		delete skin_box;
	if (owned && pixmap)
		delete pixmap;
}


}
