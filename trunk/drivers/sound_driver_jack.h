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

#include "version.h"

#ifdef DRIVER_JACK_ENABLED


#include <jack/jack.h>
#include "engine/sound_driver.h"
#include <unistd.h>

namespace ReShaked {



/**
	@author red <red@killy>
*/
class SoundDriver_JACK : public SoundDriver {
	

	jack_client_t *client;
	
	static int process_jack_callback(jack_nframes_t nframes,void *arg);
	static int jack_set_mixfreq(jack_nframes_t nframes,void *arg);
	static int jack_set_buffsize(jack_nframes_t nframes,void *arg);
	static void jack_shutdown(void *arg);
	
	String last_error;
	
	float mixing_frequency;
	
	struct PortList {
		
		std::vector<jack_port_t*> ports;
	};
	
	std::vector<PortList> input_port_list;
	std::vector<PortList> output_port_list;
	
	bool active;
	int process_offset;
	int process_nframes;
	void process(int p_frames);
	
	
	virtual void write_to_inputs(int p_frames);
	virtual void read_from_outputs(int p_frames);
	virtual bool is_input_enabled(int p_input);
	virtual bool is_output_enabled(int p_output);

public:
	
	
	int get_settings_count();
	Property *get_setting(int p_setting);
	
	bool uses_file();
	String get_file();
	void set_file(String p_file);
	
	String get_name();
	float get_mix_rate();
	
	bool init();
	void finish();
	String get_error();

	int get_output_latency();	
	
	Status get_status();
	
	SoundDriver_JACK();
	~SoundDriver_JACK();

};

}


#endif


#endif
