#ifndef SONG_EDIT_H
#define SONG_EDIT_H

#include "engine/song.h"
#include "editing/undo_stream.h"
#include "editing/cursor.h"
#include "editing/track_edit.h"

namespace ReShaked {

class SongEdit : public UndoRedoOwner {


	struct UndoRedoTrackAdd : public UndoRedoOp {

	        Track * track;
		TrackEdit *track_edit;

		UndoRedoTrackAdd(UndoRedoOwner *p_owner) : UndoRedoOp(p_owner) {};
	};
	
	Song *song;
	Cursor cursor;

	std::vector<TrackEdit*> track_editors;

 	void undo(UndoRedoOp *p_item);
	void redo(UndoRedoOp *p_item);
public:

	void add_audio_track(int p_channels);
	void add_pattern_track(int p_channels);
	void add_bus_track(int p_channels);

	SongEdit(UndoStream *p_undo_stream,Song *p_song);

};

};

#endif
