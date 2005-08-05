
#include "track_edit.h"

namespace ReShaked {



Cursor& TrackEdit::get_cursor() {

	return *cursor;
};

TrackEdit(UndoStream *p_undo_stream, Cursor *p_cursor) : UndoRedoOwner(p_undo_stream) {

	cursor=p_cursor;

}


};