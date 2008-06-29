//
// C++ Implementation: edit_commands
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "edit_commands.h"

EditCommands *EditCommands::singleton=NULL;


EditCommands *EditCommands::get_singleton() {

	return singleton;

}


void EditCommands::_audio_graph_connect_helper(AudioGraph *p_graph,AudioConnection p_connection) {

	p_graph->connect( p_connection );
	UpdateNotify::get_singleton()->audio_graph_changed();

}
void EditCommands::_audio_graph_disconnect_helper(AudioGraph *p_graph,AudioConnection p_connection) {

	p_graph->disconnect( p_connection );
	UpdateNotify::get_singleton()->audio_graph_changed();

}

void EditCommands::audio_graph_connect(AudioGraph *p_graph,const AudioConnection& p_connection) {

	ERR_FAIL_COND(!p_graph->can_connect(p_connection));
	
	AudioConnection c=p_connection;
	
	CommandBase *cmd_do=command( this, &EditCommands::_audio_graph_connect_helper, p_graph,c ); 
	CommandBase *cmd_undo=command( this, &EditCommands::_audio_graph_disconnect_helper, p_graph,c ); 

	add_action( "Audio Graph - Connect",cmd_do, cmd_undo );
}
void EditCommands::audio_graph_disconnect(AudioGraph *p_graph,const AudioConnection& p_connection) {

	ERR_FAIL_COND(!p_graph->has_connection(p_connection));
	
	AudioConnection c=p_connection;
	
	CommandBase *cmd_do=command( this, &EditCommands::_audio_graph_disconnect_helper, p_graph,c ); 
	CommandBase *cmd_undo=command( this, &EditCommands::_audio_graph_connect_helper, p_graph,c ); 

	add_action( "Audio Graph - Disconnect",cmd_do, cmd_undo );
}
void EditCommands::audio_graph_disconnect_port(AudioGraph *p_graph,int p_node_idx,AudioNode::PortType p_type, AudioNode::PortFlow p_flow, int p_port) {

	std::list<AudioConnection> conns;
		
	for (int i=0;i<p_graph->get_connection_count();i++) {
	
		AudioConnection ac=*p_graph->get_connection(i);
		
		if (ac.type!=p_type)
			continue;

		if (p_flow==AudioNode::PORT_IN) {
		
			if (ac.to_node==p_node_idx && ac.to_port==p_port)
				conns.push_back(ac);
		} else {
		
			if (ac.from_node==p_node_idx && ac.from_port==p_port)
				conns.push_back(ac);
		
		}
	
	}
	
	if (conns.empty())
		return;
		
	begin_group("Disconnect All");
	
	for( std::list<AudioConnection>::iterator I = conns.begin() ; I != conns.end() ; I++) {
	
		audio_graph_disconnect( p_graph, *I );
	}
	
	end_group();
}
void EditCommands::audio_graph_disconnect_node(AudioGraph *p_graph,int p_node_idx) {


	std::list<AudioConnection> connections;
	
	for (int i=0;i<p_graph->get_connection_count();i++) {
	
		AudioConnection c=*p_graph->get_connection(i);
		
		if (c.from_node==p_node_idx || c.to_node==p_node_idx) {
		
			connections.push_back(c);
		}
	}
	
	begin_group("Audio Graph - Clear Node Connections");
	
	for(std::list<AudioConnection>::iterator I=connections.begin();I!=connections.end();I++) {
	
		audio_graph_disconnect(p_graph,*I);
	}
	
	end_group();
}

void EditCommands::_audio_graph_move_node_helper(AudioGraph *p_graph,int p_node_idx,int p_x,int p_y) {

	ERR_FAIL_INDEX(p_node_idx,p_graph->get_node_count());
	
	AudioNode *n = p_graph->get_node(p_node_idx);
	ERR_FAIL_COND(!n);
	
	n->set_x(p_x);
	n->set_y(p_y);

	UpdateNotify::get_singleton()->audio_graph_changed();

}

void EditCommands::audio_graph_move_node(AudioGraph *p_graph,int p_node_idx,int p_x,int p_y) {

	ERR_FAIL_INDEX(p_node_idx,p_graph->get_node_count());
	
	AudioNode *n = p_graph->get_node(p_node_idx);
	ERR_FAIL_COND(!n);

	CommandBase *cmd_do=command( this, &EditCommands::_audio_graph_move_node_helper, p_graph, p_node_idx, p_x,p_y ); 
	CommandBase *cmd_undo=command( this, &EditCommands::_audio_graph_move_node_helper, p_graph, p_node_idx, n->get_x(),n->get_y() ); 

	add_action( "Audio Graph - Move Node",cmd_do, cmd_undo );
}

void EditCommands::_audio_graph_swap_nodes_helper(AudioGraph *p_graph,int p_node_idx,int p_with_node_idx) {

	p_graph->swap_node_indices(p_node_idx,p_with_node_idx);
	UpdateNotify::get_singleton()->audio_graph_changed();
	
}
void EditCommands::audio_graph_swap_nodes(AudioGraph *p_graph,int p_node_idx,int p_with_node_idx) {

	ERR_FAIL_INDEX(p_node_idx,p_graph->get_node_count());
	ERR_FAIL_INDEX(p_with_node_idx,p_graph->get_node_count());
	if (p_node_idx==p_with_node_idx)
		return;

	CommandBase *cmd_do=command(this,&EditCommands::_audio_graph_swap_nodes_helper,p_graph,p_node_idx,p_with_node_idx ); 
	CommandBase *cmd_undo=command(this,&EditCommands::_audio_graph_swap_nodes_helper,p_graph,p_with_node_idx,p_node_idx ); 

	add_action( "Audio Graph - Swap Nodes",cmd_do, cmd_undo );
}


void EditCommands::_audio_graph_add_node_helper(AudioGraph* p_graph, AudioNode* p_node,int p_at_pos) {


} 
	
void EditCommands::_audio_graph_remove_node_helper(AudioGraph* p_graph, int p_node_idx) {



}
	
void EditCommands::audio_graph_add_node(AudioGraph *p_graph,AudioNode *p_node) {
	
	
	int at_pos = p_graph->get_node_count();
	CommandBase *cmd_do=command( this, &EditCommands::_audio_graph_add_node_helper, p_graph, p_node, at_pos ); 
	CommandBase *cmd_undo=command( this, &EditCommands::_audio_graph_remove_node_helper, p_graph, at_pos ); 
	
	cmd_do->add_data(p_node);
	
	add_action("Audio Graph - Add Node: '"+p_node->get_info()->caption+"'",cmd_do,cmd_undo);
}
void EditCommands::audio_graph_remove_node(AudioGraph *p_graph,AudioNode *p_node,int p_node_idx) {

	
	
	AudioNode * node = p_graph->get_node(p_node_idx);
	ERR_FAIL_COND( !node );
	
	begin_group("Audio Graph - Remove Node: '"+node->get_info()->caption+"'");
	{		
		audio_graph_disconnect_node(p_graph,p_node_idx);		
			
		CommandBase *cmd_do=command( this, &EditCommands::_audio_graph_remove_node_helper, p_graph, p_node_idx ); 
		CommandBase *cmd_undo=command( this, &EditCommands::_audio_graph_add_node_helper, p_graph, node, p_node_idx ); 
			
		cmd_undo->add_data(node);
		
		add_action("take_node",cmd_do,cmd_undo);
	}
	end_group();
	

}
	

void EditCommands::audio_node_set_name(AudioNode *p_node, String p_name) {


}


EditCommands::EditCommands()
{
}


EditCommands::~EditCommands()
{
}


