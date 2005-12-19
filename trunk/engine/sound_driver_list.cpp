//
// C++ Implementation: sound_driver_list
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sound_driver_list.h"
#include "audio_control.h"

namespace ReShaked {

SoundDriverList *SoundDriverList::singleton=NULL;

SoundDriverList* SoundDriverList::get_singleton() {
	
	return singleton;
}
void SoundDriverList::add_driver(SoundDriver* p_driver) {
	
	driver_list.push_back(p_driver);
	current=driver_list.size()-1;
}
void SoundDriverList::init_driver(int p_index,bool p_internal_init) {
	
	ERR_FAIL_COND(song==NULL);
	
	if (p_index==-1)
		p_index=current;
	
	ERR_FAIL_INDEX(p_index,driver_list.size());
	
	AudioControl::mutex_lock();
	
	if (driver_list[current]->get_status()!=SoundDriver::STATUS_DISABLED) {
		
		driver_list[current]->finish();
	}
	
	current=p_index;
	
	driver_list[current]->set_port_layout(&song->get_audio_port_layout());
	song->set_input_node( driver_list[current]->get_input_node() );
	song->set_output_node( driver_list[current]->get_output_node() );
	
	if (p_internal_init)
		driver_list[current]->init();
		
	song->set_mix_rate( driver_list[current]->get_mix_rate() );
	
	AudioControl::mutex_unlock();
	
}
void SoundDriverList::finish_driver() {
	ERR_FAIL_COND(song==NULL);
	ERR_FAIL_COND(is_current_driver_active());
	
	AudioControl::mutex_lock();
	driver_list[current]->finish();
	AudioControl::mutex_unlock();
	
}

bool SoundDriverList::is_current_driver_active() {
	
	ERR_FAIL_INDEX_V(current,driver_list.size(),false);
	return (driver_list[current]->get_status()!=SoundDriver::STATUS_DISABLED);
}

int SoundDriverList::get_driver_count() {
	
	return driver_list.size();
}	
String SoundDriverList::get_driver_name(int p_driver) {
	
	ERR_FAIL_INDEX_V(p_driver,driver_list.size(),"");
	return driver_list[p_driver]->get_name();
}


int SoundDriverList::get_current_driver() {
	
	return current;
}
void SoundDriverList::set_song(Song *p_song) {
	
	song=p_song;	
}

SoundDriverList::SoundDriverList() {
	
	current=-1;
	song=NULL;
	singleton=this;
}


SoundDriverList::~SoundDriverList() {
	
}


}
