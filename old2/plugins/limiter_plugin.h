//
// C++ Interface: limiter_plugin
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDLIMITER_PLUGIN_H
#define RESHAKEDLIMITER_PLUGIN_H

#include "engine/sound_plugin.h"

namespace ReShaked {

/**
	@author red <red@killy>
 */
class LimiterPlugin : public SoundPlugin {
			    
	
	enum {
		
		MAX_DELAY_MS=20
	};
	
	AudioPlug *input_plug;
	AudioPlug *output_plug;
	
	LocalProperty pre_gain;		    
	LocalProperty threshold;
	LocalProperty attack;
	LocalProperty release;
	LocalProperty post_gain;
	
	float current_gain;
	float target_gain;
	float increment;
	
	AudioBuffer ring;
	unsigned int ring_pos;
	unsigned int ring_mask;
	
	float mix_rate;
	
	std::vector<Property*> property_list;
	

	void process_1(float p_frames);
	void process_2(float p_frames);
	void process_4(float p_frames);
	
	float max_val;
	bool peaked;
	
public:

	float get_max_val();
	bool has_peaked();
	void clear_peaked();
	
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
	void reset();
	/* Processing */
	void process(int p_frames);
	

	LimiterPlugin(const SoundPluginInfo *p_info,int p_channels);
	~LimiterPlugin();

};

}

#endif
