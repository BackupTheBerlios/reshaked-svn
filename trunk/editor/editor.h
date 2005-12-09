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
	
	void begin_check_shift_selection();
	void end_check_shift_selection();
	bool test_key_press_selecting(int &p_keypress);
	void fix_selection();
	
friend class EditorCommands;	
	
	void revalidate_cursor();
	EditorData::Selection::Pos get_cursor_selection_pos();
	
public:
	
	
	/** PATTERN EDIT (editor_pattern.cpp) */
	
	void set_pattern_note_edit_mode(EditorData::PatternNoteEditMode p_mode);
	EditorData::PatternNoteEditMode get_pattern_note_edit_mode();
	
	void set_pattern_edit_column(int p_column);
	int get_pattern_edit_column();
	
	void set_pattern_edit_field(int p_field);
	int get_pattern_edit_field();
	
	void track_pattern_add_column(int p_which=-1);
	void track_pattern_remove_column(int p_which=-1);
	
	bool pattern_edit_key_press(int p_key_value); ///< return true if it must repaint / grab event
	 
	/** AUTOMATION EDIT (editor_automation.cpp) */		 
			 
	void show_automation(String p_path);
	void hide_automation(String p_path);
	
	void add_automation_point(Automation *p_automation,Tick p_tick, float p_val);
	void move_automation_point(Automation *p_automation,int p_block, int p_point, Tick p_to_tick, float p_to_val);
	void remove_automation_point(Automation *p_automation,int p_block,int p_point);
	
			 
	bool automation_edit_key_press(int p_key_value); ///< return true if it must repaint / grab event
		
	
	/** BLOCK LIST ACCESS ABSTRACTION (editor_blocklist.cpp) */
	
	int get_track_blocklist(int p_track); ///< Get which blocklist idx is a track
	int get_blocklist_track(int p_blocklist); ///< Get which track contains a blocklist 
	
	int get_current_blocklist();
	void set_current_blocklist(int p_which);
	
	int get_current_track();
	int find_track_idx(Track *p_track); //

	int get_blocklist_count();
	BlockList* get_blocklist(int p_idx);
	int find_blocklist(BlockList *p_blocklist);
	
	/** BLOCKLIST EDITING (editor_blocklist.cpp) */
	
	bool blocklist_insert_block(BlockList *p_blocklist,BlockList::Block *p_block,Tick p_pos);
	void blocklist_remove_block(BlockList *p_blocklist,int p_which);
	void blocklist_resize_block(BlockList *p_blocklist,int p_which,Tick p_new_size);
	
	void get_blocklists_sharing_block(BlockList::Block * p_block, std::list<int> *p_blocklist);
	void update_blocklists_sharing_block(BlockList::Block * p_block);
	
	Tick get_block_list_max_len(BlockList *p_bl);
	
	/** TRACK EDITING (editor_track.cpp) */
	
	
	void set_track_name(int p_track,String p_name);
	
	void move_track_left(int p_which);
	void move_track_right(int p_which);
	
	void add_track(TrackType p_type,int p_channels,String p_name);
	void remove_track(int p_which);
	
	/** SELECTION EDITING */
	
	int get_selection_begin_column();
	int get_selection_begin_row();
	int get_selection_begin_blocklist();
	
	int get_selection_end_column();
	int get_selection_end_row();
	int get_selection_end_blocklist();
	
	bool is_selection_active();
	void disable_selection();
	
	/** CURSOR EDITING (editor.cpp) */	
	
	
	void set_snap(int p_new_snap);
	void set_editing_octave(int p_octave);
	int get_editing_octave();
	
	Tick get_song_max_len();
	
	/** UNDO SYSTEM (edit.cpp) */
	
	void begin_meta_undo_block(String p_name);
	void end_meta_undo_block();

	void lock_undo_stream();
	void unlock_undo_stream();
	
	void undo();
	void redo();
	
	/** INTERNAL DATA (editor.cpp) */
	
	Cursor &get_cursor();
	Song* get_song();
	UI_UpdateNotify *get_ui_update_notify();
	
	/** INITIALIZATION (editor.cpp) */

	Editor(Song *p_song,UI_UpdateNotify *p_ui_update_notify);
	~Editor();

};

}

#endif
