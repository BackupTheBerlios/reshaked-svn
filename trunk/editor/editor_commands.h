//
// C++ Interface: editor_commands
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDEDITOR_COMMANDS_H
#define RESHAKEDEDITOR_COMMANDS_H

#include "editor/editor_data.h"
#include "engine/track_pattern.h"

namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class Editor;
			       
class EditorCommands {
friend class Editor;

	EditorData *d;
	Editor *editor;
	

	
	CommandFunc* add_track(bool p_no_undo,Track *p_track,int p_at_pos,std::list<AudioGraph::Connection> *p_node_connections=NULL);
	CommandFunc* remove_track(bool p_no_undo,int p_which);
	
	CommandFunc* track_move_left(bool p_no_undo,int p_which);
	CommandFunc* track_move_right(bool p_no_undo,int p_which);
	
	CommandFunc* track_swap_properties(bool p_no_undo,Track *p_track,int p_which,int p_with_which);
	
	
	CommandFunc* automation_show(bool p_no_undo,Track *p_track,int p_property);
	CommandFunc* automation_hide(bool p_no_undo,Track *p_track,int p_property);
	
	CommandFunc* add_automation_point(bool p_no_undo,Automation *p_automation,Tick p_tick, float p_val, float p_lfo_depth);
	CommandFunc* move_automation_point(bool p_no_undo,Automation *p_automation,int p_block, int p_point, Tick p_to_tick, Automation::AutomationValue p_to_val);
	CommandFunc* remove_automation_point(bool p_no_undo,Automation *p_automation,int p_block,int p_point);
	
	CommandFunc* automation_set_lfo(bool p_no_undo,Automation *p_automation,int p_block,LFO p_lfo);
	CommandFunc* automation_set_interpolation(bool p_no_undo,Automation *p_automation,int p_block,Automation::BlockInterpolationMethod p_int);
	
	CommandFunc* automation_set_follow_swing(bool p_no_undo,Automation *p_automation, bool p_follow);
	
	
	CommandFunc* blocklist_create_block(bool p_no_undo,BlockList *p_blocklist,BlockList::Block *p_block,Tick p_pos);
	CommandFunc* blocklist_delete_block(bool p_no_undo,BlockList *p_blocklist,int p_which);
	
	CommandFunc* blocklist_insert_block(bool p_no_undo,BlockList *p_blocklist,BlockList::Block *p_block,Tick p_pos);
	CommandFunc* blocklist_remove_block(bool p_no_undo,BlockList *p_blocklist,int p_which);
	
	CommandFunc* blocklist_set_block_length(bool p_no_undo,BlockList *p_blocklist,int p_which,Tick p_length);
	
	
	CommandFunc* set_note(bool p_no_undo,Track_Pattern *p_pattern,Track_Pattern::Position,Track_Pattern::Note);

	CommandFunc* track_pattern_add_column(bool p_no_undo,Track_Pattern *p_pattern);
	CommandFunc* track_pattern_remove_column(bool p_no_undo,Track_Pattern *p_pattern);
	
	CommandFunc* connection_create(bool p_no_undo,AudioGraph *p_graph, int p_node_from,int p_plug_from, int p_node_to, int p_plug_to);
	CommandFunc* connection_erase(bool p_no_undo,AudioGraph *p_graph, int p_node_from,int p_plug_from, int p_node_to, int p_plug_to);
	
	CommandFunc* track_plugin_add(bool p_no_undo,Track *p_track,Track::PluginInsertData p_data);
	CommandFunc* track_plugin_remove(bool p_no_undo,Track *p_track,int p_which);
	
	CommandFunc* track_plugin_move_left(bool p_no_undo,Track *p_track,int p_which);
	CommandFunc* track_plugin_move_right(bool p_no_undo,Track *p_track,int p_which);
	
	CommandFunc* track_mute(bool p_no_undo,Track *p_track,bool p_mute);
		
	CommandFunc* plugin_set_skip(bool p_no_undo,SoundPlugin *p_plugin,bool p_skip);
	CommandFunc* plugin_set_name(bool p_no_undo,SoundPlugin *p_plugin,String p_name);
	
	CommandFunc* automation_initial_value_changed(bool p_no_undo,Automation *p_auto,double p_to_val);
	CommandFunc* property_value_changed(bool p_no_undo,Property *p_property,double p_old_val,double p_to_val);

	CommandFunc* bar_length_add(bool p_no_undo,int p_at_beat,int p_len);
	CommandFunc* bar_length_remove(bool p_no_undo,int p_at_beat);

	CommandFunc* plugin_load_preset(bool p_no_undo,SoundPlugin *p_plugin, TreeLoader *p_old_preset, TreeLoader *p_new_preset,Track *p_track);
	
};

}

#endif
