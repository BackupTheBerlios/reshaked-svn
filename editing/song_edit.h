#ifndef SONG_EDIT_H
#define SONG_EDIT_H

#include "engine/song.h"
#include "editing/undo_stream.h"
#include "editing/cursor.h"
#include "editing/track_edit.h"
#include "editing/keyboard_input.h"
namespace ReShaked {

class SongEdit : public UndoRedoOwner {


	struct UndoRedoTrackAdd : public UndoRedoOp {

	        Track * track;
		TrackEdit *track_edit;

		UndoRedoTrackAdd(UndoRedoOwner *p_owner) : UndoRedoOp(p_owner) {};
	};
	
	Song *song;
	Cursor *cursor;
	int editing_octave;
	
	std::vector<TrackEdit*> track_editors;

 	void undo(UndoRedoOp *p_item);
	void redo(UndoRedoOp *p_item);
public:

	Cursor& get_cursor();

	
	int get_track_count();
	TrackEdit* get_track_edit(int p_index);
	void add_audio_track(int p_channels);
	void add_pattern_track(int p_channels);
	void add_bus_track(int p_channels);

	void cursor_move_up();
	void cursor_move_down();
	void cursor_move_page_up();
	void cursor_move_page_down();
	void cursor_move_pattern_up();
	void cursor_move_pattern_down();
	void cursor_move_home();
	void cursor_move_end();
	void cursor_set_pos(int p_row);
	
	void track_select(int p_index);

	void move_editing_left();
	void move_editing_right();

	int get_editing_octave();
	
	SongEdit(Cursor* p_cursor,UndoStream *p_undo_stream,Song *p_song);

};

};

#endif
