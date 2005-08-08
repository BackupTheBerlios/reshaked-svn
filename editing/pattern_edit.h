#ifndef PATTERN_EDIT_H
#define PATTERN_EDIT_H


#include "editing/track_edit.h"
#include "engine/pattern_track.h"

namespace ReShaked {


class PatternEdit : public TrackEdit {

	struct UndoRedoNote : public UndoRedoOp {

		PatternNote old_note,new_note;
		PatternPosition pos;

		UndoRedoNote(PatternEdit *p_owner);
	};

	
	enum NoteEditMode {
		MODE_NOTE,
		MODE_VOLUME,
		MODE_PORTA,
	};

	enum {

		MAX_SUBCOLUMNS=2
	};

	PatternTrack *pattern_track;
	
	NoteEditMode note_edit_mode;
	int subcolumn;

	UndoRedoNote *get_undo_redo_current_note();
	
	bool receives_edit_command_type(EditCommand::Type p_type);
	void edit_command(const EditCommand &p_command);

 	void undo(UndoRedoOp *p_item);
	void redo(UndoRedoOp *p_item);
	Track * get_track();

public:

	PatternEdit(UndoStream *p_undo_stream, Cursor *p_cursor,PatternTrack *p_track);


};


}

#endif