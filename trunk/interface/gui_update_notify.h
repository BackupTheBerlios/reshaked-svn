//
// C++ Interface: gui_update_notify
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDGUI_UPDATE_NOTIFY_H
#define RESHAKEDGUI_UPDATE_NOTIFY_H

#include "editor/ui_updatenotify.h"
#include "signals/signals.h"
using namespace GUI;
		
		
namespace ReShaked {

/**
	@author Juan Linietsky <reshaked@gmail.com>
*/
class GUI_UpdateNotify : public UI_UpdateNotify {
protected:	
	virtual void cursor_moved();
	virtual void edit_window_changed();
	virtual void edit_window_moved();
	virtual void cursor_changed_blocklist();
	virtual void track_list_changed();
	virtual void editing_octave_changed();
	virtual void rack_changed();
	virtual void rack_repaint();
	
	virtual void current_automation_status_changed();
	
	virtual void update_blocklist_list(const std::list<int>& p_which);
	
	virtual void block_layout_changed();
	
	virtual void track_names_changed();
	
	virtual void volume_mask_changed();
	virtual void cursor_step_changed();
	
	virtual void notify_action(String p_action);
	
	virtual void editor_marker_edit_request();
	virtual void editor_volume_scale_request();
	
	/* MVC Hacks */
	virtual void current_track_add_column();	
	virtual void current_track_remove_column();
	virtual void automation_editor_popup(int p_track);
	virtual void automation_options(int p_blocklist);
	
	
public:
	
	Signal<> cursor_moved_signal;
	Signal<> edit_window_changed_signal;
	Signal<> edit_window_moved_signal;
	Signal<> cursor_changed_blocklist_signal;
	Signal<> track_list_changed_signal;
	Signal<> editing_octave_changed_signal;
	Signal<> rack_changed_signal;
	Signal<> rack_repaint_signal;
	
	Signal<> current_automation_status_changed_signal;
	
	Signal<Method1<const std::list<int>&> > update_blocklist_list_signal;
	
	Signal<> block_layout_changed_signal;
	
	Signal<> track_names_changed_signal;
	
	Signal<> volume_mask_changed_signal;
	Signal<> cursor_step_changed_signal;
	
	Signal<Method1<String> > notify_action_signal;
	
	Signal<> editor_marker_edit_request_signal;
	Signal<> editor_volume_scale_request_signal;
	
	/* MVC Hacks */
	Signal<> current_track_add_column_signal;	
	Signal<> current_track_remove_column_signal;
	Signal<Method1<int> > automation_editor_popup_signal;
	Signal<Method1<int> > automation_options_signal;
	
	GUI_UpdateNotify();
	~GUI_UpdateNotify();

};

}

#endif
