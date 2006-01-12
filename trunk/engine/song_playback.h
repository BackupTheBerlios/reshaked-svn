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
	float mix_rate;
	Tick prev_tick;
	Tick current_tick;
	
	GlobalProperties *properties;
	
	struct Loop {
		bool active;
		Tick begin,end; 
	} loopdata;
	
	Status status;
			    
public:


	
	/* Control */
	void play(Tick p_from_pos=0);
	void loop(Tick p_begin,Tick p_end);
	void set_pause(bool p_paused);
	void stop();
	
	/* Info */
	
	Status get_status();
	
	/* Advance, return frames it actually advanced (in case of loop) */
	int advance(int p_frames);
	void set_mix_rate(float p_mix_rate);
	float get_mix_rate();
	
	Tick get_current_tick_from();
	Tick get_current_tick_to();
	
	SongPlayback(GlobalProperties *p_properties);
	~SongPlayback();

};

}

#endif
