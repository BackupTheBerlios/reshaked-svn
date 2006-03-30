//
// C++ Implementation: pixmap_list
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "pixmap_list.h"
#include <Qt/qpainter.h>
#include <Qt/qevent.h>
#include <Qt/qfontmetrics.h>

namespace ReShaked {


	
void PixmapList::paintEvent(QPaintEvent *e) {
	
	
	QPainter p(this);
	
	int row_height=skin.font_height+skin.separator;
	
	int visible_rows=height()/row_height;
	
	QFont font;
	font.setPixelSize(skin.font_height);
	QFontMetrics fm(font);
	
	if (skin.skin_bg)
		skin.skin_bg->paint_into(p,0,0,width(),height());
	else
		p.fillRect(0,0,width(),height(),QColor(0,0,0));
	
	for (int i=0;i<visible_rows;i++) {
		
		int row=i+view_offset;
		
		if (row>=strings.size())
			break;
		
		
		if (row==selected)
			p.fillRect(0,i*row_height,width(),row_height,skin.selected_bg_color);
		
		p.setPen(skin.separator_color);
		p.drawLine(0,row_height*(i+1),width(),row_height*(i+1));
		p.setPen( (row==selected)?skin.selected_font_color:skin.font_color );
		p.drawText(skin.row_margin,row_height*(i+1)-fm.descent(),strings[row]);
	
	}
	
}

void PixmapList::mousePressEvent(QMouseEvent *e) {
	
	int row_height=skin.font_height+skin.separator;
	
	int row=e->y()/row_height;
	row+=view_offset;
	
	if (row<0 || row>=strings.size())
		return;
	
	selected=row;
	
	item_selected_signal(selected);
	
	update();
}
	

void PixmapList::scrollbar_value_changed(int p_to_val) {
	
	
	
}


void PixmapList::add_item(QString p_item,int p_at_pos) {
	
	if (p_at_pos==-1)
		strings.push_back(p_item);
	
	update();
}
void PixmapList::clear() {
	
	strings.clear();
	view_offset=0;
	selected=-1;	
	update();
	
}

int PixmapList::get_selected() {
	
	return selected;
	
}

int PixmapList::get_item_count() {
	
	return strings.size();
}

void PixmapList::select_item(int p_index) {
	
	ERR_FAIL_INDEX(p_index,strings.size());
	
	selected=p_index;
	update();
	
}


void PixmapList::set_scrollbar(PixmapScrollBar *p_scroll_bar) {
	
	scroll_bar=p_scroll_bar;
}

PixmapList::PixmapList(QWidget *p_parent,const Skin& p_skin) : QWidget(p_parent) {
	
	view_offset=0;
	selected=-1;
	skin=p_skin;
	scroll_bar=NULL;
	
}


PixmapList::~PixmapList()
{
}


}
