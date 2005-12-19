//
// C++ Interface: sound_driver
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSOUND_DRIVER_H
#define RESHAKEDSOUND_DRIVER_H

class String;
class AudioNode;
class Property;

namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class SoundDriver{
public:
	
	
	enum Status {
		STATUS_DISABLED,
		STATUS_IDLE,
		STATUS_ACTIVE
	};
	
	virtual AudioNode *get_input_node()=0;
	virtual AudioNode *get_output_node()=0;
	
	virtual int get_settings_count()=0;
	virtual Property get_setting()=0;
	
	virtual bool uses_file()=0;
	virtual String get_file()=0;
	virtual void set_file(String p_file)=0;
	
	virtual String get_name()=0;
	
	virtual void init(int p_ports)=0;
	virtual void finish()=0;
	
	virtual Status get_status()=0;
	
	
	virtual ~SoundDriver() {};

};

}

#endif
