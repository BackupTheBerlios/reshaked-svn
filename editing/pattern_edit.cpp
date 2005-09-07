#include "pattern_edit.h"
#include "song_edit.h"
#include "pianokeys.h"
#include "global_edit.h"

namespace ReShaked {


PatternEdit::UndoRedoNote::UndoRedoNote(PatternEdit *p_owner) : UndoRedoOp(p_owner) {



};


bool PatternEdit::receives_edit_command_type(EditCommand::Type p_type) {


    return true;
}

void PatternEdit::undo(UndoRedoOp *p_item) {



}
void PatternEdit::redo(UndoRedoOp *p_item) {



}

PatternEdit::NoteList PatternEdit::get_notes_in_row(int p_row) {

	NoteList notelist;
	PatternPosition from;
	PatternPosition to;
	from.tick=get_cursor().get_snapped_window_tick_pos(p_row);

	to.tick=get_cursor().get_snapped_window_tick_pos(p_row+1)-1;

	int idx_from=pattern_track->get_pattern().get_next_index(from);
	int idx_to=pattern_track->get_pattern().get_prev_index(to);
	//printf("from: %i, to %i - idx from %i, idx to %i\n",(int)from.tick,(int)to.tick,idx_from,idx_to);
	if (idx_from==INVALID_STREAM_INDEX || idx_to==INVALID_STREAM_INDEX || idx_to==pattern_track->get_pattern().get_stream_size() || idx_to<idx_from) {
		/* empty thing */
		return notelist;
	}
	//printf("found stuff!\n");
	for (int i=idx_from;i<=idx_to;i++) {
		EdNote note;
		note.pos=pattern_track->get_pattern().get_index_pos(i);
		//printf("found tick: %i , column %i\n",(int)note.pos.tick,note.pos.column);

		if (note.pos<from || note.pos>=to)
			continue;

		note.note=pattern_track->get_pattern().get_index_value(i);
		notelist.push_back(note);
	}

	return notelist;
}

PatternEdit::UndoRedoNote *PatternEdit::get_undo_redo_current_note() {

	UndoRedoNote *undoredo = new UndoRedoNote(this);

	undoredo->old_note=pattern_track->get_note( get_cursor().get_tick_pos() , column );
	undoredo->new_note=undoredo->old_note; /* we will usually just modify something */
	undoredo->pos=PatternPosition( get_cursor().get_tick_pos() , column );

	return undoredo;



}

bool PatternEdit::key_press_event(int p_event) {

	printf("got %i\n",p_event);
	SWITCH(p_event)
		CASE( KEYBIND("left") ) {

			if (field==1)
			field=0;
			else if (column>0) {
				column--;
				field=1;
			} else
				song_edit->move_editing_left();
		}
		CASE( KEYBIND("right") ) {
			if (field==0)
				field=1;
			else if (column<(pattern_track->get_columns()-1)) {
				column++;
				field=0;
			} else
				song_edit->move_editing_right();
		}
		CASE( KEYBIND("up") ) {

			song_edit->cursor_move_up();

		}
		CASE( KEYBIND("down") ) {

			song_edit->cursor_move_down();

		}
		CASE( KEYBIND("note_entry/clear_field") ) {

			pattern_track->set_note(get_cursor().get_tick_pos(),column,PatternNote(PatternNote::NO_NOTE));

		}
		CASE( KEYBIND("note_entry/note_off") ) {

			pattern_track->set_note(get_cursor().get_tick_pos(),column,PatternNote(PatternNote::NOTE_OFF));

		}
		CASE( KEYBIND("note_entry/toggle_note_edit") ) {


			switch (note_edit_mode) {

				case MODE_NOTE: note_edit_mode=MODE_VOLUME; break;
				case MODE_VOLUME: note_edit_mode=MODE_PORTA; break;
				case MODE_PORTA: note_edit_mode=MODE_NOTE; break;
			}

		}
		END_SWITCH;

	//check if this is a note
	if (field==0 && note_edit_mode==MODE_NOTE) {

		for (int i=0;i<MAX_KEYS;i++) {
			if (IS_KEYBIND((String)"note_entry/"+key_name[i],p_event)) {

				int note=GlobalEdit::get_editing_octave()*12+i;
				Tick tickpos=get_cursor().get_tick_pos();
				pattern_track->set_note(tickpos,column,PatternNote(note,60));

			}
		}
	}

	if (note_edit_mode==MODE_VOLUME && Keyboard_Input::get_singleton_instance()->is_number(p_event)) {

		int number=Keyboard_Input::get_singleton_instance()->get_number(p_event);
		Tick tick=get_cursor().get_tick_pos(); //@TODO make in many notes
		PatternNote note = pattern_track->get_note(tick,column);
		if (note.is_empty())
			return false;
		if (field==0) {
			note.volume=(note.volume%10)+10*number;
			field=1;
		} else
			note.volume=(note.volume/10)*10+number;

		pattern_track->set_note(tick,column,note);

	};


	return true;

}

void PatternEdit::edit_command(const EditCommand &p_command) {



	switch (p_command.type) {
		case EditCommand::NUMBER_INPUT_COMMAND: {

				const NumberInputCommand *cmd = dynamic_cast<const NumberInputCommand*>(&p_command);

				ERR_FAIL_COND( cmd == NULL );

				UndoRedoNote * op = PatternEdit::get_undo_redo_current_note();
				switch (note_edit_mode) {
					case MODE_NOTE: {

						op->new_note.set_octave(cmd->number);
						get_undo_stream()->new_block("Set Octave");

					} break;
					case MODE_VOLUME: {

						if (column==0)
							op->new_note.volume=(op->new_note.volume%10)+cmd->number*10;
						else
							op->new_note.volume=((op->new_note.volume/10)*10)+cmd->number;

						get_undo_stream()->new_block("Set Volume");

					} break;
					case MODE_PORTA: {

						if (column==0)
							op->new_note.portamento=(op->new_note.portamento%10)+cmd->number*10;
						else
							op->new_note.portamento=((op->new_note.portamento/10)*10)+cmd->number;
						get_undo_stream()->new_block("Set Portamento");
					} break;

				}

				pattern_track->set_note( get_cursor().get_tick_pos() , column , op->new_note );
				get_undo_stream()->add_operation(op);
			}
	}

}

int PatternEdit::get_cursor_column() {


	return column;
}
int PatternEdit::get_cursor_field() {

	return field;
}

int PatternEdit::get_columns() {

	return pattern_track->get_columns();
}
void PatternEdit::set_columns(int p_columns) {


	pattern_track->set_columns(p_columns);
}


PatternEdit::NoteEditMode PatternEdit::get_note_edit_mode() {


	return note_edit_mode;
}

Track * PatternEdit::get_track() {

	return pattern_track;
}

PatternEdit::PatternEdit(UndoStream *p_undo_stream, Cursor *p_cursor,PatternTrack *p_track,SongEdit *p_song_edit) : TrackEdit(p_undo_stream,p_cursor) {

	song_edit=p_song_edit;

	note_edit_mode=MODE_NOTE;
	column=0;
	field=0;
	pattern_track=p_track;
	p_track->set_note(0,0,PatternNote(50,80));
	p_track->set_note(33,0,PatternNote(50,PatternNote::MAX_VOLUME));
	p_track->set_note(66,0,PatternNote(PatternNote::NOTE_OFF));
	p_track->set_note(TICKS_PER_BEAT,0,PatternNote(22,55));
	p_track->set_note(TICKS_PER_BEAT*2,0,PatternNote(PatternNote::NOTE_OFF));
	p_track->set_note(TICKS_PER_BEAT*2,1,PatternNote(44,44));
}

}