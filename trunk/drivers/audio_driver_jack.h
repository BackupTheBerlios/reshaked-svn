//
// C++ Interface: audio_driver_jack
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef AUDIO_DRIVER_JACK_H
#define AUDIO_DRIVER_JACK_H

#include "engine/audio_driver.h"
#include <vector>
#include <list>
#include <jack/jack.h>
#include <jack/midiport.h>
#include <pthread.h>
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

class AudioDriverNodeJACK;
class AudioDriverJACK : public AudioDriver {
friend class AudioDriverNodeJACK;

	static int _process(jack_nframes_t nframes, void *arg) { 
		AudioDriverJACK *adj=(AudioDriverJACK*)arg; return adj->process(nframes);
	}
	static int _srate(jack_nframes_t nframes, void *arg) { 
		AudioDriverJACK *adj=(AudioDriverJACK*)arg; return adj->set_mix_rate(nframes);
	}
	static void _shutdown(void *arg) { 
		AudioDriverJACK *adj=(AudioDriverJACK*)arg; return adj->finish();
	}

	int process(jack_nframes_t nframes);
	int set_mix_rate(jack_nframes_t nframes);
	
	static AudioDriverJACK*singleton;
	String error_text;

	std::list< AudioDriverNodeJACK* > audio_nodes;
	jack_client_t *client;
	jack_nframes_t callback_nframes;
	
	int process_offset;
	
	pthread_mutex_t mutex;
	
	void finish();
	
public:

	virtual String get_name(); 
	virtual void initialize(); 
		
	virtual bool restart(); 
	virtual bool is_active() const;
	
	virtual int get_param_count();
	virtual ControlPort *get_param(int p_param);
	
	virtual void lock();
	virtual void unlock();

	AudioDriverJACK();	
	~AudioDriverJACK();

};

#endif
