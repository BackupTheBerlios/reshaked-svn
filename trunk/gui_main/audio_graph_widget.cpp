//
// C++ Implementation: audio_graph_widget
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "audio_graph_widget.h"
#include "base/painter.h"

void AudioGraphWidget::draw(const GUI::Point& p_pos,const GUI::Size& p_size,const GUI::Rect& p_exposed) {

	get_painter()->draw_fill_rect( GUI::Point(), p_size, GUI::Color(0) );
}

AudioGraphWidget::AudioGraphWidget(Song *p_song) {

	song=p_song;
	
}


AudioGraphWidget::~AudioGraphWidget()
{
}


