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
#include "gui_main/track_editor_bar_beat.h"


/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class TrackEditorScreen : public GUI::VBoxContainer {

	TrackEditorBarBeat *barbeat;
	TrackEditorTop *top;
	
	Editor *editor;

	GUI::VBoxContainer *vb_holder;
	GUI::HBoxContainer *hb;
	
	GUI::ScrollContainer *scroll;

	GUI::ScrollBar *v_scroll;
	std::vector< TrackEditor * > track_editors;
	
	void track_block_changed(Track::Block *p_block);
	void mouse_selection_update_callback(GUI::Point p_pos);
	void cursor_track_changed_callback(int p_from,int p_to);
	void block_changed_callback(Track::Block *p_block);
	void track_changed_callback(Track *p_track);
	
public:

	void repaint();
	void rebuild();
	
	TrackEditorScreen(Editor *p_editor,GUI_UpdateNotify *p_update_notify);
	
	~TrackEditorScreen();

};

#endif
