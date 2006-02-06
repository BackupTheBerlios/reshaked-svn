//
// C++ Interface: qt_updatenotify
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDQT_UPDATENOTIFY_H
#define RESHAKEDQT_UPDATENOTIFY_H

#include <Qt/qobject.h>
#include "editor/ui_updatenotify.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
class Qt_UpdateNotify : public QObject, public UI_UpdateNotify  {
			    
	Q_OBJECT
			
signals:			
			
	void cursor_moved();
	void edit_window_changed();
	void cursor_changed_blocklist();
	void track_list_changed();
	void edit_window_moved();
	void editing_octave_changed();
	void update_blocklist_list(const std::list<int>& p_which);	
	void current_automation_status_changed();
	void block_layout_changed();
	void rack_changed();
	void track_names_changed();
	void rack_repaint();
	
	void volume_mask_changed();
	
	void notify_action(String p_action);	
	
	/* MVC Hacks */
	/* I have no way to fix this. The problem is that i cant remove/recreate the pattern display widget from itself when a keypress (shortcut) from within happens. Connecting to thse in a queued fashion is the only way */
	void current_track_add_column();	
	void current_track_remove_column();
	void automation_editor_popup(int p_track);	
	
public:
	
	
	Qt_UpdateNotify(QObject *p_parent);
	~Qt_UpdateNotify();
	

};

}

#endif
