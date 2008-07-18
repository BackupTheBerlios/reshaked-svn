//
// C++ Implementation: audio_driver
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "audio_driver.h"

AudioDriver * AudioDriver::_drivers[_MAX_DRIVERS];
int AudioDriver::_driver_count=0;
int AudioDriver::_current=-1;
int AudioDriver::_next=-1;

void AudioDriver::initialize_audio(int p_driver_index) {

	ERR_FAIL_INDEX( p_driver_index, _driver_count );	
	ERR_FAIL_COND( _current != -1);	

	_current=p_driver_index;
	_next=p_driver_index;
	
	_drivers[_current]->initialize();

}

int AudioDriver::get_current_driver_index() {

	return _current;
}

void AudioDriver::set_next_driver_index(int p_next) {

	ERR_FAIL_INDEX( p_next, _driver_count );	
	_next=p_next;
}

int AudioDriver::get_next_driver_index(int p_next) {

	return _next;
}


AudioDriver* AudioDriver::get_current_driver() {

	ERR_FAIL_INDEX_V( _current, _driver_count, NULL ); 
	
	return _drivers[ _current ];
}


void AudioDriver::add_audio_driver( AudioDriver * p_audio_driver ) {

	ERR_FAIL_COND( _driver_count == _MAX_DRIVERS );
	ERR_FAIL_COND( _current != -1 ); // audio system is initialized, no adding drivers allowed
	_drivers[ _driver_count++ ] = p_audio_driver;
}

AudioDriver::AudioDriver()
{
}


AudioDriver::~AudioDriver()
{
}


