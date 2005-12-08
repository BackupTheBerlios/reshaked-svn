//
// C++ Interface: editor
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDEDITOR_H
#define RESHAKEDEDITOR_H



#include "editor/editor_data.h"
#include "editor/editor_commands.h"
namespace ReShaked {

/**
	@author red <red@killy>
*/
class Editor {

	
	
	EditorData *d;
	EditorCommands commands;
	
	void enter_blocklist(EditorData::EnterBLDir p_dir);
	bool handle_navigation_key_press(BlockList *p_blocklist,int p_event);
	
friend class EditorCommands;	
	
	void revalidate_cursor();
public:
	
	Cursor &get_cursor();
	/** PATTERN EDIT */
	
	void set_pattern_note_edit_mode(EditorData::PatternNoteEditMode p_mode);
	EditorData::PatternNoteEditMode get_pattern_note_edit_mode();
	
	void set_pattern_edit_column(int p_column);
	int get_pattern_edit_column();
	
	void set_pattern_edit_field(int p_field);
	int get_pattern_edit_field();
	
	bool pattern_edit_key_press(int p_key_value); ///< return true if it must repaint / grab event
	bool automation_edit_key_press(int p_key_value); ///< return true if it must repaint / grab event
		
	
	/* global abstraction to a blocklist array */
	int get_track_blocklist(int p_track);
	int get_blocklist_track(int p_blocklist);
	int get_current_blocklist();
	void set_current_blocklist(int p_which);
	int get_current_track();
	int find_Track(Track *p_track);
	void set_track_name(int p_track,String p_name);
	
	void move_track_left(int p_which);
	void move_track_right(int p_which);
	
	int get_blocklist_count();
	BlockList* get_blocklist(int p_idx);
		
	void show_automation(String p_path);
	void hide_automation(String p_path);
	
	void add_automation_point(Automation *p_automation,Tick p_tick, float p_val);
	void move_automation_point(Automation *p_automation,int p_block, int p_point, Tick p_to_tick, float p_to_val);
	void remove_automation_point(Automation *p_automation,int p_block,int p_point);
	
	Song* get_song();
	
	void track_pattern_add_column(int p_which=-1);
	void track_pattern_remove_column(int p_which=-1);
	
	void add_track(TrackType p_type,int p_channels,String p_name);
	void remove_track(int p_which);
	
	void set_snap(int p_new_snap);
	void set_editing_octave(int p_octave);
	int get_editing_octave();
	void get_blocklists_sharing_block(BlockList::Block * p_block, std::list<int> *p_blocklist);
	void update_blocklists_sharing_block(BlockList::Block * p_block);
	
	Tick get_song_max_len();
	Tick get_block_list_max_len(BlockList *p_bl);
	
	void undo();
	void redo();
	
	Editor(Song *p_song,UI_UpdateNotify *p_ui_update_notify);
	
	~Editor();

};

}

#endif
