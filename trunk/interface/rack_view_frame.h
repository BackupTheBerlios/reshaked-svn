//
// C++ Interface: rack_view_frame
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDRACK_VIEW_FRAME_H
#define RESHAKEDRACK_VIEW_FRAME_H

#include "containers/box_container.h"
#include "containers/stack_container.h"
#include "bundles/scroll_box.h"
#include "bundles/combo_box.h"
#include "interface/gui_update_notify.h"
#include "interface/plugin_chooser.h"
#include "widgets/button_group.h"
#include "widgets/label.h"
#include "editor/editor.h"

using namespace GUI;

namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class RackViewFrame : public VBoxContainer {

	Editor *editor;
	
	HBoxContainer *action_hb;
	ButtonGroup front_back;
	StackContainer *viewport_stack;
	ScrollBox *viewport_rack;
	ScrollBox *viewport_connect;
	Label *preset_name;
	ComboBox *tracks_combo;
	
	PluginChooser *plugin_chooser;
	
	int selected_track;
	bool updating;
	
	void set_in_window();
	
	void track_list_changed_slot();
	
	void update_rack();
	void update_track_combo();
	
	void track_combo_changed_slot(int p_track);
	
	void choose_plugin();
public:
	
	void update_all();
	
	RackViewFrame(GUI_UpdateNotify *p_notifier,Editor * p_editor);
	~RackViewFrame();
};

}

#endif
