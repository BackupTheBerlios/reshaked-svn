//
// C++ Implementation: base_node_ui
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "base_node_ui.h"
#include "common_skin.h" 
#include "widgets/label.h"

void BaseNodeUI::presets_callback() {


}

BaseNodeUI::BaseNodeUI(AudioNode *p_node) {

	GUI::HBoxContainer *hbc= add( new GUI::HBoxContainer );
	hbc->add( new GUI::Widget, 1 );
	GUI::Button *preset = hbc->add( new GUI::Button );
	preset->add_bitmap_override( GUI::BITMAP_BUTTON_DEFAULT_ICON, BITMAP_GRAPH_NODE_PRESET );
		

}


BaseNodeUI::~BaseNodeUI() {

}


