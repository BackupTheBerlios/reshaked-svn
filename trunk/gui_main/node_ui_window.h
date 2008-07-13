//
// C++ Interface: node_ui_window
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef NODE_UI_WINDOW_H
#define NODE_UI_WINDOW_H

#include "base/window.h"
#include "base/widget.h"
#include "gui_common/node_ui_registry.h"
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class NodeUI_Window : public GUI::Window {

	GUI::VBoxContainer *container;
	BaseNodeUI *node_ui;
	GUI::TimerID timer;
	
	void update_controls_callback();
	void edited_callback( AudioNode *p_node);
public:

	GUI::Signal< GUI::Method1< AudioNode* > > edited_signal;

	void edit_node(const GUI::Point& p_location, AudioNode *p_node);

	NodeUI_Window(GUI::Window *p_parent);
	
	~NodeUI_Window();

};

#endif
