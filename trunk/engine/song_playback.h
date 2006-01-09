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
	float sampling_rate;
	Tick current_tick;
	
	struct Loop {
		bool active;
		int begin,end; //beats
	} loop;
			    
public:
	
	void set_loop(int p_begin,int p_end);
	void set_sampling_rate(float p_sampling_rate);
	
	void set_play_pos(Tick p_tick);
	
	void set_play_status(Status p_status);
	
	SongPlayback();
	~SongPlayback();

};

}

#endif
