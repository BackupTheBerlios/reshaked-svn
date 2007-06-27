//
// C++ Interface: blocklist_ui_base
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDBLOCKLIST_UI_BASE_H
#define RESHAKEDBLOCKLIST_UI_BASE_H

#include "base/widget.h"
#include "editor/editor.h"


using namespace GUI;

namespace ReShaked {

/**
	@author red <red@killy>
*/

class EditViewBlockList : public Widget {

	
	int current_line;
	Editor *_editor;
	bool _mouse_selecting;
protected:	
	
	void pre_parse_mouse_button_event(const Point& p_pos, int p_button,bool p_press,int p_modifier_mask);

	virtual void get_pos_at_pointer(Point p_pointer, int *p_blocklist,int *p_column, int *p_row)=0;
	
	void mouse_selection_begin(Point p_pos);
	void mouse_selection_update_check();
	void mouse_selection_end();
	
	int get_row_height();
	
	int get_current_line();
	void set_current_line(int p_line);
	
	virtual void fix_pre_scroll(int p_scroll);
	virtual bool can_scroll();
	
public: //signals
	Signal< Method1<EditViewBlockList*> > blocklist_ui_under_cursor_request_signal;
	
public:
	void set_blocklist_ui_under_cursor(EditViewBlockList *p_ui, Point p_pos);
	
	void update_viewport_pos(int p_new_line);
	EditViewBlockList(Editor *p_editor);
	~EditViewBlockList();

};

}

#endif
