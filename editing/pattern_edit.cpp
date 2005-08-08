#include "pattern_edit.h"

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

PatternEdit::UndoRedoNote *PatternEdit::get_undo_redo_current_note() {

	UndoRedoNote *undoredo = new UndoRedoNote(this);

	undoredo->old_note=pattern_track->get_note( get_cursor().get_tick() , subcolumn );
	undoredo->new_note=undoredo->old_note; /* we will usually just modify something */
	undoredo->pos=PatternPosition( get_cursor().get_tick() , subcolumn );

	return undoredo;
	
	
	
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

					if (subcolumn==0)
						op->new_note.volume=(op->new_note.volume%10)+cmd->number*10;
					else
						op->new_note.volume=((op->new_note.volume/10)*10)+cmd->number;
					
					get_undo_stream()->new_block("Set Volume");
                        		
				} break;
				case MODE_PORTA: {

					if (subcolumn==0)
						op->new_note.portamento=(op->new_note.portamento%10)+cmd->number*10;
					else
						op->new_note.portamento=((op->new_note.portamento/10)*10)+cmd->number;
					get_undo_stream()->new_block("Set Portamento");
				} break;

			}

                        pattern_track->set_note( get_cursor().get_tick() , subcolumn , op->new_note );
			get_undo_stream()->add_operation(op);
		}
	}

}

Track * PatternEdit::get_track() {

	return pattern_track;
}

PatternEdit::PatternEdit(UndoStream *p_undo_stream, Cursor *p_cursor,PatternTrack *p_track) : TrackEdit(p_undo_stream,p_cursor) {

    pattern_track=p_track;

}

}