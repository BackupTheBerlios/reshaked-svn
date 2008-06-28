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
	global_edit.automation_point_dont_update=false;
	selection.shift_selecting=false; 
	selection.shift_selection_valid=false;
	pattern_edit.volume_mask=99;
	pattern_edit.volume_mask_active=true;
	pattern_edit.poly_input_offset=0;
	pattern_edit.last_noteon=0;
	
	global_edit.current_rack_track=-1;
	global_edit.pattern_midi_input_enabled=false;
	global_edit.polyphonic_midi_input_enabled=false;
	
	global_edit.cursor_step=1;
	
}




}
