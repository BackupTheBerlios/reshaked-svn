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
#include "editor.h"

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

	p_graph->add_node( p_node, p_at_pos );
	UpdateNotify::get_singleton()->audio_graph_changed();
} 
	
void EditCommands::_audio_graph_remove_node_helper(AudioGraph* p_graph, int p_node_idx) {

	p_graph->erase_node( p_node_idx );
	UpdateNotify::get_singleton()->audio_graph_changed();
}
	
void EditCommands::audio_graph_add_node(AudioGraph *p_graph,AudioNode *p_node) {
	
	begin_group("Audio Graph - Add Node: '"+p_node->get_info()->caption+"'");
	
	{ // add node to graph
		int at_pos = p_graph->get_node_count();		
		
		CommandBase *cmd_do=command( this, &EditCommands::_audio_graph_add_node_helper, p_graph, p_node, at_pos ); 
		CommandBase *cmd_undo=command( this, &EditCommands::_audio_graph_remove_node_helper, p_graph, at_pos ); 
		
		cmd_do->add_data(p_node);
		
		add_action("add_node",cmd_do,cmd_undo);
	}
	
	Track * t = dynamic_cast<Track*>(p_node);
	
	if (t) { // it's a track, must add into the song
	
		Song * s = Editor::get_singleton()->get_song();
		
		int new_track_pos = Editor::get_singleton()->get_cursor_track()+1;
		
		if (new_track_pos<0 || new_track_pos>=s->get_track_count())
			new_track_pos=s->get_track_count();
					
		CommandBase *cmd_do=command( this, &EditCommands::_song_add_track_helper, s,t,new_track_pos);
		CommandBase *cmd_undo=command( this, &EditCommands::_song_remove_track_helper,s,new_track_pos); 
				
		add_action("add_track",cmd_do,cmd_undo);
	
	}
	
}
void EditCommands::audio_graph_remove_node(AudioGraph *p_graph,int p_node_idx) {

	
	
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
	
	Track * t = dynamic_cast<Track*>(node);
	
	if (t) { // it's a track, must add into the song
	
		Song * s = Editor::get_singleton()->get_song();
		
		int old_track_pos = s->find_track_pos(t);
		
		if (old_track_pos>=0) {
						
			CommandBase *cmd_do=command( this, &EditCommands::_song_remove_track_helper,s,old_track_pos); 
			CommandBase *cmd_undo=command( this, &EditCommands::_song_add_track_helper, s,t,old_track_pos);
					
			add_action("take_track",cmd_do,cmd_undo);
		}
	
	}
	
	end_group();
	

}
	
void  EditCommands::_control_port_set_visibility_helper( ControlPort *p_port, bool p_visible) {

	p_port->set_visible( p_visible );
	UpdateNotify::get_singleton()->audio_graph_changed();	
}

void  EditCommands::audio_graph_set_control_port_visibility( AudioGraph *p_graph,int p_node_idx, AudioNode::PortFlow p_flow, int p_port, bool p_visible) {

	AudioNode *an = p_graph->get_node( p_node_idx );
	ERR_FAIL_COND (!an);
	ControlPort *cp = an->get_control_port( p_flow, p_port );	
	ERR_FAIL_COND(!cp);

	if (cp->is_visible()==p_visible)
		return; // don't bother

	begin_group("Control Port '"+cp->get_name()+"' - Toggle "+String( p_visible?"Visible":"Invisible") );

	if (!p_visible)
		audio_graph_disconnect_port(p_graph,p_node_idx,AudioNode::PORT_CONTROL,p_flow,p_port);
	
	CommandBase *cmd_do=command( this, &EditCommands::_control_port_set_visibility_helper, cp, p_visible);
	CommandBase *cmd_undo=command( this, &EditCommands::_control_port_set_visibility_helper, cp, !p_visible);
			
	add_action("set_node_visibility",cmd_do,cmd_undo);
	
	end_group();

}

void EditCommands::_audio_node_set_name_helper(AudioNode *p_node, String p_name) {

	p_node->set_name( p_name );
	UpdateNotify::get_singleton()->track_list_repaint();
}	

void EditCommands::audio_node_set_name(AudioNode *p_node, String p_name) {

	CommandBase *cmd_do=command( this, &EditCommands::_audio_node_set_name_helper, p_node, p_name);
	CommandBase *cmd_undo=command( this, &EditCommands::_audio_node_set_name_helper, p_node, p_node->get_name());
			
	add_action("AudioNode - Set Name",cmd_do,cmd_undo);

}
void EditCommands::_audio_node_set_layer_helper(AudioNode *p_node,int p_layer) {

	p_node->set_layer(p_layer);
	UpdateNotify::get_singleton()->audio_graph_changed();	
}

void EditCommands::audio_graph_set_node_layer(AudioGraph *p_graph,AudioNode *p_node,int p_layer) {

	if (p_layer==p_node->get_layer())
		return; // pointless

	int node_idx=p_graph->get_node_index(p_node);
	
	ERR_FAIL_COND(node_idx<0);

	begin_group("Audio Node - Change Layer");

	
	// all connections no longer valid that involve this node must be erased
	
	if (p_layer!=AudioNode::LAYER_ALWAYS_VISIBLE) {
		
		std::list<AudioConnection> conns;
			
		for (int i=0;i<p_graph->get_connection_count();i++) {
		
			AudioConnection ac=*p_graph->get_connection(i);
			
			if (ac.from_node!=node_idx && ac.to_node!=node_idx)
				continue; // doesn't invovle this node
			
			AudioNode *node_b=p_graph->get_node( ac.from_node==node_idx?ac.to_node:ac.from_node);
			
			
			if (node_b->get_layer()==AudioNode::LAYER_ALWAYS_VISIBLE)
				continue;
					
			if (node_b->get_layer() & p_layer)
				continue; // share soem bit, means still connected
	
			conns.push_back(ac);
		
		}
		
		for( std::list<AudioConnection>::iterator I = conns.begin() ; I != conns.end() ; I++) {
		
			audio_graph_disconnect( p_graph, *I );
		}
		
	}


	CommandBase *cmd_do=command( this, &EditCommands::_audio_node_set_layer_helper, p_node, p_layer );
	CommandBase *cmd_undo=command( this, &EditCommands::_audio_node_set_layer_helper, p_node, p_node->get_layer() );

	add_action("change_layer",cmd_do,cmd_undo);
	
	end_group();

}

/******* SONG *******/

void EditCommands::_song_add_track_helper(Song *p_song,Track *p_track,int p_at_index) {

	p_song->add_track(p_track,p_at_index);
	UpdateNotify::get_singleton()->track_list_changed();
	Editor::get_singleton()->validate_cursor_pos();
	
}
void EditCommands::_song_remove_track_helper(Song *p_song, int p_track) {

	p_song->remove_track(p_track);
	UpdateNotify::get_singleton()->track_list_changed();
	Editor::get_singleton()->validate_cursor_pos();
}


/****** SONG ******/

void EditCommands::_song_swap_tracks_helper(Song *p_song, int p_track, int p_with_track) {

	p_song->swap_tracks( p_track, p_with_track );
	
	UpdateNotify::get_singleton()->track_list_changed();
}

void EditCommands::song_swap_tracks(Song *p_song, int p_track, int p_with_track) {


	if (p_track==p_with_track)
		return;
		
	if (p_track<0 || p_track>=p_song->get_track_count())
		return;
		
	if (p_with_track<0 || p_with_track>=p_song->get_track_count())
		return;
		
	CommandBase *cmd_do=command( this, &EditCommands::_song_swap_tracks_helper, p_song, p_track, p_with_track );
	CommandBase *cmd_undo=command( this, &EditCommands::_song_swap_tracks_helper, p_song, p_with_track, p_track );

	add_action(String("Song - Swap Tracks"),cmd_do,cmd_undo);
		
		
}


void EditCommands::_song_change_initial_tempo_helper(Song *p_song, float p_bpm) {

	p_song->set_initial_tempo( p_bpm );
}


void EditCommands::song_change_initial_tempo(Song *p_song, float p_bpm) {

	CommandBase *cmd_do=command( this, &EditCommands::_song_change_initial_tempo_helper, p_song, p_bpm );
	CommandBase *cmd_undo=command( this, &EditCommands::_song_change_initial_tempo_helper, p_song, p_song->get_initial_tempo() );

	add_action(String("Song - Change Initial Tempo (BPM)"),cmd_do,cmd_undo);

	
}
/******* TRACK ********/

void EditCommands::_track_collapse_helper(Track *p_track,bool p_collapsed) {

	p_track->set_collapsed(p_collapsed);
	UpdateNotify::get_singleton()->track_list_changed();
	Editor::get_singleton()->validate_cursor_pos();
}


void EditCommands::track_collapse(Track *p_track,bool p_collapsed) {

	if (p_track->is_collapsed()==p_collapsed)
		return; // pointless

	CommandBase *cmd_do=command( this, &EditCommands::_track_collapse_helper, p_track, p_collapsed );
	CommandBase *cmd_undo=command( this, &EditCommands::_track_collapse_helper, p_track, !p_collapsed );

	add_action(String("Track - ")+(p_collapsed?"Collapse":"Expand"),cmd_do,cmd_undo);

}


void EditCommands::_track_block_toggle_repeat_helper( Track::Block *p_block, bool p_repeat ) {

	p_block->set_repeat_enabled( p_repeat );
	
	UpdateNotify::get_singleton()->track_block_changed( p_block );
}

void EditCommands::track_block_toggle_repeat( Track::Block *p_block, bool p_repeat ) {

	CommandBase *cmd_do=command( this, &EditCommands::_track_block_toggle_repeat_helper, p_block,p_repeat);
	CommandBase *cmd_undo=command( this, &EditCommands::_track_block_toggle_repeat_helper, p_block, p_block->is_repeat_enabled() );


	add_action(String("Track Block - ")+(p_repeat?"Enable Repeat":"Disable Repeat"),cmd_do,cmd_undo);
}

struct _BlockRef {

	Track::Block *block;
	
	_BlockRef(Track::Block *p_block) {
	
		block=p_block;
	}
	
	~_BlockRef() {
	
		if (block->get_refcount() == 0 ) // block is not in use
			delete block;
	}
};


void EditCommands::_track_add_block_helper(Track *p_track,Track::Block *p_block, Tick p_at_pos) {

	int pos = p_track->insert_block(p_block,p_at_pos);
	
	ERR_FAIL_COND(pos < 0 );
	
	UpdateNotify::get_singleton()->track_list_repaint();
}

void EditCommands::_track_remove_block_helper(Track *p_track,Track::Block *p_block) {

	int idx=-1;
	
	for (int i=0;i<p_track->get_block_count();i++) {
	
		if (p_track->get_block( i ) == p_block) {
		
			idx=i;
			break;
		}	
	}
	
	ERR_FAIL_COND( idx < 0 );
	
	p_track->remove_block( idx );
		
	UpdateNotify::get_singleton()->track_list_repaint();
}

void EditCommands::track_add_block(Track *p_track,Track::Block *p_block, Tick p_at_pos) {

	if (!p_track->block_fits(p_at_pos,p_block->get_length()))
		return;

	CommandBase *cmd_do=command( this, &EditCommands::_track_add_block_helper, p_track, p_block,p_at_pos);
	CommandBase *cmd_undo=command( this, &EditCommands::_track_remove_block_helper, p_track, p_block );
	
	cmd_do->add_data( new _BlockRef( p_block) );
	
	add_action("Track - Add Block",cmd_do,cmd_undo);
}


void EditCommands::track_remove_block(Track *p_track,Track::Block *p_block) {

	int idx=-1;
	
	for (int i=0;i<p_track->get_block_count();i++) {
	
		if (p_track->get_block( i ) == p_block) {
		
			idx=i;
			break;
		}	
	}
	
	ERR_FAIL_COND( idx < 0);
	
	Tick pos = p_track->get_block_pos( idx );

	CommandBase *cmd_do=command( this, &EditCommands::_track_remove_block_helper,p_track,  p_block );
	CommandBase *cmd_undo=command( this, &EditCommands::_track_add_block_helper, p_track, p_block,pos );
	
	cmd_undo->add_data( new _BlockRef( p_block) );
	
	add_action("Track - Remove Block",cmd_do,cmd_undo);
}


void EditCommands::_track_resize_block_helper(Track *p_track, int p_block_idx, Tick p_new_size) {

	ERR_FAIL_INDEX( p_block_idx, p_track->get_block_count() );
	
	p_track->get_block(p_block_idx)->set_length(p_new_size);
	
	UpdateNotify::get_singleton()->track_block_changed( p_track->get_block( p_block_idx ) );
	
}

void EditCommands::track_resize_block(Track *p_track, int p_block_idx, Tick p_new_size) {

	ERR_FAIL_INDEX( p_block_idx, p_track->get_block_count() );
	
	std::list<int> exclude_list;
	exclude_list.push_back(p_block_idx);
	
	if (!p_track->block_fits( p_track->get_block_pos( p_block_idx), p_new_size, exclude_list ))
		return; // doesn't fit, nothing to do
		
	Tick previous_len = p_track->get_block(p_block_idx)->get_length();
		
	begin_group("Track - Resize Block");
		
	if (previous_len > p_new_size ) {
		// must erase notes when shrinking
		PatternTrack *pt = dynamic_cast<PatternTrack*>( p_track );
		
		if (pt) {
			std::list<PatternTrack::Position> to_erase;
			PatternTrack::PatternBlock *block = dynamic_cast<PatternTrack::PatternBlock*>(p_track->get_block(p_block_idx));
				
			for (int j=0;j<block->get_note_count();j++) {
			
				PatternTrack::Position pos = block->get_note_pos( j );
				
				if (pos.tick<p_new_size)
					continue;
				
				pos.tick += pt->get_block_pos( p_block_idx );
				to_erase.push_back(pos);
			}
			
			// erase them
			std::list<PatternTrack::Position>::iterator I;
			
			for( I = to_erase.begin() ; I != to_erase.end() ; I++ ) {
			
				pattern_set_note( pt, I->column, I->tick , PatternTrack::Note() );
			}
		}
	}
		
	CommandBase *cmd_do=command( this, &EditCommands::_track_resize_block_helper,p_track,  p_block_idx, p_new_size );
	CommandBase *cmd_undo=command( this, &EditCommands::_track_resize_block_helper, p_track, p_block_idx, previous_len );
	
	add_action("track_remove_block",cmd_do,cmd_undo);	
		
	end_group();
}


/*******PATTERN*******/



void EditCommands::_pattern_set_note_helper( PatternTrack::PatternBlock *p_block, PatternTrack::Position p_pos, PatternTrack::Note p_note ) {

	p_block->set(p_pos,p_note);
	UpdateNotify::get_singleton()->track_block_changed( p_block );
}


void EditCommands::pattern_set_note( PatternTrack *p_pattern, int p_column, Tick p_tick, const PatternTrack::Note &p_note ) {


	int blk_idx=p_pattern->get_block_at_pos( p_tick );
	if (blk_idx<0)
		return;
	PatternTrack::PatternBlock *block = static_cast<PatternTrack::PatternBlock*>(p_pattern->get_block(blk_idx));
	if (!block)
		return; //pointless
		
	Tick tick = p_tick - p_pattern->get_block_pos(blk_idx);

	PatternTrack::Position pos( tick, p_column );
	PatternTrack::Note old_note = block->get( pos );
	if (p_note==old_note)
		return; // also pointless, will help the paste commands
		
	PatternTrack::Note note=p_note;
	CommandBase *cmd_do=command( this, &EditCommands::_pattern_set_note_helper, block, pos, note);
	CommandBase *cmd_undo=command( this, &EditCommands::_pattern_set_note_helper, block,pos, old_note );

	add_action("Pattern - Edit Note",cmd_do,cmd_undo);
		
}


void EditCommands::_pattern_set_visible_columns_helper( PatternTrack *p_pattern, int p_visible ) {

	p_pattern->set_visible_columns( p_visible );
	UpdateNotify::get_singleton()->track_changed( p_pattern );
}

void EditCommands::pattern_set_visible_columns( PatternTrack *p_pattern, int p_visible ) {

	if (p_visible<1)
		return;

	begin_group("Pattern - Set Columns");
	
	// find notes to erase
	std::list<PatternTrack::Position> to_erase;
		
	for (int i=0;i<p_pattern->get_block_count();i++) {
	
		PatternTrack::PatternBlock *block = dynamic_cast<PatternTrack::PatternBlock*>(p_pattern->get_block( i ));
		
		for (int j=0;j<block->get_note_count();j++) {
		
			PatternTrack::Position pos = block->get_note_pos( j );
			
			if (pos.column < p_visible)
				continue;
			
			pos.tick += p_pattern->get_block_pos( i );
			to_erase.push_back(pos);
		}
	}
	
	// erase them
	std::list<PatternTrack::Position>::iterator I;
	
	for( I = to_erase.begin() ; I != to_erase.end() ; I++ ) {
	
		pattern_set_note( p_pattern, I->column, I->tick , PatternTrack::Note() );
	}
	
	
	CommandBase *cmd_do=command( this, &EditCommands::_pattern_set_visible_columns_helper, p_pattern, p_visible);
	CommandBase *cmd_undo=command( this, &EditCommands::_pattern_set_visible_columns_helper, p_pattern, p_pattern->get_visible_columns());

	add_action("set_columns",cmd_do,cmd_undo);
	
	end_group();


}

void EditCommands::_barbeat_set_barlen_helper( Song *p_song, int p_beat,int p_barlen ) {

	p_song->get_bar_map().insert_bar_len_at_beat(p_beat,p_barlen);
	UpdateNotify::get_singleton()->track_list_repaint();
}

void EditCommands::barbeat_set_barlen( Song *p_song, int p_beat,int p_barlen ) {

	
	int previous=0;
	
	int idx = p_song->get_bar_map().get_bar_len_idx_from_beat(p_beat);
	
	if (idx>=0) {
	
		if (p_song->get_bar_map().get_bar_len_idx_pos(idx)==p_beat) {
		
			previous=p_song->get_bar_map().get_bar_len_idx_len(idx);
		}
	}

	CommandBase *cmd_do=command( this, &EditCommands::_barbeat_set_barlen_helper, p_song,p_beat,p_barlen);
	CommandBase *cmd_undo=command( this, &EditCommands::_barbeat_set_barlen_helper, p_song, p_beat, previous);

	add_action("Bar Length Map - Change at Beat ",cmd_do,cmd_undo);

}


void EditCommands::_marker_set_helper( Song *p_song, int p_beat, String p_marker ) {

	if (p_marker=="") {
	
		int exact=p_song->get_marker_map().find_exact(p_beat);
		if (exact!=-1) {
		
			p_song->get_marker_map().erase(exact);
		}
	} else {
	
		p_song->get_marker_map().insert(p_beat,p_marker);
	}
	
	UpdateNotify::get_singleton()->track_list_repaint();
}



void EditCommands::marker_set( Song *p_song, int p_beat, String p_marker ) {

	String previous="";
	
	int exact=p_song->get_marker_map().find_exact(p_beat);
	
	if (exact!=-1) {
		
		previous = p_song->get_marker_map()[exact];
	}
	 
	CommandBase *cmd_do=command( this, &EditCommands::_marker_set_helper, p_song,p_beat,p_marker);
	CommandBase *cmd_undo=command( this, &EditCommands::_marker_set_helper, p_song, p_beat, previous);

	add_action("Marker Map - Change Marker",cmd_do,cmd_undo);	 
	 
}

EditCommands::EditCommands()
{
}


EditCommands::~EditCommands()
{
}



