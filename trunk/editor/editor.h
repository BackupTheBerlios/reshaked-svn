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



#include "editor/editor_commands.h"
#include "tree_loader_disk.h"
#include "engine/midi_driver.h"
namespace ReShaked {

/**
	@author red <red@killy>
*/
				    
class EditorData;
		    
class Editor {

	
	
	EditorData *d;
	EditorCommands commands;
	
	void enter_blocklist(EditorData::EnterBLDir p_dir);
	bool handle_navigation_key_press(BlockList *p_blocklist,int &p_event);
	
	void begin_check_shift_selection();
	void end_check_shift_selection();
	bool test_key_press_selecting(int &p_keypress);
	void fix_selection();
	bool selection_can_paste_at(EditorData::Selection::Pos p_pos);
	void selection_paste_at(EditorData::Selection::Pos p_pos);
	void selection_clear_area(EditorData::Selection::Pos p_from,EditorData::Selection::Pos p_to);
	EditorData::Selection::Pos get_selection_end_from_pos(EditorData::Selection::Pos p_pos);
	
	void play_note_at_cursor(int p_custom_column=-1);
	
	void selection_transpose(bool p_up);	
	
	int get_current_track_columns();
	
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
			 
	void show_automation(int p_property,Track *p_track=NULL);
	void hide_automation(int p_property,Track *p_track=NULL);
	
	void add_automation_point(Automation *p_automation,Tick p_tick, float p_val,float p_lfo_depth=0,bool p_collapsable=false);
	void move_automation_point(Automation *p_automation,int p_block, int p_point, Tick p_to_tick, float p_to_val,float p_to_lfo);
	void remove_automation_point(Automation *p_automation,int p_block,int p_point);
	
			 
	bool automation_edit_key_press(int p_key_value); ///< return true if it must repaint / grab event
	
	void automation_set_lfo(Automation *p_automation,int p_block,LFO p_lfo);
	void automation_set_interpolation(Automation *p_automation,int p_block,Automation::BlockInterpolationMethod p_int);
	
	void automation_set_follow_swing(Automation *p_auto,bool p_follow);
		
	
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
	
	/* USed when creating the block for the first time */
	bool blocklist_create_block(BlockList *p_blocklist,BlockList::Block *p_block,Tick p_pos);
	/* Used when effectively deletingt the block */
	void blocklist_delete_block(BlockList *p_blocklist,int p_which);
	
	/* USed for moving aroudn blocks */
	bool blocklist_insert_block(BlockList *p_blocklist,BlockList::Block *p_block,Tick p_pos);
	void blocklist_remove_block(BlockList *p_blocklist,int p_which);
	void blocklist_resize_block(BlockList *p_blocklist,int p_which,Tick p_new_size);
	
	void get_blocklists_sharing_block(BlockList::Block * p_block, std::list<int> *p_blocklist);
	void update_blocklists_sharing_block(BlockList::Block * p_block);
	
	void blocklist_block_set_repeat(BlockList::Block *p_block,bool p_repeat);
	
	Tick get_block_list_max_len(BlockList *p_bl);
	
	/** TRACK EDITING (editor_track.cpp) */
	
	void set_track_name(int p_track,String p_name);
	
	void move_track_left(int p_which);
	void move_track_right(int p_which);
	
	void add_track(TrackType p_type,int p_channels,String p_name);
	void remove_track(int p_which);
	
	AudioGraph::ConnectError connection_create(AudioGraph *p_graph, int p_node_from,int p_plug_from, int p_node_to, int p_plug_to);
	void connection_erase(AudioGraph *p_graph, int p_node_from,int p_plug_from, int p_node_to, int p_plug_to);
	
	void add_plugin_to_track(Track *p_track,SoundPlugin *p_plugin,bool p_append_to_end=true);
	void remove_plugin_from_track(Track *p_track,int p_which);
	
	void set_plugin_skips_processing(SoundPlugin *p_plugin, bool p_skips);
	void property_changed(Property * p_property,double p_old_value,Track *p_hint_track=NULL);
	void set_plugin_preset_name(SoundPlugin *p_plugin,String p_name);
	
	void track_move_plugin_left(Track *p_track,int p_plugin);
	void track_move_plugin_right(Track *p_track,int p_plugin);
	
	void set_track_mute(Track *p_track,bool p_mute);
	void set_track_solo(int p_track_idx);
	
	void track_automation_move_left(Track *p_track,int p_property);
	void track_automation_move_right(Track *p_track,int p_property);
	
	/** SELECTION EDITING */
	
	int get_selection_begin_column();
	int get_selection_begin_row();
	int get_selection_begin_blocklist();
	
	int get_selection_end_column();
	int get_selection_end_row();
	int get_selection_end_blocklist();
	
	bool selection_can_paste_at_cursor();
	bool is_selection_active();
	void disable_selection();
	
	void selection_begin();
	void selection_end();
	void selection_column_all();
	void selection_copy();
	void selection_zap();
	void selection_paste();
	void selection_paste_insert();
	void selection_paste_overwrite();
	void selection_paste_mix();
	
	void selection_mouse_drag_begin(int p_blocklist,int p_column,int p_row);
	void selection_mouse_drag_end(int p_blocklist,int p_column,int p_row);

	void selection_volume_scale(int p_percent);
	void selection_to_loop();
	void selection_create_block();
	void selection_set_volumes_to_mask();
	
	void selection_cursor_transpose_up();
	void selection_cursor_transpose_down();
	
	enum QuantizeType {
		QUANTIZE_UP,
		QUANTIZE_NEAREST,
		QUANTIZE_DOWN
	};
			
	void selection_quantize(QuantizeType p_type);
	
	/** CURSOR EDITING (editor.cpp) */	
	
	
	void cursor_set_step(int p_step);
	int cursor_get_step();
	void set_snap(int p_new_snap);
	void set_editing_octave(int p_octave);
	int get_editing_octave();
	
	Tick get_song_max_len();
	
	void marker_set(int p_beat,String p_text);
	void marker_remove(int p_beat);
	
	void bar_length_set(int p_at_beat,int p_len);
	void bar_length_remove(int p_at_beat);
	
	int get_volume_mask();
	void set_volume_mask(int p_val);
	bool is_volume_mask_active();
	void set_volume_mask_active(bool p_active);
	
	void set_loop_begin_at_cursor();
	void set_loop_end_at_cursor();
	
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
	void set_current_rack_track(int p_track);
	int get_current_rack_track();
	void set_pattern_midi_input_enabled(bool p_enabled);
	bool is_pattern_midi_input_enabled();
	
	/** MIDI */
	
	void midi_record_event(const DriverInputEvent& ev);
	void midi_edit_event(const EventMidi& ev);
	void midi_record_automation_event(Track *p_track,const DriverInputEvent& ev);
	void set_polyphonic_midi_input_enabled(bool p_enabled);	
	void midi_reset();
	
	/** PRESETS */
	
	void set_plugin_preset_file(SoundPlugin *p_plugin,String p_filename,bool p_reference);
			
	TreeLoaderDisk::ErrorReading load_plugin_preset(SoundPlugin *p_plugin,String p_filename,Track *p_track,String p_set_preset_name="",bool p_reference=false);
	TreeLoaderDisk::ErrorReading load_track_preset(Track *p_track,String p_filename);
	
	
	/** INITIALIZATION (editor.cpp) */

	
	void reset();
	Editor(Song *p_song,UI_UpdateNotify *p_ui_update_notify);
	~Editor();

};

}

#endif
