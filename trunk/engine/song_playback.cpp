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
#include "get_time.h"
namespace ReShaked {


unsigned int SongPlayback::get_song_playback_time() {
	
	return play_start_time;
}

void SongPlayback::play(Tick p_from_pos) {
	
	status=STATUS_PLAY;
	loopdata.active=false;
	current_tick=(Sint64)p_from_pos<<TICK_FRAC;
	prev_tick=0;
	latency_buffer.clear();
	play_start_time=GetTime::get_time_msec();
}
void SongPlayback::loop(Tick p_begin,Tick p_end) {
	
	ERR_FAIL_COND( p_begin>=p_end );
	status=STATUS_PLAY;
	loopdata.active=true;
	loopdata.begin=p_begin;
	loopdata.end=p_end;
	current_tick=(Sint64)p_begin<<TICK_FRAC;
	prev_tick=p_begin;
	latency_buffer.clear();
	play_start_time=GetTime::get_time_msec();
	
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
	
	return current_tick>>TICK_FRAC;
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
	
	if (loopdata.active && (current_tick>>TICK_FRAC) >=loopdata.end)
		current_tick=(Sint64)loopdata.begin<<TICK_FRAC;
	
	double time=(double)p_frames/mix_rate;
	double beat_size=60.0/properties->get_tempo().get();
	double beat_frac=time/beat_size;
	Sint64 tick_adv=(Sint64)(beat_frac*(double)TICKS_PER_BEAT*(double)(1<<TICK_FRAC));
	
	prev_tick=current_tick>>TICK_FRAC;
	current_tick+=tick_adv;
	latency_buffer.add( prev_tick,tick_adv>>TICK_FRAC,time*1000.0);
	
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

void SongPlayback::set_recording(bool p_recording) {
	
	record=p_recording;
}

bool SongPlayback::can_process_tick() {
	
	return process_tick;
}

bool SongPlayback::is_recording() {
	
	return record;
}

SongPlayback::SongPlayback(GlobalProperties *p_properties) {
	
	properties=p_properties;	
	status=STATUS_STOP;
	mix_rate=1;
	current_tick=0;
	prev_tick=0;
	latency_buffer.clear();
	record_automation=false;
	record=false;
	play_start_time=0;
}


SongPlayback::~SongPlayback() {
	

}


}
