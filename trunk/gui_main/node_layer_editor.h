//
// C++ Interface: node_layer_editor
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef NODE_LAYER_EDITOR_H
#define NODE_LAYER_EDITOR_H


#include "engine/audio_graph.h"
#include "base/window.h"
#include "widgets/button_group.h"
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class NodeLayerEditor : public GUI::Window {
	
	AudioGraph *audio_graph;

	GUI::ButtonGroup layer_group;

	void group_changed(int p_layer);

	bool updating_layer;

	AudioNode *node;

public:

	void edit( AudioNode *p_node );

	NodeLayerEditor(AudioGraph *p_audio_graph,GUI::Window *p_parent);	
	~NodeLayerEditor();

};

#endif
