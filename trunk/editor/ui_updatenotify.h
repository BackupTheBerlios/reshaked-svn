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
	
	
	virtual void current_automation_status_changed()=0;
	
	virtual void update_blocklist_list(const std::list<int>& p_which)=0;
	
	virtual void block_layout_changed()=0;
	
	virtual void track_names_changed()=0;
	virtual ~UI_UpdateNotify(){}
	
};

}

#endif
