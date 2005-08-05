#ifndef PATTERN_EDIT_H
#define PATTERN_EDIT_H


namespace ReShaked {


class PatternEdit : public TrackEdit {

	enum NoteEditMode {
		MODE_NOTE,
		MODE_VOLUME,
		MODE_PORTA,
	};

	enum {

		MAX_SUBCOLUMNS=2
	};

	NoteEditMode note_edit_mode;
	int subcolumn;

	bool receives_edit_command_type(EditCommand::Type p_type);
	void edit_command(const EditCommand &p_command);

public:




};

#endif