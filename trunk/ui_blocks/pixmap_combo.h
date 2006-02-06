//
// C++ Interface: pixmap_combo
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDPIXMAP_COMBO_H
#define RESHAKEDPIXMAP_COMBO_H

#include "ui_blocks/pixmap_label.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
class PixmapCombo : public PixmapLabel {
			    
	Q_OBJECT
			    
	std::vector<QString> string_list;			    
	int selected;
	
	void click_override();
	void wheelEvent ( QWheelEvent * e ) ;
	
	void update_internal();
signals:
			
	void item_selected(int p_idx);
public:
	void clear();
	void add_item(QString p_item);
	void set_item_text(int p_item,QString p_text);
	void select_item(int p_item);
	int get_selected();
	int get_item_count();
	
	PixmapCombo(QWidget *p_parent,QPixmap p_bg);
	~PixmapCombo();

};

}

#endif
