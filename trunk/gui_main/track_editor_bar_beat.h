//
// C++ Interface: track_editor_bar_beat
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TRACK_EDITOR_BAR_BEAT_H
#define TRACK_EDITOR_BAR_BEAT_H


#include "base/widget.h"
#include "editor/editor.h"

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class TrackEditorBarBeat : public GUI::Widget {

	enum {
	
		MAX_CHAR_WIDTH=8	
	};

	int get_font_width();
	int get_row_height();

	GUI::Size get_minimum_size_internal();
	void draw(const GUI::Point& p_pos,const GUI::Size& p_size,const GUI::Rect& p_exposed);

public:
    TrackEditorBarBeat();

    ~TrackEditorBarBeat();

};

#endif
