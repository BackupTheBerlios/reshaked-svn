//
// C++ Interface: track_editor_screen
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TRACK_EDITOR_SCREEN_H
#define TRACK_EDITOR_SCREEN_H


#include "containers/box_container.h"
#include "editor/editor.h"
#include "widgets/scroll_bar.h"
#include "bundles/scroll_box.h"
#include "gui_main/track_editor_top.h"
#include "gui_main/track_editor.h"
#include "gui_main/gui_update_notify.h"


/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class TrackEditorScreen : public GUI::VBoxContainer {

	TrackEditorTop *top;
	Editor *editor;

	GUI::VBoxContainer *vb_holder;
	GUI::HBoxContainer *hb;
	
	GUI::ScrollContainer *scroll;

	GUI::ScrollBar *v_scroll;
	std::vector< TrackEditor * > track_editors;
	
public:

	void rebuild();
	
	TrackEditorScreen(Editor *p_editor,GUI_UpdateNotify *p_update_notify);
	
	~TrackEditorScreen();

};

#endif
