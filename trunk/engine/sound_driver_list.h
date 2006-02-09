//
// C++ Interface: sound_driver_list
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSOUND_DRIVER_LIST_H
#define RESHAKEDSOUND_DRIVER_LIST_H

#include "engine/sound_driver.h"
#include "engine/song.h"

namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class SoundDriverList {
	
	static SoundDriverList *singleton;
	Song *song;
	std::vector<SoundDriver*> driver_list;
	int current;
public:
	static SoundDriverList* get_singleton();
	
	void add_driver(SoundDriver* p_driver);
	bool init_driver(int p_index=-1,bool p_internal_init=true); //-1 for current
	void finish_driver();
	void reset_driver();
	

	int get_driver_count();
	String get_driver_name(int p_driver);
	SoundDriver *get_driver(int p_index);
	int get_current_driver();
	bool is_current_driver_active();
	
	float get_mix_rate();	
	int get_output_latency();
	
	void set_song(Song *p_song);

    	SoundDriverList();

    	~SoundDriverList();

};

}

#endif
