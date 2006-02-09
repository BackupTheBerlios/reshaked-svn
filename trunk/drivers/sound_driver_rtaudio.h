//
// C++ Interface: sound_driver_rtaudio
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSOUND_DRIVER_RTAUDIO_H
#define RESHAKEDSOUND_DRIVER_RTAUDIO_H


#include "version.h"

#ifdef DRIVER_RTAUDIO_ENABLED

#include "engine/sound_driver.h"
#include "drivers/RtAudio.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/



class SoundDriver_RtAudio : public SoundDriver {
	
	RtAudio *rtaudio;
	RtAudio::RtAudioApi api;
	int device;
	String driver_name;
	
	String last_error;
	
	float mixing_frequency;
	
	int current_inputs;
	int current_outputs;
	
	bool active;
	
	virtual void write_to_inputs(int p_frames);
	virtual void read_from_outputs(int p_frames);
	virtual bool is_input_enabled(int p_input);
	virtual bool is_output_enabled(int p_output);
	
	
	std::vector<int> sampling_rate_values;
	
	//map visible devices to real devices
	std::vector<int> input_device_index;
	std::vector<int> output_device_index;
	
	
	OptionsProperty input_devices;
	OptionsProperty output_devices;
	
	OptionsProperty buffer_size;
	OptionsProperty mix_rate;
	LocalProperty fragments;
	
	static int static_callback(char *buffer, int bufferSize, void *p_instance);
	
	void callback(char *buffer, int bufferSize);

	std::vector<Property*> settings;
	
	int process_offset;
	float *current_input;
	
	std::vector<float> output_aux;
public:
	
	String get_last_error();
		
	AudioNode *get_input_node();
	AudioNode *get_output_node();
	
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
	
	SoundDriver_RtAudio(RtAudio::RtAudioApi p_api);

	
	~SoundDriver_RtAudio();

};

}

#endif

#endif
