//
// C++ Implementation: music_event
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "music_event.h"

MusicEvent::MusicEvent(Type p_type, unsigned char p_chan,unsigned char data1, unsigned char data2 ) {

	type=p_type;
	channel=p_chan;
	raw.param1=data1;
	raw.param2=data2;
}


MusicEvent::MusicEvent() {

	type=NONE;
	channel=0;
	raw.param1=0;
	raw.param2=0;
}


MusicEvent::~MusicEvent()
{
}


