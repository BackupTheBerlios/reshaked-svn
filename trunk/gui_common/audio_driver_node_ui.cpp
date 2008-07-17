//
// C++ Implementation: audio_driver_node_ui
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "audio_driver_node_ui.h"

#include "bundles/margin_group.h"
#include "containers/box_container.h"
#include "containers/stack_container.h"
#include "widgets/label.h"
#include "containers/grid_container.h"

void AudioDriverNodeUI::rescan() {

	std::list<String>::iterator I;
	
	
	std::list<String> fixed_list;
	
	driver_node->get_fixed_connectable_list(&fixed_list);	
	
	for (int i=0;i<driver_node->get_instanced_channels();i++) {
	
		combo_fixed[i]->clear();
		
		for (I=fixed_list.begin();I!=fixed_list.end();I++) {
		
			combo_fixed[i]->add_string(*I);
		}
	}
	
	
	std::list<String> named_list;
	
	driver_node->get_named_connectable_list(&named_list);	
	
	for (int i=0;i<driver_node->get_instanced_channels();i++) {
	
		combo_named[i]->clear();
		
		for (I=named_list.begin();I!=named_list.end();I++) {
		
			combo_named[i]->add_string(*I);
		}
	}
		
	
}

BaseNodeUI *AudioDriverNodeUI::ui_create_func(AudioNode *p_node) {

	if (dynamic_cast<AudioDriverNode*>( p_node )) {
	
		return new AudioDriverNodeUI( dynamic_cast<AudioDriverNode*>( p_node ) );
	}
	
	return NULL;
}


AudioDriverNodeUI::AudioDriverNodeUI(AudioDriverNode* p_node) : BaseNodeUI (p_node) {

	driver_node=p_node;

	GUI::MarginGroup *mg;
		
	mg = add( new GUI::MarginGroup("Connection Type:                    ") );
	GUI::HBoxContainer *hbc = mg->add( new GUI::HBoxContainer );
	
	bgroup.add_button( hbc->add( new GUI::Button("Fixed"),1 ) );
	bgroup.add_button( hbc->add( new GUI::Button("Named"),1 ) );
	
	
	if (driver_node->get_instanced_channels()==0)
		mg = add( new GUI::MarginGroup("Connection:") );
	else
		mg = add( new GUI::MarginGroup("Connections:") );
		
	GUI::StackContainer *stack = mg->add( new GUI::StackContainer );
	
	GUI::GridContainer *combo_grid;
	 
	combo_grid = stack->add( new GUI::GridContainer(2) );
	
	combo_fixed = new GUI::ComboBox*[driver_node->get_instanced_channels()];
	for (int i=0;i<driver_node->get_instanced_channels();i++) {
	
		combo_grid->add( new GUI::Label( String::num(i+1)+":" ), false, false );
		combo_fixed[i]=combo_grid->add( new GUI::ComboBox, true, false );
	}
	 
	combo_grid = stack->add( new GUI::GridContainer(2) );
	
	combo_named = new GUI::ComboBox*[driver_node->get_instanced_channels()];
	for (int i=0;i<driver_node->get_instanced_channels();i++) {
	
		combo_grid->add( new GUI::Label( String::num(i+1)+":" ), false, false );
		combo_named[i]=combo_grid->add( new GUI::ComboBox, true, false );
	}
		
	rescan();
	
	bgroup.current_button_changed_signal.connect( stack, &GUI::StackContainer::raise );
}


AudioDriverNodeUI::~AudioDriverNodeUI()
{
	if (combo_named)
		delete[] combo_named;
	if (combo_fixed)
		delete[] combo_fixed;
		
}


