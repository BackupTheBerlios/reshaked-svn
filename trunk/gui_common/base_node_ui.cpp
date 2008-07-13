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


void BaseNodeUI::register_range_for_updates(ControlPortRange *p_range) {

	updatable_ranges.push_back(p_range);
}

void BaseNodeUI::presets_callback() {

	edited_signal.call(_node);
}

void BaseNodeUI::check_ranges() {

	for (std::list<ControlPortRange*>::iterator I=updatable_ranges.begin(); I != updatable_ranges.end() ; I++ )
		(*I)->check_for_changes();
}

BaseNodeUI::BaseNodeUI(AudioNode *p_node) {

	_node=p_node;
	GUI::HBoxContainer *hbc= add( new GUI::HBoxContainer );
	hbc->add( new GUI::Widget, 1 );
	GUI::Button *preset = hbc->add( new GUI::Button );
	preset->pressed_signal.connect( this, &BaseNodeUI::presets_callback );
	preset->add_bitmap_override( GUI::BITMAP_BUTTON_DEFAULT_ICON, BITMAP_GRAPH_NODE_PRESET );
			
	
}


BaseNodeUI::~BaseNodeUI() {

	
}


