//
// C++ Interface: song_playback
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSONG_PLAYBACK_H
#define RESHAKEDSONG_PLAYBACK_H

#include "typedefs.h"
#include "engine/global_properties.h"
#include "engine/latency_buffer.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
class SongPlayback {
public:
	enum Status {
		STATUS_PLAY,
		STATUS_PAUSE,
		STATUS_STOP,
							
	};
	
private:
	
	enum {
		
		TICK_FRAC=24
	};
	
	float mix_rate;
	Tick prev_tick;
	
	Sint64 current_tick; //< needs to be fractional, since ticks are advanced according to frames

	
	GlobalProperties *properties;
	
	struct Loop {
		bool active;
		Tick begin,end; 
	} loopdata;
	
	Status status;
	bool record_automation;
	bool record;
	unsigned int play_start_time;			    
	LatencyBuffer latency_buffer;
	
	bool process_tick; 
public:


	
	/* Control */
	void play(Tick p_from_pos=0);
	void loop(Tick p_begin,Tick p_end);
	void set_pause(bool p_paused);
	void stop();
	
	bool can_process_tick(); ///< If the amount of frames processed isnt enough for a change of tick, this is false
	
	void set_recording_automations(bool p_recording);
	bool is_recording_automations();
	void set_recording(bool p_recording);
	bool is_recording();
	
	/* Info */
	
	Status get_status();
	bool is_loop_active();
	
	/* Advance, return frames it actually advanced (in case of loop) */
	int advance(int p_frames);
	void set_mix_rate(float p_mix_rate);
	float get_mix_rate();
	
	Tick get_current_tick_from();
	Tick get_current_tick_to();
	
	Tick get_playback_pos_tick();
	
	unsigned int get_song_playback_time();
	
	SongPlayback(GlobalProperties *p_properties);
	~SongPlayback();

};

}

#endif
