//
// C++ Interface: audio_graph_screen
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef AUDIO_GRAPH_SCREEN_H
#define AUDIO_GRAPH_SCREEN_H

#include "gui_main/audio_graph_widget.h"
#include "bundles/scroll_box.h"

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class AudioGraphScreen : public GUI::VBoxContainer {

	Song *song;

	AudioGraphWidget *audio_graph_widget;
	GUI::ScrollBox *scroll_box;

public:
	AudioGraphScreen(Song *p_song);	
	~AudioGraphScreen();

};

#endif
