//
// C++ Interface: global_beatbar_column
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDGLOBAL_BEATBAR_COLUMN_H
#define RESHAKEDGLOBAL_BEATBAR_COLUMN_H

#include <Qt/qwidget.h>
#include "interface/global_view.h"

namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class GlobalBeatBarColumn : public QWidget {
				       
	GlobalView *global_view;
	BarMap *bar_map;
				       
	void paintEvent(QPaintEvent *p);
public:
	
	void set_global_view(GlobalView *p_global_view);
	
	GlobalBeatBarColumn(QWidget *p_parent,BarMap *p_bar_map);
	~GlobalBeatBarColumn();

};

}

#endif
