//
// C++ Interface: pixmap_list
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDPIXMAP_LIST_H
#define RESHAKEDPIXMAP_LIST_H

#include <Qt/qwidget.h>
#include "ui_blocks/pixmap_scrollbar.h"
#include "ui_blocks/helpers.h"


namespace ReShaked {

/**
	@author red <red@killy>
*/
class PixmapList : public QWidget {
	
	Q_OBJECT
	
public:	
	
	struct Skin {
		
		SkinBox *skin_bg;
		QColor font_color;
		QColor selected_font_color;
		QColor selected_bg_color;
		QColor separator_color;
		int separator;
		int font_height;
		int row_margin;
	};
	
private:	
	
	Skin skin;
	std::vector<QString> strings;
	PixmapScrollBar *scroll_bar;
	
	int view_offset;
	int selected;	
	
	void paintEvent(QPaintEvent *e);
	void mousePressEvent(QMouseEvent *e);
	
private slots:	
	
	void scrollbar_value_changed(int p_to_val);
	
signals:	
	
	void item_selected_signal(int p_which);
public:
	
	int get_item_count();
	
	void add_item(QString p_item,int p_at_pos=-1);
	void clear();
	
	int get_selected();
	
	void select_item(int p_index);
	
	void set_scrollbar(PixmapScrollBar *p_scroll_bar);
	PixmapList(QWidget *p_parent,const Skin& p_skin);
	~PixmapList();

};

}

#endif
