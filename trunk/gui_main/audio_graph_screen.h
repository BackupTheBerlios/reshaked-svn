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
#include "widgets/button_group.h"

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class AudioGraphScreen : public GUI::VBoxContainer {

	Song *song;


	void layer_group_changed(int p_to);

	GUI::ButtonGroup layer_group;

	AudioGraphWidget *audio_graph_widget;
	GUI::ScrollBox *scroll_box;

public:


	AudioGraphScreen(GUI_UpdateNotify *p_update_notify,Song *p_song);	
	~AudioGraphScreen();

};

#endif
