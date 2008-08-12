//
// C++ Interface: editor
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef EDITOR_H
#define EDITOR_H

#include "engine/song.h"
#include "engine/pattern_track.h"
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/



class Editor {
public:

	enum TrackEditMode {
	
		EDIT_MODE_FRONT,
		EDIT_MODE_BACK
	};
public:
	Song *song;

	static Editor *singleton;
	
	struct Insertion {
	
		int octave;
		bool volume_mask_active;
		int volume_mask;
	
	} insert;
	
	struct Cursor {
	
		Tick tick;
		int zoom_divisor; // tick divisor for zoom
		int track;
		int col;
		int field;
		int window_offset;
		int window_rows;
		int step;
	} cursor;
	
	
	struct Selection {
	
		struct {
		
			int track,col;
			Tick tick;
		} begin,end;
		bool active;
	} selection,shift_selection;
	
	Track *get_current_track() const;
	
	TrackEditMode track_edit_mode;
		
		
	void pattern_set_note_at_cursor( PatternTrack::Note p_note);
	PatternTrack::Note pattern_get_note_at_cursor() const;
		
	bool pattern_editor_keypress( unsigned int p_code ); // return true if event must be handled
	void cursor_move_track_left();
	void cursor_move_track_right();
	void adjust_window_offset();
	
	void selection_verify();
	void shift_selection_check_begin(unsigned int &p_code);
	void shift_selection_check_end();
public:

	Song *get_song() const { return song; }

	void set_cursor_tick(Tick p_tick);
	void set_cursor_track(int p_track);
	void set_cursor_col(int p_col);
	void set_cursor_field(int p_field);
	void set_cursor_zoom_divisor(int p_divisor);
	void set_window_offset(int p_row);
	void set_window_rows(int p_rows);
	void set_cursor_row(int p_row);
	void set_cursor_step(int p_step);
	
	Tick get_cursor_tick() const;
	int get_cursor_track() const;
	int get_cursor_col() const;
	int get_cursor_field() const;
	int get_cursor_zoom_divisor() const;
	int get_window_offset() const;
	int get_window_rows() const;
	int64 get_cursor_row() const;
	
	int get_cursor_step() const;
	
	int64 get_ticks_per_row() const;
	int64 get_row_ticks(int p_row) const;
	int64 get_window_offset_ticks() const;
	
	void set_track_edit_mode( TrackEditMode p_track_edit_mode );
	TrackEditMode get_track_edit_mode() const;

	void validate_cursor_pos();

	/**/
	
	
	void set_selection_begin_at_cursor();
	void set_selection_end_at_cursor();
	
	bool is_selection_active() const;
	int get_selection_begin_track() const;
	int get_selection_begin_column() const;
	int get_selection_begin_row() const;
	int get_selection_end_track() const;
	int get_selection_end_column() const;
	int get_selection_end_row() const;
	
	void set_selection_disabled();	
	
	void set_mouse_selection_begin(int p_track,int p_col, int p_row);
	void set_mouse_selection_end(int p_track,int p_col, int p_row);	
	
	void select_column_block_all();

	static Editor *get_singleton();

	/* pattern editing */
	
	bool track_editor_keypress( unsigned int p_code ); // return true if event must be handled
	


	Editor(Song *p_song);
	
	~Editor();

};

#endif
