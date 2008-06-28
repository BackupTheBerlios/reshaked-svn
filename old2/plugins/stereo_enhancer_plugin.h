//
// C++ Interface: stereo_enhancer_plugin
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSTEREO_ENHANCER_PLUGIN_H
#define RESHAKEDSTEREO_ENHANCER_PLUGIN_H

#include "engine/sound_plugin.h"

namespace ReShaked {

/**
	@author red <red@killy>
 */
class StereoEnhancerPlugin : public SoundPlugin {
			    
	
	enum {
		
		MAX_DELAY_MS=50
	};
	
	AudioPlug *input_plug;
	AudioPlug *output_plug;
	
	LocalProperty pan_pullout;   
	LocalProperty time_pullout;
	LocalProperty surround;
	
	
	float *delay_ringbuff;
	unsigned int ringbuff_pos;
	unsigned int ringbuff_mask;
	
	
	float mix_rate;
	
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
	void reset();

	/* Processing */
	void process(int p_frames);

	StereoEnhancerPlugin(const SoundPluginInfo *p_info,int p_channels);
	~StereoEnhancerPlugin();

};

}


#endif
