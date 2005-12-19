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

#include "typedefs.h"
#include "engine/audio_node.h"
#include "engine/audio_port_layout.h"
#include "property.h"
#include <vector>

namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class SoundDriver{
	
	class InputNode : public AudioNode {
	friend class SoundDriver;
		SoundDriver *driver;
		std::vector<AudioPlug*> input_plugs;
	public:
		int get_input_plug_count();
		int get_output_plug_count();
		AudioPlug *get_input_plug(int p_index);
		AudioPlug *get_output_plug(int p_index);
		
		bool is_input_plug_enabled(int p_idx);
		bool is_output_plug_enabled(int p_idx);
		String get_input_plug_caption(int p_which);
		String get_output_plug_caption(int p_which);
		bool is_system() { return true; }
		String get_caption();
		
		void process(int p_frames);
		
	};
		
	class OutputNode : public AudioNode {
	friend class SoundDriver;
		SoundDriver *driver;
		std::vector<AudioPlug*> output_plugs;
	public:	
		int get_input_plug_count();
		int get_output_plug_count();
		AudioPlug *get_input_plug(int p_index);
		AudioPlug *get_output_plug(int p_index);
		
		bool is_input_plug_enabled(int p_idx);
		bool is_output_plug_enabled(int p_idx);
		String get_input_plug_caption(int p_which);
		String get_output_plug_caption(int p_which);
		bool is_system() { return true; }
		String get_caption();
		
		void process(int p_frames);

	};
	
	InputNode input_node;
	OutputNode output_node;
	const AudioPortLayout *port_layout;
	
protected:	

	
	int get_input_count();
	AudioPlug *get_input_plug(int p_input);
	int get_output_count();
	AudioPlug *get_output_plug(int p_output);
	
	virtual void write_to_inputs()=0;
	virtual void read_from_outputs()=0;
	virtual bool is_input_enabled(int p_input)=0;
	virtual bool is_output_enabled(int p_output)=0;
	
	const AudioPortLayout *get_port_layout();
public:
	
	
	enum Status {
		STATUS_DISABLED,
		STATUS_IDLE,  
		STATUS_ACTIVE
	};
	
	
	virtual int get_settings_count()=0;
	virtual Property *get_setting()=0;
	
	virtual bool uses_file()=0;
	virtual String get_file()=0;
	virtual void set_file(String p_file)=0;
	
	virtual String get_name()=0;
	
	virtual bool init()=0;
	virtual void finish()=0;
	
	virtual Status get_status()=0;
	
	virtual float get_mix_rate()=0;	
	
	
	void set_port_layout(const AudioPortLayout *p_layout);
	AudioNode *get_input_node();
	AudioNode *get_output_node();
	
	
	virtual ~SoundDriver() {};

};

}

#endif
