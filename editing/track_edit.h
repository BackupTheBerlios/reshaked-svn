#ifndef TRACK_EDIT_H
#define TRACK_EDIT_H

#include "editing/undo_stream.h"
#include "editing/cursor.h"
#include "editing/edit_commands.h"
#include "engine/track.h"

namespace ReShaked {

class TrackEdit : public UndoRedoOwner {

	Cursor *cursor;


public:

	Cursor &get_cursor();
	virtual bool receives_edit_command_type(EditCommand::Type p_type)=0;

	virtual void edit_command(const EditCommand &p_command)=0;
	virtual Track* get_track()=0;

	TrackEdit(UndoStream *p_undo_stream, Cursor *p_cursor);

};


}; //end of namespace
#endif
