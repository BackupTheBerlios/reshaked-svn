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
	ScrollBox *viewport;
	
	std::vector<EditViewBlockList*> edit_blocklists;
	
	void blocklist_ui_under_cursor_request_slot(EditViewBlockList* p_blocklist);
	
	
	void ensure_cursor_visible();
	void cursor_changed_blocklist();
	void edit_window_moved_slot();
	void repaint_blocklists(const std::list<int>& p_which);
public:	
	
	
	void repaint();
	void rebuild();
	EditViewFrame(GUI_UpdateNotify *p_notifier,Editor *p_editor);
	~EditViewFrame();

};

}

#endif
