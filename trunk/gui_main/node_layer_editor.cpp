
//
// C++ Implementation: node_layer_editor
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "node_layer_editor.h"
#include "widgets/bitmap_button.h"
#include "gui_common/common_skin.h"
#include "containers/center_container.h"
#include "containers/grid_container.h"
#include "containers/box_container.h"
#include "audio_graph_widget.h"
#include "editor/edit_commands.h"

static void create_buttons(GUI::BoxContainer *p_container,GUI::ButtonGroup *p_group,int p_offset,int p_count) {

	GUI::HBoxContainer *hbc = p_container->add( new GUI::HBoxContainer );

	for(int i=0;i<p_count;i++) {
		
		GUI::BitmapButton *bb = hbc->add( new GUI::BitmapButton );
		
		bb->add_bitmap_override( GUI::BITMAP_BUTTON_NORMAL, BITMAP_LAYER_BUTTON_0 + i + p_offset );
		bb->add_bitmap_override( GUI::BITMAP_BUTTON_HOVER, BITMAP_LAYER_BUTTON_0_HOVER + i + p_offset);
		bb->add_bitmap_override( GUI::BITMAP_BUTTON_PRESSED, BITMAP_LAYER_BUTTON_0_PRESSED + i + p_offset);
		p_group->add_button( bb );
			
	};
	
	hbc->set_separation(0);

}

void NodeLayerEditor::group_changed(int p_layer) {

	if (updating_layer)
		return;

	if (p_layer<16) {
	
		EditCommands::get_singleton()->audio_graph_set_node_layer(audio_graph,node,1<<p_layer);
	} else {
	
		EditCommands::get_singleton()->audio_graph_set_node_layer(audio_graph,node,AudioNode::LAYER_ALWAYS_VISIBLE);
	
	}
}

void NodeLayerEditor::edit( AudioNode *p_node ) {

	node=p_node;
	updating_layer=true;
	int layer=p_node->get_layer();
	
	if (layer<0 || layer>=16)
		layer_group.set_current(16);
	else {
	
		for (int i=0;i<16;i++)
			if (layer&(1<<i))
				layer_group.set_current(i);
			
	}
	
	show();
	updating_layer=false;
}

NodeLayerEditor::NodeLayerEditor(AudioGraph *p_audio_graph,GUI::Window *p_parent) : GUI::Window(p_parent,GUI::Window::MODE_POPUP,GUI::Window::SIZE_CENTER) {

	audio_graph=p_audio_graph;
	
	GUI::HBoxContainer *hbc = new GUI::HBoxContainer;
	
	set_root_frame(hbc);
		
	GUI::BitmapButton *bb_all = new GUI::BitmapButton;
		
	bb_all->add_bitmap_override( GUI::BITMAP_BUTTON_NORMAL, BITMAP_LAYER_BUTTON_ALL );
	bb_all->add_bitmap_override( GUI::BITMAP_BUTTON_HOVER, BITMAP_LAYER_BUTTON_ALL_HOVER  );
	bb_all->add_bitmap_override( GUI::BITMAP_BUTTON_PRESSED, BITMAP_LAYER_BUTTON_ALL_PRESSED );
			
	GUI::VBoxContainer *vbc1 = hbc->add( new GUI::VBoxContainer );
	vbc1->set_separation(0);
		
	create_buttons(vbc1,&layer_group,0,4);
	create_buttons(vbc1,&layer_group,4,4);
	
	hbc->add( bb_all );
	
	GUI::VBoxContainer *vbc2 = hbc->add( new GUI::VBoxContainer );
	vbc2->set_separation(0);
	
	create_buttons(vbc2,&layer_group,8,4);
	create_buttons(vbc2,&layer_group,12,4);
	
	layer_group.add_button( bb_all );
		
	hbc->set_separation(0);
		
	layer_group.current_button_changed_signal.connect( this, &NodeLayerEditor::group_changed );
	
	updating_layer=false;
}


NodeLayerEditor::~NodeLayerEditor() {

}


