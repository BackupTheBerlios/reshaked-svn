
#ifndef EDIT_COMMANDS_H
#define EDIT_COMMANDS_H

struct EditCommand {

	class Type {
		CURSOR_COMMAND,
		PATTERN_EDIT_COMMAND,
		NOTE_COMMAND
		AUTOMATION_EDIT_COMMAND,
		AUDIO_EDIT_COMMAND,
		NUMBER_INPUT_COMMAND,
	};

	Type type;

	~EditCommand() {};
};


struct CursorCommand : public EditCommand {



	enum Command {

		CMD_MOVE_UP,
		CMD_MOVE_DOWN,
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
		CMD_EXPAND_X2
		CMD_SHRINK_X2,
		CMD_QUANTIZE,

	};

	Command command;
}

struct NoteCommand : public EditCommand {

	Note note;
	bool off;
};

struct NumberInputCommand : public EditCommand {

	int number;
};

#endif

