//
// C++ Interface: audio_graph_widget
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef AUDIO_GRAPH_WIDGET_H
#define AUDIO_GRAPH_WIDGET_H


#include "engine/song.h"
#include "base/widget.h"
#include "gui_main/gui_update_notify.h"
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class AudioGraphWidget : public GUI::Widget{

	Song *song;

	int current_layer;

	GUI::Size get_node_size(AudioNode *p_node);

	void draw_node(const GUI::Rect& p_rect,AudioNode *p_node);
	virtual void draw(const GUI::Point& p_pos,const GUI::Size& p_size,const GUI::Rect& p_exposed);

public:

	enum {
	
		SHOW_ALL_LAYERS=-1
	};

	void redraw();

	AudioGraphWidget(GUI_UpdateNotify *p_update_notify,Song *p_song);
	~AudioGraphWidget();

};

#endif
