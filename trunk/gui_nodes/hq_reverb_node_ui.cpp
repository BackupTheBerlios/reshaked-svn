//
// C++ Implementation: hq_reverb_node_ui
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "hq_reverb_node_ui.h"
#include "gui_common/node_ui_label.h"
#include "gui_common/node_ui_value.h"
#include "widgets/knob.h"
#include "gui_common/common_skin.h"
#include "containers/grid_container.h"
#include "bundles/scroll_box.h"
#include "widgets/label.h"
#include "gui_common/control_port_range.h"


BaseNodeUI *HQ_ReverbNodeUI::ui_create_func(AudioNode *p_node) {

	HQ_ReverbNode *hq_reverb = dynamic_cast<HQ_ReverbNode*>(p_node);
	if (!hq_reverb)
		return NULL;
		
	return new HQ_ReverbNodeUI(hq_reverb);
}


void HQ_ReverbNodeUI::check_ranges_user() {

	if (display_ir)
		display_ir->check_for_changes();
}

void HQ_ReverbNodeUI::set_in_window() {


	GUI::HBoxContainer *main_hbc = add( new GUI::HBoxContainer );
	
	GUI::VBoxContainer *vb1 = main_hbc->add( new GUI::VBoxContainer );
	
	GUI::HBoxContainer *knobs_hbc = vb1->add( new GUI::HBoxContainer );
	
	
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
	
	
}

HQ_ReverbNodeUI::HQ_ReverbNodeUI(HQ_ReverbNode *p_hq_reverb) : BaseNodeUI(p_hq_reverb) {

	node=p_hq_reverb;
	display_ir=NULL;
}


HQ_ReverbNodeUI::~HQ_ReverbNodeUI()
{
}
