//
// C++ Interface: control_port_visibility_editor
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef CONTROL_PORT_VISIBILITY_EDITOR_H
#define CONTROL_PORT_VISIBILITY_EDITOR_H


#include "engine/audio_node.h"
#include "base/window.h"
#include "containers/box_container.h"
#include "widgets/tree.h"
#include "engine/audio_graph.h"

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class ControlPortVisibilityEditor : public GUI::Window {
	
	AudioGraph *audio_graph;
	GUI::Tree *tree;

	struct BindData {
	
		AudioNode *audio_node;
		ControlPort *control_port;
		AudioNode::PortFlow flow;
		int port_idx;
		GUI::TreeItem* item;		
	};

	void changed_callback(int p_cell, BindData p_data);
public:

	void edit(AudioNode *p_node);

	ControlPortVisibilityEditor(AudioGraph *p_graph,GUI::Window *p_parent);	
	~ControlPortVisibilityEditor();

};

#endif
