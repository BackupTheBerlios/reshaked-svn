#ifndef TRACK_EDIT_H
#define TRACK_EDIT_H

namespace ReShaked {

class TrackEdit : public UndoRedoOwner {

	Cursor *cursor;
protected:

	Cursor &get_cursor();

public:

	virtual bool receives_edit_command_type(EditCommand::Type p_type)=0;

	virtual void edit_command(const EditCommand &p_command)=0;

	TrackEdit(UndoStream *p_undo_stream, Cursor *p_cursor);

};


}; //end of namespace
#endif