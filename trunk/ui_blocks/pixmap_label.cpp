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
#include <Qt/qevent.h>
namespace ReShaked {


void PixmapLabel::click_override() {
	
	
}

void PixmapLabel::mousePressEvent(QMouseEvent *e) {
	
	if (e->button()!=Qt::LeftButton)
		return;
	
	click_override();
	clicked_signal();
}

void PixmapLabel::set_bg(QPixmap p_bg) {
	
	pixmap=p_bg;
	update();
}

void PixmapLabel::paintEvent(QPaintEvent *e) {
	
	QPainter p(this);
	if (skin_box)
		skin_box->paint_into(p,0,0,width(),height());
	else {
		
		switch (expand_type) {
			case EXPAND_NONE: p.drawPixmap(0,0,pixmap); break;
			case EXPAND_TILE_V:
			case EXPAND_TILE_H: p.drawTiledPixmap(0,0,width(),height(),pixmap); break;
			case EXPAND_STRETCH_H:
			case EXPAND_STRETCH_V: p.drawPixmap(0,0,width(),height(),pixmap); break;
		};
	}
	
	QFontMetrics m(font);
	p.setFont(font);
	
	p.setPen(col);
	int text_ofs=centered?(m.width(text)/2):0;
	p.translate(pos);
	if (angle!=0)
		p.rotate(angle);
	
	p.drawText(-text_ofs,m.ascent(),text);

}

void PixmapLabel::set_pos(QPoint p_pos,bool p_centered) {
	
	centered=p_centered;
	pos=p_pos;
	update();
}
void PixmapLabel::set_color(QColor p_col) {
	
	col=p_col;
	update();
}


void PixmapLabel::set_angle(int p_phi) {
	
	angle=p_phi;
	update();
}

void PixmapLabel::set_font(const QFont &p_font) {
	
	
	font=p_font;
	update();
}

void PixmapLabel::set_text(QString p_text) {
	
	text=p_text;
	update();
	
}

PixmapLabel::PixmapLabel(QWidget *p_parent,SkinBox *p_skin_box,bool p_own_it) :QWidget(p_parent) {
	
	skin_box=p_skin_box;
	owned=p_own_it;
	if (p_skin_box->get_center_h()==0) {
		setFixedHeight(p_skin_box->get_top()+p_skin_box->get_bottom() );

	} if (p_skin_box->get_center_w()==0) {
		setFixedWidth(p_skin_box->get_left()+p_skin_box->get_right() );
	}
	
	pos=QPoint(0,0);
	col=QColor(255,255,255);
	angle=0;
	
	expand_type=EXPAND_NONE;
}

QFont& PixmapLabel::get_font() {
	return font;	
}
PixmapLabel::PixmapLabel(QWidget *p_parent,QPixmap p_pixmap,PixmapExpandType p_type) :QWidget(p_parent) {
	
	expand_type=p_type;
	skin_box=NULL;
	pixmap=p_pixmap;
	owned=false;
	if (p_type==EXPAND_NONE)
		setFixedSize(pixmap.size()); 
	else if (p_type==EXPAND_TILE_H || p_type==EXPAND_STRETCH_H)
		setFixedHeight(pixmap.height()); 
	else if (p_type==EXPAND_TILE_V || p_type==EXPAND_STRETCH_V)
		setFixedWidth(pixmap.width()); 
	
	
	
	font.setBold(true);
	int font_pix_size=p_pixmap.height()*3/5;
	if (font_pix_size<=0)
		font_pix_size=1; //so qt wont complain 
	font.setPixelSize(font_pix_size);
	set_pos(QPoint(p_pixmap.width()/2,(p_pixmap.height()-font_pix_size)/2),true);
	set_color(QColor(255,255,255));
	angle=0;
	
	if (p_type==EXPAND_TILE_H) {
		
		set_pos(QPoint(5,pos.y()));
		
	}
		
		

}

PixmapLabel::~PixmapLabel()
{
	
	if (owned && skin_box)
		delete skin_box;
}


}
