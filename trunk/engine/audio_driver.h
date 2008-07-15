//
// C++ Interface: audio_driver
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef AUDIO_DRIVER_H
#define AUDIO_DRIVER_H


#include "engine/audio_node.h"

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class AudioDriver {

	enum {
		_MAX_DRIVERS=8
	};
	
	static AudioDriver * _drivers[_MAX_DRIVERS];
	static int _driver_count;
	static int _current;
	static int _next;


public:

	/* Driver Manager API */

	static void initialize_audio(int p_driver_index);

	static int get_current_driver_index();
	static void set_next_driver_index(int p_next);
	static int get_next_driver_index(int p_next);

	static AudioDriver* get_current_driver();

	static void add_audio_driver( AudioDriver * p_audio_driver );

	/* Driver API */

	virtual String get_name()=0; // driver name
	virtual void initialize()=0; // register the audionodes
		
	virtual bool restart()=0; // restart driver (and reconnect to everything)
	virtual bool is_active() const=0;
	
	virtual int get_param_count()=0;
	virtual ControlPort *get_param(int p_param)=0;
	
	virtual void lock()=0;
	virtual void unlock()=0;

	AudioDriver();	
	virtual ~AudioDriver();

};

#endif
