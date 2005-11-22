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


#include "engine/song.h"
#include "engine/cursor.h"
namespace ReShaked {

/**
	@author red <red@killy>
*/
class Editor {
public:
	enum PatternNoteEditMode {
		MODE_NOTE,
		MODE_VOLUME,
	};	
private:
	
	
	enum EnterBLDir {
		ENTER_LEFT,
		ENTER_RIGHT
	};
	enum {
		
		MAX_FIELDS=2
	};
	
	
	struct PatternEdit {
		
		
		PatternNoteEditMode note_edit_mode;
		int column;
		int field;
		
	} pattern_edit;
	
	struct GlobalEdit {
		
		int current_blocklist;
		int editing_octave;
		
	} global_edit;
	
	Song *song;
	Cursor cursor;
	UI_UpdateNotify *ui_update_notify;
	
	void enter_blocklist(EnterBLDir p_dir);

public:
	
	Cursor &get_cursor();
	/** PATTERN EDIT */
	
	void set_pattern_note_edit_mode(PatternNoteEditMode p_mode);
	PatternNoteEditMode get_pattern_note_edit_mode();
	
	void set_pattern_edit_column(int p_column);
	int get_pattern_edit_column();
	
	void set_pattern_edit_field(int p_field);
	int get_pattern_edit_field();
	
	bool pattern_edit_key_press(int p_key_value); ///< return true if it must repaint
		
	
	/* global abstraction to a blocklist array */
	int get_track_blocklist(int p_track);
	int get_blocklist_track(int p_blocklist);
	int get_current_blocklist();
	void set_current_blocklist(int p_which);
	int get_current_track();
	
	int get_blocklist_count();
	BlockList* get_blocklist(int p_idx);
		
	void add_automation(String p_path);
	
	Song* get_song();
	
	void track_pattern_add_column();
	void track_pattern_remove_column();
	
	void add_track(TrackType p_type,int p_channels,String p_name);
	
	Editor(Song *p_song,UI_UpdateNotify *p_ui_update_notify);
	
	~Editor();

};

}

#endif
