//
// C++ Interface: sample_trigger
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSAMPLE_TRIGGER_H
#define RESHAKEDSAMPLE_TRIGGER_H

#include "engine/sound_plugin.h"
#include "dsp/midi_synth.h"

#include "dsp/sample.h"	
#include "dsp/resample.h"
#include "engine/audio_ring_buffer.h"	


namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
 */
class SampleTrigger : public SoundPlugin {
				       
	
	enum {
		
		
		FRAC_BITS=12, //work with 12 bits of fixed point precision
		MAX_CHANS=4,
				
		FADEOUT_BUFFER_LEN=1024, //fadeout for declicker
		DECLICK_RING_BUFFER_LEN=8192, //ringbuffer to mix back declicks
	};
	
	AudioPlug *output_plug;
	
	struct Params {
		NoteProperty note;	    
		LocalProperty pan;		    
		LocalProperty depth;		    
		LocalProperty release;		    
		LocalProperty gain;		    
	} param;			
	
	Sample sample;
	
	ResampleData rd;
	bool sample_active;
	
	float velocity;
	float release_env;
	
	float prev_vol[MAX_CHANS];
	bool is_off;
	bool first_mix;
	
	void reset();		
	
	AudioBuffer fadeout_buffer;
	AudioRingBuffer declick_ring_buffer;
	
	float mix_rate;
	
	void process_to_buffer(int p_frames,AudioBuffer *p_buffer);
	void set_volumes_from_pan(float *p_to);
	
	void save(TreeSaver *p_saver);	
	void load(TreeLoader *p_loader);	
	
	std::vector<Property*> property_list;
public:

	static const SoundPluginInfo *create_info();
	/* Plugs */	

	int get_input_plug_count();
	int get_output_plug_count();
	AudioPlug *get_input_plug(int p_index);
	AudioPlug *get_output_plug(int p_index);


	/* Port Info */
	int get_port_count();
	Property& get_port(int p_index);
	PortType get_port_type(int p_index);


	/* Setting up */
	void set_mixing_rate(float p_mixing_rate);

	/* Processing */
	void process(int p_frames);

	Sample& get_sample();


	SampleTrigger(const SoundPluginInfo *p_info,int p_channels);
	~SampleTrigger();

};

}


#endif
