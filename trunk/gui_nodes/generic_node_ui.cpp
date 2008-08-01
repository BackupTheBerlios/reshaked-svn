//
// C++ Implementation: generic_node_ui
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "generic_node_ui.h"
#include "gui_common/node_ui_label.h"
#include "gui_common/node_ui_value.h"
#include "widgets/knob.h"
#include "gui_common/common_skin.h"
#include "containers/grid_container.h"
#include "bundles/scroll_box.h"
#include "widgets/label.h"
#include "gui_common/control_port_range.h"

BaseNodeUI *GenericNodeUI::ui_create_func(AudioNode *p_node) {

	
	return new GenericNodeUI(p_node);
}


void GenericNodeUI::set_in_window() {

	int max=constant(C_GENERIC_NODE_UI_ROW_MAX)*constant(C_GENERIC_NODE_UI_ROW_SIZE);
	
	int node_max=node->get_port_count( AudioNode::PORT_CONTROL, AudioNode::PORT_IN ) + node->get_port_count( AudioNode::PORT_CONTROL, AudioNode::PORT_OUT );
	
	GUI::GridContainer *port_grid;
	
	if (node_max>max) {
	
		GUI::ScrollBox *sbox = add( new GUI::ScrollBox,1 );
		sbox->set_scroll_v(true);
		sbox->set_scroll_h(false);
		sbox->set_expand_v(true);
		sbox->set_expand_h(true);
		sbox->set_minimum_size( GUI::Size(0, constant( C_GENERIC_NODE_UI_SCROLL_HEIGHT ) ) );
		
		port_grid = sbox->set( new GUI::GridContainer( constant(C_GENERIC_NODE_UI_ROW_SIZE) ) );
		
	} else {
	
		port_grid = add( new GUI::GridContainer( constant(C_GENERIC_NODE_UI_ROW_SIZE) ), 1 );	
	}
	
	for (int i=0;i<node->get_port_count( AudioNode::PORT_CONTROL, AudioNode::PORT_IN );i++) {
	
		port_grid->add( make_knob( node->get_control_port( AudioNode::PORT_IN, i ) ), false, false );
		//port_grid->add( make_vslider( node->get_control_port( AudioNode::PORT_IN, i ) ), false, false );
		
	}
	
	
}

GenericNodeUI::GenericNodeUI(AudioNode *p_node) : BaseNodeUI(p_node) {

	node=p_node;
}


GenericNodeUI::~GenericNodeUI()
{
}


