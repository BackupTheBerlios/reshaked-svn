//
// C++ Implementation: midi_driver_list
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "midi_driver_list.h"
#include "sound_driver_list.h"
#include "audio_control.h"

namespace ReShaked {

MidiDriverList *MidiDriverList::singleton=NULL;


MidiDriverList* MidiDriverList::get_singleton() {
	
	return singleton;
}
void MidiDriverList::add_driver(MidiDriver* p_driver) {
	
	driver_list.push_back(p_driver);
	current=driver_list.size()-1;
	if (song_playback)
		p_driver->set_song_playback(song_playback);
}

void MidiDriverList::reset_driver() {
	
	init_driver( current );
}

bool MidiDriverList::init_driver(int p_index,bool p_internal_init) {
	
	ERR_FAIL_COND_V(song_playback==NULL,true);
	
	if (p_index==-1)
		p_index=current;
	
	ERR_FAIL_INDEX_V(p_index,driver_list.size(),true);
	
	AudioControl::mutex_lock();
	
	
	if (driver_list[current]->is_active()) {
		
		driver_list[current]->finish();
	}
	
	current=p_index;
	
	if (p_internal_init)
		driver_list[current]->init();
	
	
	AudioControl::mutex_unlock();
	
	return (p_internal_init && !driver_list[current]->is_active());
	
}


void MidiDriverList::finish_driver() {
	ERR_FAIL_COND(song_playback==NULL);
	if (!is_current_driver_active())
		return;
	
	AudioControl::mutex_lock();
	driver_list[current]->finish();
	AudioControl::mutex_unlock();
	
}

bool MidiDriverList::is_current_driver_active() {
	
	ERR_FAIL_INDEX_V(current,driver_list.size(),false);
	return (driver_list[current]->is_active());
}

int MidiDriverList::get_driver_count() {
	
	return driver_list.size();
}	
String MidiDriverList::get_driver_name(int p_driver) {
	
	ERR_FAIL_INDEX_V(p_driver,driver_list.size(),"");
	return driver_list[p_driver]->get_name();
}


int MidiDriverList::get_current_driver() {
	
	return current;
}
void MidiDriverList::set_song_playback(SongPlayback *p_song_playback) {
	
	song_playback=p_song_playback;	
	for (int i=0;i<driver_list.size();i++)
		driver_list[i]->set_song_playback(song_playback);
}
MidiDriver *MidiDriverList::get_driver(int p_index) {
	
	if (p_index==-1)
		p_index=current;
	ERR_FAIL_INDEX_V(p_index,driver_list.size(),NULL);
	return driver_list[p_index];	
	
}

MidiDriverList::MidiDriverList() {
	
	current=-1;
	song_playback=NULL;
	singleton=this;
	
}


MidiDriverList::~MidiDriverList() {
	
}


}
