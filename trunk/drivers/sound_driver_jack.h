//
// C++ Interface: sound_driver_jack
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSOUND_DRIVER_JACK_H
#define RESHAKEDSOUND_DRIVER_JACK_H

#ifdef DRIVER_JACK_ENABLED

namespace ReShaked {



/**
	@author red <red@killy>
*/
class SoundDriver_JACK{
public:
	
	AudioNode *get_input_node();
	AudioNode *get_output_node();
	
	int get_settings_count();
	Property get_setting();
	
	bool uses_file();
	String get_file();
	void set_file(String p_file);
	
	String get_name();
	
	void init(int p_ports);
	void finish();
	
	Status get_status();
	
	SoundDriver_JACK();
	~SoundDriver_JACK();

};

}


#endif


#endif
