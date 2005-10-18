#ifndef PATTERN_EDIT_H
#define PATTERN_EDIT_H


#include "editing/track_edit.h"
#include "engine/pattern_track.h"
#include <list>

namespace ReShaked {

class SongEdit;

class PatternEdit : public TrackEdit {
public:
                
	struct EdNote {
		PatternPosition pos;
		PatternNote note;
	};

	typedef std::list<EdNote> NoteList;

	enum NoteEditMode {
		MODE_NOTE,
		MODE_VOLUME,
		MODE_PORTA,
	};

private:

	struct UndoRedoNote : public UndoRedoOp {

		PatternNote old_note,new_note;
		PatternPosition pos;

		UndoRedoNote(PatternEdit *p_owner);
	};



	enum {

		MAX_FIELDS=2
	};

	SongEdit *song_edit;
	
	PatternTrack *pattern_track;

	NoteEditMode note_edit_mode;
	int column;
	int field;

	UndoRedoNote *get_undo_redo_current_note();


 	void undo(UndoRedoOp *p_item);
	void redo(UndoRedoOp *p_item);
	Track * get_track();

public:

	int get_columns();
	void set_columns(int p_columns);
	int get_cursor_column();
	int get_cursor_field();
	void set_cursor_column_and_field(int p_column,int p_field);

	bool key_press_event(int p_event);
	
	bool receives_edit_command_type(EditCommand::Type p_type);
	void edit_command(const EditCommand &p_command);
	NoteEditMode get_note_edit_mode();
	NoteList get_notes_in_row(int p_row);

	PatternEdit(UndoStream *p_undo_stream, Cursor *p_cursor,PatternTrack *p_track,SongEdit *p_song_edit);


};


}

#endif