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
#include "engine/audio_control.h"
namespace ReShaked {



void SongPlayback::play(Tick p_from_pos) {
	
	status=STATUS_PLAY;
	loopdata.active=false;
	current_tick=p_from_pos;
	prev_tick=0;
	latency_buffer.clear();
}
void SongPlayback::loop(Tick p_begin,Tick p_end) {
	
	ERR_FAIL_COND( p_begin>=p_end );
	status=STATUS_PLAY;
	loopdata.active=true;
	loopdata.begin=p_begin;
	loopdata.end=p_end;
	current_tick=p_begin;
	prev_tick=p_begin;
	latency_buffer.clear();
	
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

bool SongPlayback::is_loop_active() {
	
	return loopdata.active;
}

int SongPlayback::advance(int p_frames) {
	if (status!=STATUS_PLAY)
		return p_frames;
	
	if (loopdata.active && current_tick>=loopdata.end)
		current_tick=loopdata.begin;
	
	float time=(float)p_frames/mix_rate;
	float beat_size=60.0/properties->get_tempo().get();
	float beat_frac=time/beat_size;
	Tick tick_adv=(int)(beat_frac*(float)TICKS_PER_BEAT);
	
	prev_tick=current_tick;
	current_tick+=tick_adv;
	latency_buffer.add( prev_tick,tick_adv,time*1000.0);
	
	return p_frames;
}

Tick SongPlayback::get_playback_pos_tick() {
	
	if (status!=STATUS_PLAY)
		return 0;
	
	int latency_msecs=int(((float)AudioControl::get_output_latency()/(float)mix_rate)*1000.0);
	
	Tick pos=latency_buffer.get_tick_at_latency(latency_msecs);
	if (pos==-1)
		return prev_tick;
	return pos;
}

void SongPlayback::set_recording_automations(bool p_recording) {
	
	record_automation=p_recording;
}
bool SongPlayback::is_recording_automations() {
	
	return record_automation;
}

SongPlayback::SongPlayback(GlobalProperties *p_properties) {
	
	properties=p_properties;	
	status=STATUS_STOP;
	mix_rate=1;
	current_tick=0;
	prev_tick=0;
	latency_buffer.clear();
	record_automation=false;
}


SongPlayback::~SongPlayback() {
	

}


}
