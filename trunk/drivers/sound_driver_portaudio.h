//
// C++ Interface: sound_driver_portaudio
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSOUND_DRIVER_PORTAUDIO_H
#define RESHAKEDSOUND_DRIVER_PORTAUDIO_H

#include "version.h"


#ifdef DRIVER_PORTAUDIO_ENABLED
#include <vector>
#include "typedefs.h"
#include "drivers/portaudio/portaudio.h"
#include "engine/sound_driver.h"
namespace ReShaked {

/**
	@author red <red@killy>
*/
class SoundDriver_PortAudio : public SoundDriver {
	
	PaStream *stream;
	
	
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
	
	
	static int static_callback( const void *input, void *output, unsigned long frameCount, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData );
	void callback(float *in_buffer,float*out_buffer, int p_bufferSize);


	std::vector<Property*> settings;
	
	int process_offset;
	float *current_input;
	
	float *current_output;
	
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
	
	
	SoundDriver_PortAudio();
	~SoundDriver_PortAudio();
	
	static bool initialize_portaudio();
	static void finalize_portaudio();
	

};

}

#endif
#endif //PORTAUDIO_ENABLED
