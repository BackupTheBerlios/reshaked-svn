//
// C++ Interface: global_view_cursor
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDGLOBAL_VIEW_CURSOR_H
#define RESHAKEDGLOBAL_VIEW_CURSOR_H


#include "widgets/menu_button.h"
#include "widgets/slider.h"
#include "widgets/button_group.h"
#include "containers/box_container.h"

#include "interface/global_view.h"

using namespace GUI;

namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class GlobalViewCursor : public HBoxContainer {
				       

public:					       

	enum {
		MAX_MODES=4
	};
	
	GlobalView::EditMode mode;
	
	MenuButton *mode_button[MAX_MODES];
	MenuButton *delete_selected;
	MenuButton *select_linked;
	MenuButton *unlink_selected;
	HSlider *zoom;
	void add_button(GlobalView::EditMode p_mode, int p_bitmap_list_idx,String p_hint);

	bool inited;
 	void set_in_window();
	
	ButtonGroup group;
	
public:	
	
	Signal<> repeat_set_signal;
	Signal<> repeat_unset_signal;
	Signal< Method1<int> > edit_mode_changed_signal;
	Signal<> delete_clicked_signal;
	Signal<> select_linked_signal;
	Signal<> unlink_selected_signal;
	Signal< Method1<float> > zoom_changed_signal;

//slots
	void mode_selected(int p_mode);

public:
	
	GlobalViewCursor();
	~GlobalViewCursor();

};

}

#endif
