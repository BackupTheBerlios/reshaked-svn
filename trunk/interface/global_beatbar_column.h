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



class MarkerColumn : public Widget {


	GlobalView *global_view;
	BarMap *bar_map;
	Editor *editor;
	ValueStream<int,String> *marker_list;	
	
	bool inside;
	int inside_beat;
	int marker_beat;
	
	Size get_minimum_size_internal();
	
	void paint_marker(Painter& p, int p_marker_idx,int p_w);
	
	virtual void draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed);
	virtual void mouse_button(const Point& p_pos, int p_button,bool p_press,int p_modifier_mask);
	virtual void mouse_leave();
	virtual void mouse_motion(const Point& p_pos, const Point& p_rel, int p_button_mask);
				       
	StringInputDialog *input_dialog;
	void set_in_window();
	void marker_edited(String p_text);
public:
	
	void set_global_view(GlobalView *p_global_view);
	MarkerColumn(Editor *p_editor);
	~MarkerColumn();
	
};
				       
class GlobalBeatBarColumn : public Widget {
				       
	GlobalView *global_view;
	BarMap *bar_map;
	Editor *editor;

	Size get_minimum_size_internal();
	
	virtual void draw(const Point& p_pos,const Size& p_size,const Rect& p_exposed);

public:
	
	void set_global_view(GlobalView *p_global_view);
	
	GlobalBeatBarColumn(Editor *p_editor);
	~GlobalBeatBarColumn();

};


}

#endif
