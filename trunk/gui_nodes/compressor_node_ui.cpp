//
// C++ Implementation: compressor_node_ui
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "compressor_node_ui.h"

#include "gui_common/node_ui_label.h"
#include "gui_common/node_ui_value.h"
#include "widgets/knob.h"
#include "gui_common/common_skin.h"
#include "containers/grid_container.h"
#include "containers/center_container.h"
#include "widgets/label.h"
#include "gui_common/control_port_range.h"


BaseNodeUI *CompressorNodeUI::ui_create_func(AudioNode *p_node) {

	CompressorNode *compressor = dynamic_cast<CompressorNode*>(p_node);
	if (!compressor)
		return NULL;
		
	return new CompressorNodeUI(compressor);
}


void CompressorNodeUI::check_ranges_user() {

	if (display_curve)
		display_curve->check_for_changes();
}

void CompressorNodeUI::set_in_window() {


	GUI::HBoxContainer *main_hbc = add( new GUI::HBoxContainer );
	
	main_hbc->add( make_vslider("PreGain") );
	
	GUI::GridContainer *grid1 = main_hbc->add( new GUI::GridContainer(2) );
	
	grid1->add( make_knob( "Threshold"),false,false );
	grid1->add( make_knob( "Ratio"),false,false );
	grid1->add( make_knob( "Attack"),false,false );
	grid1->add( make_knob( "Release"),false,false );
	
	display_curve=main_hbc->add( new GUI::CenterContainer )->set( new CompressorCurveDisplay(
		node->get_control_port_by_name( AudioNode::PORT_IN, "Threshold" ),
		node->get_control_port_by_name( AudioNode::PORT_IN, "Ratio" )
	));
	
	main_hbc->add( make_vslider("PostGain") );
	
}

CompressorNodeUI::CompressorNodeUI(CompressorNode *p_compressor) : BaseNodeUI(p_compressor) {

	node=p_compressor;
	display_curve=NULL;
}


CompressorNodeUI::~CompressorNodeUI()
{
}
