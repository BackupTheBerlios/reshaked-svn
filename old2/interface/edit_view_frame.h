//
// C++ Interface: edit_view_frame
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDEDIT_VIEW_FRAME_H
#define RESHAKEDEDIT_VIEW_FRAME_H


#include "containers/box_container.h"
#include "bundles/scroll_box.h"
#include "interface/edit_view_toolbar.h"
#include "interface/edit_view_blocklist.h"
#include "interface/edit_view_beatbar.h"
#include "interface/gui_update_notify.h"
#include "editor/editor.h"

using namespace GUI;

namespace ReShaked {

/**
	@author Juan Linietsky <reshaked@gmail.com>
*/
class EditViewFrame : public VBoxContainer {
	
	Editor *editor;
	HBoxContainer *hbc_base,*hbc;
	EditViewToolbar *toolbar;
	EditViewBeatBar *beat_bar;
	ScrollBox *viewport;
	
	std::vector<Widget*> sidewidgets;
	std::vector<EditViewBlockList*> edit_blocklists;
	
	void blocklist_ui_under_cursor_request_slot(EditViewBlockList* p_blocklist,const Point& p_global);
	
	
	void ensure_cursor_visible();
	void cursor_changed_blocklist();
	void edit_window_moved_slot();
	void repaint_blocklists(const std::list<int>& p_which);
	
	void show_track_dialog_callback(const Point& p_pos, int p_track);
	void show_automation_dialog_callback(const Point& p_pos, int p_track,int p_automation);
	
	
public:	
	
	Signal< Method2<const Point&,int>  > show_track_dialog_signal;
	Signal< Method3<const Point&,int,int>  > show_automation_dialog_signal;
	
	void repaint();
	void rebuild();
	EditViewFrame(GUI_UpdateNotify *p_notifier,Editor *p_editor);
	~EditViewFrame();

};

}

#endif
