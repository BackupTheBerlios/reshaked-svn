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

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class AudioGraphWidget : public GUI::Widget{

	Song *song;

	virtual void draw(const GUI::Point& p_pos,const GUI::Size& p_size,const GUI::Rect& p_exposed);

public:

	AudioGraphWidget(Song *p_song);
	~AudioGraphWidget();

};

#endif
