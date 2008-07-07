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


void NodeUI_Window::edit_node(const GUI::Point& p_location, AudioNode *p_node) {

	if (node_ui)
		delete node_ui;
		
	node_ui=NULL;	
	node_ui=NodeUI_Registry::create_ui( p_node );
	
	container->add( node_ui, 1 );
	
	set_size( GUI::Size(1,1));
	set_pos(p_location);
	show();
	
}

NodeUI_Window::NodeUI_Window(GUI::Window *p_parent) : GUI::Window(p_parent,GUI::Window::MODE_POPUP,GUI::Window::SIZE_NORMAL)  {

	container = new GUI::VBoxContainer;
	
	set_root_frame(container);	
	node_ui=NULL;
}


NodeUI_Window::~NodeUI_Window()
{
}


