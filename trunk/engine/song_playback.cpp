//
// C++ Implementation: song_playback
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "song_playback.h"

namespace ReShaked {



void SongPlayback::play(Tick p_from_pos) {
	
	status=STATUS_PLAY;
	loopdata.active=false;
	current_tick=0;
	prev_tick=0;
}
void SongPlayback::loop(Tick p_begin,Tick p_end) {
	
	ERR_FAIL_COND( p_begin>=p_end );
	
	status=STATUS_PLAY;
	loopdata.active=true;
	loopdata.begin=p_begin;
	loopdata.end=p_end;
	current_tick=p_begin;
	prev_tick=p_begin;
	
}
void SongPlayback::set_pause(bool p_paused) {
	
	status=STATUS_PAUSE;
}
void SongPlayback::stop() {
	
	status=STATUS_STOP;
	
}


void SongPlayback::set_mix_rate(float p_mix_rate) {
	
	mix_rate=p_mix_rate;	
}

float SongPlayback::get_mix_rate() {
	
	return mix_rate;
}
SongPlayback::Status SongPlayback::get_status() {
	
	return status;
}

Tick SongPlayback::get_current_tick_to() {
	
	return current_tick;
}
Tick SongPlayback::get_current_tick_from() {
	
	
	return prev_tick;
}


int SongPlayback::advance(int p_frames) {
	if (status!=STATUS_PLAY)
		return p_frames;
	
	float time=(float)p_frames/mix_rate;
	float beat_size=60.0/properties->get_tempo().get();
	float beat_frac=time/beat_size;
	Tick tick_adv=(int)(beat_frac*(float)TICKS_PER_BEAT);
	
	prev_tick=current_tick;
	current_tick+=tick_adv;
	
	return p_frames;
}


SongPlayback::SongPlayback(GlobalProperties *p_properties) {
	
	properties=p_properties;	
	status=STATUS_STOP;
	mix_rate=1;
	current_tick=0;
	prev_tick=0;
}


SongPlayback::~SongPlayback() {
	

}


}
