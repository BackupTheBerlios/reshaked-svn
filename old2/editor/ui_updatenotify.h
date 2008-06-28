//
// C++ Interface: ui_updatenotify
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDUI_UPDATENOTIFY_H
#define RESHAKEDUI_UPDATENOTIFY_H
#include <list>
#include "typedefs.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
class UI_UpdateNotify{
public:
	
	virtual void cursor_moved()=0;
	virtual void edit_window_changed()=0;
	virtual void edit_window_moved()=0;
	virtual void cursor_changed_blocklist()=0;
	virtual void track_list_changed()=0;
	virtual void editing_octave_changed()=0;
	virtual void rack_changed()=0;
	virtual void rack_repaint()=0;
	
	virtual void current_automation_status_changed()=0;
	
	virtual void update_blocklist_list(const std::list<int>& p_which)=0;
	
	virtual void block_layout_changed()=0;
	
	virtual void track_names_changed()=0;
	
	virtual void volume_mask_changed()=0;
	virtual void cursor_step_changed()=0;
	
	virtual void notify_action(String p_action)=0;
	
	virtual void editor_marker_edit_request()=0;
	virtual void editor_volume_scale_request()=0;
	
	/* MVC Hacks */
	virtual void current_track_add_column()=0;	
	virtual void current_track_remove_column()=0;
	virtual void automation_editor_popup(int p_track)=0;
	virtual void automation_options(int p_blocklist)=0;
	
	virtual ~UI_UpdateNotify(){}
	
};

}

#endif
