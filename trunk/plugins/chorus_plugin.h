//
// C++ Interface: chorus_plugin
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDCHORUS_PLUGIN_H
#define RESHAKEDCHORUS_PLUGIN_H

#include "engine/sound_plugin.h"

namespace ReShaked {

/**
	@author red <red@killy>
 */
class ChorusPlugin : public SoundPlugin {
			    
	
	enum {
		
		MAX_DELAY_MS=50,
		MAX_DEPTH_MS=20,
		MAX_WIDTH_MS=50,
		MAX_VOICES=1,
		CYCLES_FRAC=16,
		CYCLES_MASK=(1<<CYCLES_FRAC)-1
	};
	
	
	AudioPlug *input_plug;
	AudioPlug *output_plug;
	
	struct Voice {
	
		LocalProperty delay;
		LocalProperty rate;
		LocalProperty depth;
		LocalProperty level;
		LocalProperty width;
		LocalProperty cutoff;
		LocalProperty pan;
		LocalProperty pan_depth;
		
		unsigned long long cycles;
		std::vector<float> filter_h;
		
	} voice[MAX_VOICES];
		
	LocalProperty wet;
	LocalProperty dry;
	
	std::vector<Property*> properties;
	
	AudioBuffer ring_buffer;
	
	unsigned int buffer_pos;
	unsigned int buffer_mask;
	
	float mix_rate;
	
	void process_stereo(int p_frames);
	void process_quad(int p_frames);
	
public:
	
	void reset();
	
	
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

	ChorusPlugin(const SoundPluginInfo *p_info,int p_channels);
	~ChorusPlugin();

};

}

#endif
