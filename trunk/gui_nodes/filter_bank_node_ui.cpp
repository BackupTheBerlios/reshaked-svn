//
// C++ Implementation: filterbanknodeui
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "filter_bank_node_ui.h"
#include "gui_common/node_ui_label.h"
#include "gui_common/node_ui_value.h"
#include "widgets/knob.h"
#include "gui_common/common_skin.h"
#include "containers/grid_container.h"
#include "bundles/scroll_box.h"
#include "widgets/label.h"

#include "gui_common/control_port_range.h"


BaseNodeUI *FilterBankNodeUI::ui_create_func(AudioNode *p_node) {

	FilterBankNode *filterbank = dynamic_cast<FilterBankNode*>(p_node);
	if (!filterbank)
		return NULL;
		
	return new FilterBankNodeUI(filterbank);
}


void FilterBankNodeUI::check_ranges_user() {

	//if (display_ir)
	//	display_ir->check_for_changes();
}

void FilterBankNodeUI::set_in_window() {


	GUI::HBoxContainer *main_hbc = add( new GUI::HBoxContainer );
	
	GUI::GridContainer *grid = main_hbc->add( new GUI::GridContainer(3));
	
	grid->add( new GUI::Label( "#", GUI::Label::ALIGN_CENTER), false, false );
	grid->add( new GUI::Label( "Mode",GUI::Label::ALIGN_CENTER), false, false );
	grid->add( new GUI::Label( "Stages",GUI::Label::ALIGN_CENTER), false, false );
	
	for (int i=0;i<4;i++) {
	
		grid->add( new GUI::Label( String::num(i+1) ), false, false );
		GUI::Frame *mode = make_enum( "Filter"+String::num(i+1)+"/Mode",false );
		mode->add_constant_override( C_GENERIC_NODE_UI_ENUM_MIN_WIDTH, C_NODEUI_FILTERBANK_MODE_WIDTH );
		GUI::Frame *stages = make_enum( "Filter"+String::num(i+1)+"/Stages",false );
		stages->add_constant_override( C_GENERIC_NODE_UI_ENUM_MIN_WIDTH, C_NODEUI_FILTERBANK_STAGES_WIDTH );
		grid->add( mode, false, false );
		grid->add( stages, false, false );
	}
	
	//GUI::HBoxContainer *knobs_hbc = vb1->add( new GUI::HBoxContainer );
	
#if 0	
	knobs_hbc->add( make_knob( "PreDelay") );
	knobs_hbc->add( make_knob( "Pre-Fbk") );
	knobs_hbc->add( make_knob( "Pre-HPF") );
	knobs_hbc->add( make_knob( "RoomSize") );
	knobs_hbc->add( make_knob( "Damp") );
	knobs_hbc->add( make_knob( "Width") );
	
	display_ir = vb1->add( new ReverbDisplayIR );
	
	display_ir->set_properties( 
		node->get_control_port_by_name( AudioNode::PORT_IN, "PreDelay" ),
		node->get_control_port_by_name( AudioNode::PORT_IN, "Pre-Fbk" ),
		node->get_control_port_by_name( AudioNode::PORT_IN, "Pre-HPF" ),
		node->get_control_port_by_name( AudioNode::PORT_IN, "RoomSize" ),
		node->get_control_port_by_name( AudioNode::PORT_IN, "Damp" ),
		node->get_control_port_by_name( AudioNode::PORT_IN, "Width" ) );
		
	main_hbc->add( make_vslider("Wet") );
	main_hbc->add( make_vslider("Dry") );
#endif	
	
}

FilterBankNodeUI::FilterBankNodeUI(FilterBankNode *p_filterbank) : BaseNodeUI(p_filterbank) {

	node=p_filterbank;
	
}


FilterBankNodeUI::~FilterBankNodeUI()
{
}
