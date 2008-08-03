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
#include "widgets/slider.h"
#include "gui_common/control_port_range.h"


BaseNodeUI *FilterBankNodeUI::ui_create_func(AudioNode *p_node) {

	FilterBankNode *filterbank = dynamic_cast<FilterBankNode*>(p_node);
	if (!filterbank)
		return NULL;
		
	return new FilterBankNodeUI(filterbank);
}


void FilterBankNodeUI::check_ranges_user() {

	if (fr)
		fr->check_for_changes();
}

void FilterBankNodeUI::set_in_window() {


	GUI::HBoxContainer *main_hbc = add( new GUI::HBoxContainer );
	
	GUI::GridContainer *grid = main_hbc->add( new GUI::GridContainer(3));
	grid->add_stylebox_override( GUI::SB_CONTAINER, SB_NODEUI_FILTER_BANK_SECTION_BOX );
	
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
	
	GUI::VBoxContainer * fr_vb = main_hbc->add( new GUI::VBoxContainer );
	fr = fr_vb->add( new FilterBankFR(4),1 );
	
	fr_vb->add_stylebox_override( GUI::SB_CONTAINER, SB_NODEUI_FILTER_BANK_SECTION_BOX );
	
	for (int i=0;i<4;i++) {
	
		String prefix = "Filter"+String::num(i+1)+"/";
		void set_cutoff_range(int p_which,GUI::Range *p_prop);
		void set_resonance_range(int p_which,GUI::Range *p_prop);
		void set_mode_range(int p_filter,GUI::Range *p_prop);
		void set_stages_range(int p_filter,GUI::Range *p_prop);
		void set_cutoff_offset_range(GUI::Range *p_prop);		
	
		
	
		fr->set_cutoff_range( i, new ControlPortRange(node->get_control_port_by_name( AudioNode::PORT_IN, prefix+"Cutoff")) );
		fr->set_resonance_range( i, new ControlPortRange(node->get_control_port_by_name( AudioNode::PORT_IN, prefix+"Resonance")) );
		fr->set_mode_range( i, new ControlPortRange(node->get_control_port_by_name( AudioNode::PORT_IN, prefix+"Mode")) );
		fr->set_stages_range( i, new ControlPortRange(node->get_control_port_by_name( AudioNode::PORT_IN, prefix+"Stages")) );
	}
	
	ControlPortRange * offset_range = new ControlPortRange(node->get_control_port_by_name( AudioNode::PORT_IN, "Offset"));
	fr->set_cutoff_offset_range( offset_range ); // this will own it 
	
	GUI::HSlider *ofs = fr_vb->add( new GUI::HSlider );
	
	ofs->set_range( offset_range, false ); // this will share it
	
	register_range_for_updates( offset_range );
	
	
	
	//GUI::HBoxContainer *knobs_hbc = vb1->add( new GUI::HBoxContainer );
	
#if 0	
	knobs_hbc->add( make_knob( "PreDelay") );
	knobs_hbc->add( make_knob( "Pre-Fbk") );
	knobs_hbc->add( make_knob( "Pre-HPF") );
	knobs_hbc->add( make_knob( "RoomSize") );
	knobs_hbc->add( make_knob( "Damp") );
	knobs_hbc->add( make_knob( "Width") );
	
	fr = vb1->add( new ReverbDisplayIR );
	
	fr->set_properties( 
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
