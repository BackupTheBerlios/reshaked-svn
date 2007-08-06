//
// C++ Interface: track_dialog
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDTRACK_DIALOG_H
#define RESHAKEDTRACK_DIALOG_H


#include "bundles/popup_menu.h"
#include "editor/editor.h"
#include "bundles/input_dialog.h"
using namespace GUI;

namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class TrackDialog : public PopUpMenu {
	
	enum ItemID {
		
		ITEM_MUTE,
		ITEM_SOLO,
		ITEM_ADD_COLUMN,
		ITEM_REMOVE_COLUMN,
		ITEM_AUTOMATIONS,
		ITEM_MOVE_TRACK_LEFT,
		ITEM_MOVE_TRACK_RIGHT,
		ITEM_RENAME,
		ITEM_REMOVE
		
	};
	
	
	void action(int p_action);
	Editor *editor;
	Track * t;
	int t_idx;
	StringInputDialog *track_name;
	void track_renamed_callback(String p_name);
	
	
public:
	
	Signal< Method1<Track*> > request_automation_editor_signal;
	void show(const Point& p_pos,int p_track);
	
	TrackDialog(Window *p_parent,Editor *p_editor);
	~TrackDialog();

};

}

#endif
