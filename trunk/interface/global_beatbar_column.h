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
#include "editor/editor.h"

namespace ReShaked {




class LoopColumn : public QWidget {


	GlobalView *global_view;
	Editor *editor;
	
	bool inside;
	int inside_beat;


	void paintEvent(QPaintEvent *p);
	void mousePressEvent(QMouseEvent *e);
	
	void mouseMoveEvent(QMouseEvent *e);
	void enterEvent(QEvent *ee);
	void leaveEvent(QEvent *ee);
				       
public:
	
	void set_global_view(GlobalView *p_global_view);
	LoopColumn(QWidget *p_parent,Editor *p_editor);
	
};


class MarkerColumn : public QWidget {


	GlobalView *global_view;
	BarMap *bar_map;
	Editor *editor;
	ValueStream<int,String> *marker_list;	
	
	bool inside;
	int inside_beat;

	
	void paint_marker(QPainter& p, int p_marker_idx);
	void paintEvent(QPaintEvent *p);
	void mousePressEvent(QMouseEvent *e);
	
	void mouseMoveEvent(QMouseEvent *e);
	void enterEvent(QEvent *ee);
	void leaveEvent(QEvent *ee);
				       
public:
	
	void set_global_view(GlobalView *p_global_view);
	MarkerColumn(QWidget *p_parent,Editor *p_editor);
	
};
					       
class GlobalBeatBarColumn : public QWidget {
				       
	GlobalView *global_view;
	BarMap *bar_map;
	Editor *editor;

	void paintEvent(QPaintEvent *p);

public:
	
	void set_global_view(GlobalView *p_global_view);
	
	GlobalBeatBarColumn(QWidget *p_parent,Editor *p_editor);
	~GlobalBeatBarColumn();

};

}

#endif
