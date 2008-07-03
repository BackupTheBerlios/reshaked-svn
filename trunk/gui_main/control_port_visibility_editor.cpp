//
// C++ Implementation: control_port_visibility_editor
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "control_port_visibility_editor.h"

#include "bundles/scroll_box.h"
#include "bundles/window_box.h"
#include "editor/edit_commands.h"

void ControlPortVisibilityEditor::changed_callback(int p_cell, BindData p_data) {

	EditCommands::get_singleton()->audio_graph_set_control_port_visibility( audio_graph, audio_graph->get_node_index( p_data.audio_node), p_data.flow, p_data.port_idx, p_data.item->is_checked(0) );

}
void ControlPortVisibilityEditor::edit(AudioNode *p_node) {

	
	tree->clear();

	GUI::TreeItem * root = tree->create_item(NULL); // root
	
	GUI::TreeItem *in=NULL;
	if (p_node->get_port_count( AudioNode::PORT_CONTROL, AudioNode::PORT_IN )) {
		
		in = tree->create_item( root );
		in->set_text(0,"Input Controls");
	}
		

	GUI::TreeItem *out=NULL;
	if (p_node->get_port_count( AudioNode::PORT_CONTROL, AudioNode::PORT_OUT )) {
	
		out = tree->create_item( root );
		out->set_text(0,"Output Controls");
	}
		
	AudioNode::PortFlow io_types[2]={AudioNode::PORT_IN,AudioNode::PORT_OUT};
		
	for (int io=0;io<2;io++) {
		
		for (int i=0;i<p_node->get_port_count( AudioNode::PORT_CONTROL, io_types[io] );i++) {
		
			ControlPort *control = p_node->get_control_port( io_types[io], i );
			ERR_CONTINUE( !control );
		
			GUI::TreeItem *port = tree->create_item( io_types[io]==AudioNode::PORT_IN?in:out );
			
			port->set_cell_mode( 0, GUI::CELL_MODE_CHECK );
			port->set_checked( 0, control->is_visible() );
			port->set_text( 0, control->get_name() );
			port->set_editable( 0, true );
						
			BindData bdata;
			bdata.audio_node=p_node;
			bdata.flow=io_types[io];
			bdata.item=port;
			bdata.control_port=control;
			bdata.port_idx=i;
			
			port->edited_signal.connect( GUI::Method1<int>( GUI::Method2<int,BindData>( this, &ControlPortVisibilityEditor::changed_callback), bdata) );
		
		}
	}
	tree->set_hide_root( true );
	show();
}


ControlPortVisibilityEditor::ControlPortVisibilityEditor(AudioGraph *p_graph,GUI::Window *p_parent) : GUI::Window(p_parent,GUI::Window::MODE_POPUP,GUI::Window::SIZE_TOPLEVEL_CENTER) {

	audio_graph=p_graph;
	
	GUI::WindowBox *main_vb = new GUI::WindowBox("Control Port Visibility Editor");
	
	set_root_frame( main_vb );
	
	GUI::ScrollBox *scroll = main_vb->add( new GUI::ScrollBox,1 );
	
	
	tree = scroll->set( new GUI::Tree(1) );
	
	scroll->set_expand_h(true);
	scroll->set_expand_v(true);
	scroll->set_scroll_h(true);
	scroll->set_scroll_v(true);
	
	
}


ControlPortVisibilityEditor::~ControlPortVisibilityEditor()
{
}


