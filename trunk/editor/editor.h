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
#include <list>
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/



class Editor {
public:

	enum TrackEditMode {
	
		EDIT_MODE_FRONT,
		EDIT_MODE_BACK
	};
	
	enum PasteMode {
	
		PASTE_OVERWRITE,
		PASTE_MIX,
		PASTE_INSERT
	};
	
	enum SelectionCommand {
	
		SELECTION_SCALE_VOLUMES,
		SELECTION_APPLY_VOLUME_MASK,
		SELECTION_QUANTIZE_UP,
		SELECTION_QUANTIZE_NEAREST,
		SELECTION_QUANTIZE_DOWN,
		SELECTION_TRANSPOSE_UP,
		SELECTION_TRANSPOSE_UP_OCTAVE,
		SELECTION_TRANSPOSE_DOWN,
		SELECTION_TRANSPOSE_DOWN_OCTAVE,	
	};
public:

	struct NoteListElement {
	
		PatternTrack::Position pos;
		PatternTrack::Note note;
	};

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
	
	struct Clipboard {
	
		struct TrackData {
			virtual ~TrackData() {};
		};
		
		struct PatternTrackData : public TrackData {
		
			std::list<NoteListElement> notes;
		};
	
		std::vector<TrackData*> tracks;
		bool active;
		Tick length;
		
		void clear() { for(int i=0;i<tracks.size();i++) delete tracks[i]; tracks.clear(); active=false; length=0; }
		
		Clipboard() { clear(); }
	} clipboard;
	
	
	Track *get_current_track() const;
	
	TrackEditMode track_edit_mode;
		
	void pattern_track_set_note( int p_track, const PatternTrack::Position& p_pos, const PatternTrack::Note& p_note );
	void pattern_set_note_at_cursor( PatternTrack::Note p_note);
	PatternTrack::Note pattern_get_note_at_cursor() const;
		
	bool pattern_editor_keypress( unsigned int p_code ); // return true if event must be handled
	void cursor_move_track_left();
	void cursor_move_track_right();
	void adjust_window_offset();
	
	void insert_at_pos(int p_track,int p_col, Tick p_tick,int p_rows=1);
	void delete_at_pos(int p_track,int p_col, Tick p_tick);
	
	void selection_verify();
	void shift_selection_check_begin(unsigned int &p_code);
	void shift_selection_check_end();
	
	void clipboard_zap_area(const Selection& p_selection);
public:

	Song *get_song() const { return song; }

	/* insertion */

	void set_volume_mask_active(bool p_active);
	bool is_volume_mask_active() const;
	void set_volume_mask(int p_volume_mask);
	int get_volume_mask() const;

	/* cursor  */
	
	void set_octave(int p_octave);
	int get_octave() const;
	
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
	void delete_block_at_cursor();
	void toggle_block_repeat_at_cursor();
	void resize_block_at_cursor();
	/* selection */
	
	
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

	void selection_command(SelectionCommand p_command,int p_param=0);
	void selection_create_blocks();
	void selection_make_loop();
	/* clipboard */
	
	void clipboard_copy();
	void clipboard_cut();
	void clipboard_paste(PasteMode p_mode);
	

	/* pattern editing */
	
	bool track_editor_keypress( unsigned int p_code ); // return true if event must be handled
	
	void set_loop_begin_at_cursor();
	void set_loop_end_at_cursor();
	
	void set_bar_len_at_beat(int p_beat,int p_len);
	void set_marker_at_beat(int p_beat,String p_marker);
	
	static Editor *get_singleton();


	Editor(Song *p_song);
	
	~Editor();

};

#endif
