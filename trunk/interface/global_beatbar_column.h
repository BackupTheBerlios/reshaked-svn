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

#include "base/widget.h"
#include "bundles/popup_menu.h"
#include "bundles/input_dialog.h"

#include "interface/global_view.h"
#include "editor/editor.h"

using namespace GUI;

namespace ReShaked {




class LoopColumn : public Widget {


	enum PopUpOptions {
		
		OPTION_SET_LOOP_BEGIN,
		OPTION_SET_LOOP_END,
		OPTION_CHANGE_BAR_LENGTH,
		OPTION_REMOVE_BAR_LENGTH
	};
	
	PopUpMenu *menu;
	GlobalView *global_view;
	Editor *editor;
	
	bool inside;
	int inside_beat;

	int beat_at_click;

	Size get_minimum_size_internal();
	virtual void draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed);
	virtual void mouse_button(const Point& p_pos, int p_button,bool p_press,int p_modifier_mask);
	virtual void mouse_leave();
	virtual void mouse_motion(const Point& p_pos, const Point& p_rel, int p_button_mask);
	
	
	void numeric_input_dialog_beat(double p_val);
	void option_selected(int p_option);
				       
	NumericInputDialog *input_dialog;
	void set_in_window();
public:
	
	Signal<> redraw_global_view_signal;
	
	void set_global_view(GlobalView *p_global_view);
	LoopColumn(Editor *p_editor);
	~LoopColumn();
	
};

/*

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
*/
}

#endif
