//
// C++ Implementation: audio_graph_screen
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "audio_graph_screen.h"

AudioGraphScreen::AudioGraphScreen(Song *p_song) {

	song=p_song;

	scroll_box = add( new GUI::ScrollBox, 1 );
	audio_graph_widget = scroll_box->set( new AudioGraphWidget(p_song) );
	scroll_box->set_expand_h(true);
	scroll_box->set_expand_v(true);
	scroll_box->set_scroll_h(true);
	scroll_box->set_scroll_v(true);	
	
}


AudioGraphScreen::~AudioGraphScreen()
{
}


