//
// C++ Interface: track_editor_track_name
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TRACK_EDITOR_TRACK_NAME_H
#define TRACK_EDITOR_TRACK_NAME_H

#include "base/widget.h"
#include "editor/editor.h"
#include "gui_main/track_editor.h"
#include "bundles/popup_menu.h"
#include "bundles/input_dialog.h"
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class TrackEditorTrackName : public GUI::Widget {


	enum MenuOption {
	
		TRACK_MUTE,
		TRACK_SOLO,
		PATTERN_ADD_COLUMN,
		PATTERN_REMOVE_COLUMN,
		TRACK_MOVE_LEFT,
		TRACK_MOVE_RIGHT,
		TRACK_RENAME,
		TRACK_REMOVE	
	};
	

	GUI::PopUpMenu *menu;

	GUI::StringInputDialog *name_input;
	
	void name_input_callback(String p_string);
	void menu_callback(int p_option);
	void about_to_show_menu();

	int over;
	TrackEditor *track_editor;
	GUI::Size get_minimum_size_internal();
	virtual void draw(const GUI::Point& p_pos,const GUI::Size& p_size,const GUI::Rect& p_exposed);
	virtual void mouse_button(const GUI::Point& p_pos, int p_button,bool p_press,int p_modifier_mask);
	virtual void mouse_motion(const GUI::Point& p_pos, const GUI::Point& p_rel, int p_button_mask);
	virtual void mouse_leave();
	
	void set_in_window();
public:
	TrackEditorTrackName(TrackEditor *p_track_editor);	
	~TrackEditorTrackName();

};

#endif
