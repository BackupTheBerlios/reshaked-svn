//
// C++ Implementation: chorus_node_ui
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "chorus_node_ui.h"
#include "gui_common/node_ui_label.h"
#include "gui_common/node_ui_value.h"
#include "widgets/knob.h"
#include "gui_common/common_skin.h"
#include "containers/grid_container.h"
#include "bundles/scroll_box.h"
#include "widgets/label.h"
#include "bundles/tab_box.h"
#include "gui_common/control_port_range.h"


BaseNodeUI *ChorusNodeUI::ui_create_func(AudioNode *p_node) {

	ChorusNode *chorus = dynamic_cast<ChorusNode*>(p_node);
	if (!chorus)
		return NULL;
		
	return new ChorusNodeUI(chorus);
}

void ChorusNodeUI::tab_changed_slot(int p_to) {


	if (chorus_ui_data)
		chorus_ui_data->current_page=p_to;
	printf("tab changed to %i, uidata %p\n",p_to,chorus_ui_data);
}
void ChorusNodeUI::set_in_window() {


	chorus_ui_data = dynamic_cast<ChorusUIData*>( node->get_ui_data() );
	
	if (!chorus_ui_data) {
	
		chorus_ui_data= new ChorusUIData;
		node->set_ui_data(chorus_ui_data);
	}
	
	

	GUI::HBoxContainer *main_hbc = add( new GUI::HBoxContainer );
	
	GUI::TabBox *tabbox = main_hbc->add( new GUI::TabBox );
	
	for (int i=0;i<4;i++) {
		
		String prefix="Voice"+String::num(i+1)+"/";
		
		GUI::GridContainer *grid = tabbox->add("Voice "+String::num(i+1),new GUI::GridContainer(4));
		
		grid->add( make_knob(prefix+"Level"), false, false );
		grid->add( make_knob(prefix+"Delay"), false, false);
		grid->add( make_knob(prefix+"Rate"), false, false );
		grid->add( make_knob(prefix+"Depth"), false, false );
	
		grid->add( make_knob(prefix+"Phase"), false, false );
		grid->add( make_knob(prefix+"Width"), false, false );
		grid->add( make_knob(prefix+"LPF"), false, false );
		grid->add( make_knob(prefix+"Pan"), false, false );
		//grid->add( make_knob(prefix+"Depth") );
	
	}
	
	
	main_hbc->add( make_vslider("Wet") );
	main_hbc->add( make_vslider("Dry") );
	
	tabbox->raise( chorus_ui_data->current_page ); 
	
	tabbox->get_tabs()->tab_changed_signal.connect( this, &ChorusNodeUI::tab_changed_slot );
	
}

ChorusNodeUI::ChorusNodeUI(ChorusNode *p_chorus) : BaseNodeUI(p_chorus) {

	node=p_chorus;
	chorus_ui_data=NULL;
}


ChorusNodeUI::~ChorusNodeUI()
{
}
