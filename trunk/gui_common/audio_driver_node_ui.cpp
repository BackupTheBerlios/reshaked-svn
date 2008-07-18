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


BaseNodeUI *AudioDriverNodeUI::ui_create_func(AudioNode *p_node) {

	if (dynamic_cast<AudioDriverNode*>( p_node )) {
	
		return new AudioDriverNodeUI( dynamic_cast<AudioDriverNode*>( p_node ) );
	}
	
	return NULL;
}


void AudioDriverNodeUI::rescan() {

	scanning=true;
	
	std::list<String>::iterator I;
	
	
	std::list<String> fixed_list;
	
	driver_node->get_fixed_connectable_list(&fixed_list);	
	
	for (int i=0;i<driver_node->get_instanced_channels();i++) {
	
		combo_fixed[i]->clear();
		
		for (I=fixed_list.begin();I!=fixed_list.end();I++) {
		
			combo_fixed[i]->add_string(*I);
		}
		
		combo_fixed[i]->select( driver_node->get_fixed_connection(i) );
	}
	
	
	std::list<String> named_list;
	
	driver_node->get_named_connectable_list(&named_list);	
	
	for (int i=0;i<driver_node->get_instanced_channels();i++) {
	
		combo_named[i]->clear();
		
		int samenameidx=-1;
		int j=0;
		for (I=named_list.begin();I!=named_list.end();I++,j++) {
		
			combo_named[i]->add_string(*I);
			if (*I==driver_node->get_named_connection(i) ) {
			
				samenameidx=j;
			}
		}
		if (samenameidx!=-1)
			combo_fixed[i]->select( samenameidx );
	}
		
	scanning=false;
	
}


void AudioDriverNodeUI::combo_named_selected(int idx, int combo_idx) {

	if (scanning)
		return;
	
	driver_node->connect_named(combo_named[combo_idx]->get_string(idx),combo_idx);
}
void AudioDriverNodeUI::combo_fixed_selected(int idx, int combo_idx) {

	if (scanning)
		return;
	printf("connect fixed, port %i, connect to %i\n",combo_idx,idx);	
	driver_node->connect_fixed(idx,combo_idx);
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
		combo_fixed[i]->selected_signal.connect( GUI::Method1<int>( GUI::Method2<int,int>( this, &AudioDriverNodeUI::combo_fixed_selected), i ) );
	}
	 
	combo_grid = stack->add( new GUI::GridContainer(2) );
	
	combo_named = new GUI::ComboBox*[driver_node->get_instanced_channels()];
	for (int i=0;i<driver_node->get_instanced_channels();i++) {
	
		combo_grid->add( new GUI::Label( String::num(i+1)+":" ), false, false );
		combo_named[i]=combo_grid->add( new GUI::ComboBox, true, false );
		combo_named[i]->selected_signal.connect( GUI::Method1<int>( GUI::Method2<int,int>( this, &AudioDriverNodeUI::combo_named_selected), i ) );
	}
		
	scanning=false;
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


