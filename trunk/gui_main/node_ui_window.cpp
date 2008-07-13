//
// C++ Implementation: node_ui_window
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "node_ui_window.h"

void NodeUI_Window::edited_callback( AudioNode *p_node) {

	edited_signal.call(p_node);
}

void NodeUI_Window::update_controls_callback() {

	if (node_ui && is_visible())
		node_ui->check_ranges();
}

void NodeUI_Window::edit_node(const GUI::Point& p_location, AudioNode *p_node) {

	if (node_ui)
		delete node_ui;
		
	node_ui=NULL;	
	node_ui=NodeUI_Registry::create_ui( p_node );
	node_ui->edited_signal.connect( this, &NodeUI_Window::edited_callback );
	container->add( node_ui, 1 );
	
	set_size( GUI::Size(1,1));
	set_pos(p_location);
	show();
	
}

NodeUI_Window::NodeUI_Window(GUI::Window *p_parent) : GUI::Window(p_parent,GUI::Window::MODE_POPUP,GUI::Window::SIZE_NORMAL)  {

	container = new GUI::VBoxContainer;
	
	set_root_frame(container);	
	node_ui=NULL;

	GUI::Timer *t=get_timer();
	timer = t->create_timer( GUI::Method( this, &NodeUI_Window::update_controls_callback),200 );
	

}


NodeUI_Window::~NodeUI_Window() {

	if (timer!=GUI::INVALID_TIMER_ID && get_timer())
		get_timer()->remove_timer(timer);
}


