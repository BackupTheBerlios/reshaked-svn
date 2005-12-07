//
// C++ Implementation: editor_data
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "editor_data.h"

namespace ReShaked {

EditorData::EditorData(Song *p_song,UI_UpdateNotify *p_ui_update_notify) :cursor(p_ui_update_notify) {
	song=p_song;
	ui_update_notify=p_ui_update_notify;
	global_edit.setnote_dont_update=false;
}




}
