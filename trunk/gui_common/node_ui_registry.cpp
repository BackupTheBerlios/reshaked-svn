//
// C++ Implementation: node_ui_registry
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "node_ui_registry.h"


std::list<UI_CreateFunc> NodeUI_Registry::create_func_array;

BaseNodeUI *NodeUI_Registry::create_ui(AudioNode *p_node) {

	for (std::list<UI_CreateFunc>::iterator I=create_func_array.begin();I!=create_func_array.end();I++) {
	
		BaseNodeUI *ui=(*I)(p_node);
		if (ui)
			return ui;
	}
	
	return NULL;
}

void NodeUI_Registry::add_create_ui_func(UI_CreateFunc p_func) {

	create_func_array.push_back(p_func);
}


