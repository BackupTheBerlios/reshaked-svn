#include "song_edit.h"

#include "pattern_edit.h"

namespace ReShaked {


Cursor& SongEdit::get_cursor() {
	
	return cursor;	
	
}
void SongEdit::undo(UndoRedoOp *p_item) {


}

void SongEdit::redo(UndoRedoOp *p_item) {



}
int SongEdit::get_track_count() {

	return track_editors.size();
}
TrackEdit* SongEdit::get_track_edit(int p_index) {

	ERR_FAIL_INDEX_V(p_index,track_editors.size(),NULL);
	return track_editors[p_index];

}

void SongEdit::add_audio_track(int p_channels) {



}


void SongEdit::add_pattern_track(int p_channels) {

	UndoRedoTrackAdd * op = new UndoRedoTrackAdd(this);

        PatternTrack *pattern_track = new PatternTrack(p_channels);
	op->track = pattern_track;
	song->add_track(op->track);
	op->track_edit = new PatternEdit(get_undo_stream(),&cursor, pattern_track,this);
	track_editors.push_back(op->track_edit);

	get_undo_stream()->new_block("Add Pattern Track");
	get_undo_stream()->add_operation(op);

}
void SongEdit::add_bus_track(int p_channels) {



}

void SongEdit::cursor_move_up() {

	cursor.set_pos(cursor.get_pos()-1);

}
void SongEdit::cursor_move_down() {

	cursor.set_pos(cursor.get_pos()+1);

}
void SongEdit::cursor_move_page_up() {


}
void SongEdit::cursor_move_page_down() {


}
void SongEdit::cursor_move_pattern_up() {


}
void SongEdit::cursor_move_pattern_down() {


}

void SongEdit::cursor_move_home() {


}
void SongEdit::cursor_move_end() {


}

void SongEdit::move_editing_left() {

	

}

void SongEdit::move_editing_right() {


}


SongEdit::SongEdit(UndoStream *p_undo_stream,Song *p_song) : UndoRedoOwner(p_undo_stream) {

	song=p_song;
	editing_octave=4;

};



};