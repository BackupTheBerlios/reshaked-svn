#include "song_edit.h"

#include "pattern_edit.h"

namespace ReShaked {

void SongEdit::undo(UndoRedoOp *p_item) {


}

void SongEdit::redo(UndoRedoOp *p_item) {



}

void SongEdit::add_audio_track(int p_channels) {



}
void SongEdit::add_pattern_track(int p_channels) {

	UndoRedoTrackAdd * op = new UndoRedoTrackAdd(this);

        PatternTrack *pattern_track = new PatternTrack(p_channels);
	op->track = pattern_track;
	song->add_track(op->track);
	op->track_edit = new PatternEdit(get_undo_stream(),&cursor, pattern_track);
	track_editors.push_back(op->track_edit);

	get_undo_stream()->new_block("Add Pattern Track");
	get_undo_stream()->add_operation(op);

}
void SongEdit::add_bus_track(int p_channels) {



}

SongEdit::SongEdit(UndoStream *p_undo_stream,Song *p_song) : UndoRedoOwner(p_undo_stream) {

	song=p_song;


};



};