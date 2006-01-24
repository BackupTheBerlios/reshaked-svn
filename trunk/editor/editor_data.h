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
#include "editor/selection_data.h"

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
			
			bool operator==(const Pos& p_pos) { return (blocklist==p_pos.blocklist && column==p_pos.column && tick==p_pos.tick); }
			bool operator!=(const Pos& p_pos) { return (!(*this==p_pos)); }
			
			Pos(int p_blocklist=0,int p_column=0, Tick p_tick=0) { blocklist=p_blocklist; column=p_column; tick=p_tick; }
		} begin,end;
		bool enabled;

		enum ShiftSelectingChanging {
			CHANGE_BEGIN,
			CHANGE_END
		};

		enum PasteMode {
			PASTE_OVERWRITE,
			PASTE_MIX,
		};
		
		bool shift_selecting;
		ShiftSelectingChanging shift_selecton_change_tick;
		ShiftSelectingChanging shift_selecton_change_blocklist_column;
		
		Pos mouse_drag_begin;
		Pos mouse_drag_end;
		
		SelectionData data;
		
	} selection;

	UndoStream undo_stream;
	Song *song;
	Cursor cursor;
	UI_UpdateNotify *ui_update_notify;
	EditorData(Song *p_song,UI_UpdateNotify *p_ui_update_notify);

};

}

#endif
