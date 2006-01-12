//
// C++ Interface: editor_play_position
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDEDITOR_PLAY_POSITION_H
#define RESHAKEDEDITOR_PLAY_POSITION_H


#include <Qt/qwidget.h>
#include "editor/editor.h"

namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class EditorPlayPosition : public QWidget {
				       
	Editor *editor;
	void paintEvent(QPaintEvent *pe);
	
	int last_drawn_play_pos;
	bool last_draw_play_visible;
	
	int get_playback_snap_pos();
	bool is_playback_visible();
	
public:
	
	void check_pos_changed();
	
	EditorPlayPosition(QWidget*p_parent,Editor *p_editor);
	~EditorPlayPosition();

};

}

#endif
