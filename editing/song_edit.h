#ifndef SONG_EDIT_H
#define SONG_EDIT_H

#include "engine/song.h"
#include "editing/undo_stream.h"
#include "editing/cursor.h"

namespace ReShaked {

class SongEdit : public UndoRedoOwner {

	Song *song;
	Cursor cursor;

	std::vector<TrackEdit*> track_editors;

 	void undo(UndoRedoOp *p_item);
	void redo(UndoRedoOp *p_item);
public:

	void add_audio_track();
	void add_pattern_track();
	void add_bus_track();

	SongEdit(UndoStream *p_undo_stream,Song *p_song);

};

};

#endif