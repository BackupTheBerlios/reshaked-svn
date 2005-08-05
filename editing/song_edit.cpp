#include "song_edit.h"

namespace ReShaked {

void SongEdit::undo(UndoRedoOp *p_item) {


}

void SongEdit::redo(UndoRedoOp *p_item) {



}

void SongEdit::add_audio_track() {



}
void SongEdit::add_pattern_track() {


}
void SongEdit::add_bus_track() {



}

SongEdit::SongEdit(UndoStream *p_undo_stream,Song *p_song) : UndoRedoOwner(p_undo_stream) {



};



};