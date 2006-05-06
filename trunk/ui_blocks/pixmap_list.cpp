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
	
	int visible_rows=(height()-skin.margin*2)/row_height;
	
	QFont font;
	font.setPixelSize(skin.font_height);
	font.setBold(skin.bold);
	QFontMetrics fm(font);
	p.setFont(font);
	
	int fontofs=skin.separator/2+skin.margin;
	
	if (skin.skin_bg)
		skin.skin_bg->paint_into(p,0,0,width(),height());
	else
		p.fillRect(0,0,width(),height(),QColor(0,0,0));
	
	p.setClipRect( QRect(skin.margin,skin.margin,width()-skin.margin*2,height()-skin.margin*2) );
	
	for (int i=0;i<visible_rows;i++) {
		
		int row=i+view_offset;
		
		if (row>=strings.size())
			break;
		
		
		if (row==selected)
			p.fillRect(0,skin.margin+i*row_height,width(),row_height,skin.selected_bg_color);
		
		p.setPen(skin.separator_color);
		p.drawLine(0,skin.margin+row_height*(i+1),width(),skin.margin+row_height*(i+1));
		p.setPen( (row==selected)?skin.selected_font_color:skin.font_color );
		p.drawText(skin.margin,fontofs+row_height*(i+1)-fm.descent(),strings[row]);
	
	}
	
}

void PixmapList::mousePressEvent(QMouseEvent *e) {
	
	int row_height=skin.font_height+skin.separator;
	
	int row=(e->y()-skin.margin)/row_height;
	row+=view_offset;
	
	if (row<0 || row>=strings.size())
		return;
	
	selected=row;
	
	item_selected_signal(selected);
	
	update();
}
	
void PixmapList::set_item_text(int p_item,QString p_text) {
		
	ERR_FAIL_INDEX(p_item,strings.size());
	strings[p_item]=p_text;
	update();
}
void PixmapList::resizeEvent ( QResizeEvent * ) {
	
	
	update_scrollbar();
}

void PixmapList::update_scrollbar() {
	
	if (!scroll_bar)
		return;
	
	int row_height=skin.font_height+skin.separator;
	
	int visible_rows=(height()-skin.margin*2)/row_height;
	
	int scrollable_rows=strings.size()-visible_rows;
	
	if (scrollable_rows<=0)
		scroll_bar->hide();
	else 
		scroll_bar->show();
	
	scroll_bar->set_max(scrollable_rows);
	//scroll_bar->set_pagesize( visible_rows );
	if (visible_rows>0)
		scroll_bar->set_pagesize(visible_rows);
	
	scroll_bar->set_value( view_offset );
	
}

void PixmapList::scrollbar_value_changed(int p_to_val) {
	
	view_offset=p_to_val;
	update();
}


void PixmapList::add_item(QString p_item,int p_at_pos) {
	
	if (p_at_pos==-1)
		strings.push_back(p_item);
	
	update();
	update_scrollbar();
}
void PixmapList::clear() {
	
	strings.clear();
	view_offset=0;
	selected=-1;	
	update();
	update_scrollbar();
	
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
	if (scroll_bar) {
		
		QObject::connect(scroll_bar,SIGNAL(value_changed_signal( int )),this,SLOT(scrollbar_value_changed( int )));
		update_scrollbar();
	}
	
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
