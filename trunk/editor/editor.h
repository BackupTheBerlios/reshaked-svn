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
	
	struct Cursor {
	
		Tick tick;
		int zoom_divisor; // tick divisor for zoom
		int track;
		int col;
		int field;
		int window_offset;
		int window_rows;
	} cursor;
	
	
	struct Selection {
	
		struct P {
		
			int track,col,row;
		} begin,end;
		bool active;
	} selection;
	
	Track *get_current_track() const;
	
	TrackEditMode track_edit_mode;
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
	
	Tick get_cursor_tick() const;
	int get_cursor_track() const;
	int get_cursor_col() const;
	int get_cursor_field() const;
	int get_cursor_zoom_divisor() const;
	int get_window_offset() const;
	int get_window_rows() const;
	
	int64 get_cursor_row() const;
	
	int64 get_ticks_per_row() const;
	int64 get_row_ticks(int p_row) const;
	int64 get_window_offset_ticks() const;
	
	void set_track_edit_mode( TrackEditMode p_track_edit_mode );
	TrackEditMode get_track_edit_mode() const;


	/**/
	
	bool is_selection_active() const;
	int get_selection_begin_track() const;
	int get_selection_begin_column() const;
	int get_selection_begin_row() const;
	int get_selection_end_track() const;
	int get_selection_end_column() const;
	int get_selection_end_row() const;

	static Editor *get_singleton();

	Editor(Song *p_song);
	
	~Editor();

};

#endif
