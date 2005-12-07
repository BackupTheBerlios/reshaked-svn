//
// C++ Interface: editor_commands
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDEDITOR_COMMANDS_H
#define RESHAKEDEDITOR_COMMANDS_H

#include "editor/editor_data.h"
#include "engine/track_pattern.h"

namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class Editor;
class EditorCommands {
friend class Editor;
	EditorData *d;
	Editor *editor;
	
	
	
	CommandFunc* add_track(bool p_no_undo,Track *p_track,int p_at_pos);
	CommandFunc* remove_track(bool p_no_undo,int p_which);
	
	CommandFunc* set_note(bool p_no_undo,Track_Pattern *p_pattern,Track_Pattern::Position,Track_Pattern::Note);

};

}

#endif
