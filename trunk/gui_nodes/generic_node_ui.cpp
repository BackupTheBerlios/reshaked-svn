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

class _GenericNodeUI_KnobMargin : public GUI::Widget {

	virtual GUI::Size get_minimum_size_internal() {
	
		return GUI::Size( constant(C_GENERIC_NODE_UI_KNOB_MARGIN), 0 );
	}

public:
};


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
	
		GUI::VBoxContainer *vbc = port_grid->add( new GUI::VBoxContainer, false, false );
		
		ControlPort *cp = node->get_control_port( AudioNode::PORT_IN, i );
		
		NodeUI_Label * label = vbc->add( new NodeUI_Label );
		label->set_text( cp->get_name() );
		
		ControlPortRange *r=new ControlPortRange( cp );
		register_range_for_updates(r);
		
		switch (cp->get_hint()) {
		
			default: {
			
				GUI::HBoxContainer *knob_hbc = vbc->add( new GUI::HBoxContainer );
				
				knob_hbc->add( new _GenericNodeUI_KnobMargin );
				GUI::Knob *knob = knob_hbc->add( new GUI::Knob );		
				knob_hbc->add( new _GenericNodeUI_KnobMargin );
				
				knob->set_range( r );
			} break;
		}
		
		NodeUI_Value * value = vbc->add( new NodeUI_Value );
		if (r)
			value->set_range( r, true );
		
	}
	
	
}

GenericNodeUI::GenericNodeUI(AudioNode *p_node) : BaseNodeUI(p_node) {

	node=p_node;
}


GenericNodeUI::~GenericNodeUI()
{
}


