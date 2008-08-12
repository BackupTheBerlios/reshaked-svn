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

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class TrackEditorTrackName : public GUI::Widget {

	TrackEditor *track_editor;
	GUI::Size get_minimum_size_internal();
	void draw(const GUI::Point& p_pos,const GUI::Size& p_size,const GUI::Rect& p_exposed);
	void mouse_button(const GUI::Point& p_pos, int p_button,bool p_press,int p_modifier_mask);
public:
	TrackEditorTrackName(TrackEditor *p_track_editor);	
	~TrackEditorTrackName();

};

#endif
