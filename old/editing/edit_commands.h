
#ifndef EDIT_COMMANDS_H
#define EDIT_COMMANDS_H

struct EditCommand {
	enum Type {
		CURSOR_COMMAND,
		PATTERN_EDIT_COMMAND,
		NOTE_COMMAND,
		AUTOMATION_EDIT_COMMAND,
		AUDIO_EDIT_COMMAND,
		NUMBER_INPUT_COMMAND,
	};

	Type type;

	virtual ~EditCommand() {};
};


struct CursorCommand : public EditCommand {



	enum Command {

		CMD_MOVE_UP,
		CMD_MOVE_DOWN,
		CMD_MOVE_LEFT,
		CMD_MOVE_RIGHT,
		CMD_MOVE_PAGE_UP,
		CMD_MOVE_PAGE_DOWN,
		CMD_NEXT_PATTERN,
		CMD_PREV_PATTERN,
		CMD_NEXT_COLUMN,
		CMD_PREV_COLUMN,
		CMD_HOME,
		CMD_END,
		CMD_INSERT,
		CMD_DELETE,
		CMD_CLEAR_ROW,
	};

	Command command;
	bool selection_mode;
	
	CursorCommand(Command p_command=CMD_MOVE_UP,bool p_selection=false) {
		command=p_command;
		selection_mode=p_selection;
		type=CURSOR_COMMAND;
	}
};

struct PatternEditCommand : public EditCommand {

	enum Command {

		CMD_RAISE_NOTES,
		CMD_LOWER_NOTES,
		CMD_RAISE_NOTES_OCTAVE,
		CMD_LOWER_NOTES_OCTAVE,
		CMD_AMPLIFY,
		CMD_SET_VOLUME,
		CMD_RAMP_VOLUME,
		CMD_ZAP,
		CMD_EXPAND_X2,
		CMD_SHRINK_X2,
		CMD_QUANTIZE,

	};

	Command command;

	PatternEditCommand() { type=PATTERN_EDIT_COMMAND;}
};

struct NoteCommand : public EditCommand {

	Note note;
	bool off;
	NoteCommand() { type=NOTE_COMMAND; }
};

struct NumberInputCommand : public EditCommand {

	int number;

	NumberInputCommand() { number=NUMBER_INPUT_COMMAND; }
};

#endif

