//
// C++ Interface: editor_data
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDEDITOR_DATA_H
#define RESHAKEDEDITOR_DATA_H

#include "engine/song.h"
#include "editor/cursor.h"
#include "editor/undo_stream.h"

namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

   
					       
class EditorData{
public:

	enum PatternNoteEditMode {
		MODE_NOTE,
		MODE_VOLUME,
	};	
				       
private:				       
friend class Editor;				       
friend class EditorCommands;	
			       
	enum EnterBLDir {
		ENTER_LEFT,
		ENTER_RIGHT
	};
	enum {

		MAX_FIELDS=2
	};


	struct PatternEdit {


		PatternNoteEditMode note_edit_mode;
		int column;
		int field;
		

	} pattern_edit;

	struct GlobalEdit {

		int current_blocklist;
		int editing_octave;
		bool setnote_dont_update;
		bool automation_point_dont_update;

	} global_edit;

	struct Selection {

		struct Pos {
			int blocklist,column;
			Tick tick; //using tick so resizing works
		} begin,end;
		bool enabled;

	} selection;

	UndoStream undo_stream;
	Song *song;
	Cursor cursor;
	UI_UpdateNotify *ui_update_notify;
	EditorData(Song *p_song,UI_UpdateNotify *p_ui_update_notify);

};

}

#endif
