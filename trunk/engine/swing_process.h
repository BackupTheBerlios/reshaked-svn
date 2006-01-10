//
// C++ Interface: swing_process
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSWING_PROCESS_H
#define RESHAKEDSWING_PROCESS_H


#include "engine/song_playback.h"

namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class SwingProcess {

	SongPlayback *playback;
	
	//Tick last_tick_to;
public:
	Tick get_current_tick_from();
	Tick get_current_tick_to();
	
	SwingProcess(SongPlayback *p_playback);
	~SwingProcess();

};

}

#endif
