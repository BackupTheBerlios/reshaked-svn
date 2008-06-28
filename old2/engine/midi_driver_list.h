//
// C++ Interface: midi_driver_list
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDMIDI_DRIVER_LIST_H
#define RESHAKEDMIDI_DRIVER_LIST_H

#include "engine/midi_driver.h"
#include <vector>


namespace ReShaked {

/**
	@author red <red@killy>
*/
class MidiDriverList{
	
	static MidiDriverList *singleton;
	SongPlayback *song_playback;
	std::vector<MidiDriver*> driver_list;
	int current;	
public:
	
	static MidiDriverList* get_singleton();
	
	void add_driver(MidiDriver* p_driver);
	bool init_driver(int p_index=-1,bool p_internal_init=true); //-1 for current
	void finish_driver();
	void reset_driver();
	

	int get_driver_count();
	String get_driver_name(int p_driver);
	MidiDriver *get_driver(int p_index=-1); //get current by default
	int get_current_driver();
	bool is_current_driver_active();
	
	void set_song_playback(SongPlayback *p_song_playback);
	
	MidiDriverList();
	~MidiDriverList();

};

}

#endif
