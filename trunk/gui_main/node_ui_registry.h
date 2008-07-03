//
// C++ Interface: node_ui_registry
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef NODE_UI_REGISTRY_H
#define NODE_UI_REGISTRY_H


#include "engine/audio_node.h"
#include "gui_common/base_node_ui.h"
#include <list>

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

typedef BaseNodeUI* (*UI_CreateFunc)(AudioNode *p_node);

class NodeUI_Registry { 

	static std::list<UI_CreateFunc> create_func_array;
public:

	static BaseNodeUI *create_ui(AudioNode *p_node);
	static void add_create_ui_func(UI_CreateFunc p_func);

};

#endif
