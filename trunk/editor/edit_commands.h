//
// C++ Interface: edit_commands
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef EDIT_COMMANDS_H
#define EDIT_COMMANDS_H


#include "engine/audio_graph.h"
#include "editor/update_notify.h"
#include "editor/undo_redo.h"
#include "editor/editor.h"
#include "engine/song.h"
#include "engine/pattern_track.h"

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class EditCommands : public UndoRedo {
	
	static EditCommands *singleton;
	
	void _audio_graph_connect_helper(AudioGraph *p_graph,AudioConnection p_connection);
	void _audio_graph_disconnect_helper(AudioGraph *p_graph,AudioConnection p_connection);
	void _audio_graph_move_node_helper(AudioGraph *p_graph,int p_node_idx,int p_x,int p_y);
	void _audio_graph_swap_nodes_helper(AudioGraph *p_graph,int p_node_idx,int p_with_node_idx);
	void _audio_graph_add_node_helper(AudioGraph* p_graph, AudioNode* p_node,int p_at_pos);
	void _audio_graph_remove_node_helper(AudioGraph* p_graph, int p_node_idx);
	void _control_port_set_visibility_helper( ControlPort *p_port, bool p_visible);
	void _audio_node_set_name_helper(AudioNode *p_node, String p_name);
		
	void _audio_node_set_layer_helper(AudioNode *p_node,int p_layer);
	
	void _song_add_track_helper(Song *p_song,Track *p_track,int p_at_index);
	void _song_remove_track_helper(Song *p_song, int p_track);
	void _track_collapse_helper(Track *p_track,bool p_collapsed);
	void _pattern_set_note_helper( PatternTrack::PatternBlock *p_block, PatternTrack::Position p_pos,PatternTrack::Note p_note );
	void _pattern_set_visible_columns_helper( PatternTrack *p_pattern, int p_visible );
	
	void _song_swap_tracks_helper(Song *p_song, int p_track, int p_with_track);
	void _song_change_initial_tempo_helper(Song *p_song, float p_bpm);
	
	void _track_block_toggle_repeat_helper( Track::Block *p_block, bool p_repeat );
	void _track_add_block_helper(Track *p_track,Track::Block *p_block, Tick p_at_pos);
	void _track_resize_block_helper(Track *p_track, int p_block_idx, Tick p_new_size);	
	void _track_remove_block_helper(Track *p_track,Track::Block *p_block);	
	
	void _barbeat_set_barlen_helper( Song *p_song, int p_beat,int p_barlen );
	void _marker_set_helper( Song *p_song, int p_beat, String p_marker );
	
public:

	static EditCommands *get_singleton();

	void audio_graph_connect(AudioGraph *p_graph,const AudioConnection& p_connection);	
	void audio_graph_disconnect(AudioGraph *p_graph,const AudioConnection& p_connection);
	void audio_graph_disconnect_port(AudioGraph *p_graph,int p_node_idx,AudioNode::PortType p_type, AudioNode::PortFlow p_flow, int p_port);
	void audio_graph_disconnect_node(AudioGraph *p_graph,int p_node_idx);
	
	void audio_graph_move_node(AudioGraph *p_graph,int p_node_idx,int p_x,int p_y);	
	void audio_graph_swap_nodes(AudioGraph *p_graph,int p_node_idx,int p_with_node_idx);	
	
	void audio_graph_add_node(AudioGraph *p_graph,AudioNode *p_node);
	void audio_graph_remove_node(AudioGraph *p_graph,int p_node_idx);
	
	void audio_graph_set_control_port_visibility( AudioGraph *p_graph,int p_node_idx, AudioNode::PortFlow p_flow, int p_port, bool p_visible);
	void audio_graph_set_node_layer(AudioGraph *p_graph,AudioNode *p_node,int p_layer);
		
	
	void audio_node_set_name(AudioNode *p_node, String p_name);

	void song_swap_tracks(Song *p_song, int p_track, int p_with_track);
	void song_change_initial_tempo(Song *p_song, float p_bpm);

	void track_collapse(Track *p_track,bool p_collapsed);
	void track_block_toggle_repeat( Track::Block *p_block, bool p_repeat );
	void track_add_block(Track *p_track,Track::Block *p_block, Tick p_at_pos);
	void track_resize_block(Track *p_track, int p_block_idx, Tick p_new_size);
	void track_remove_block(Track *p_track,Track::Block *p_block);
	
	
	void pattern_set_note( PatternTrack *p_pattern, int p_column, Tick p_tick, const PatternTrack::Note &p_note );
	void pattern_set_visible_columns( PatternTrack *p_pattern, int p_visible );

	void barbeat_set_barlen( Song *p_song, int p_beat,int p_barlen );
	void marker_set( Song *p_song, int p_beat, String p_marker );

	EditCommands();
	
	~EditCommands();

};

#endif
