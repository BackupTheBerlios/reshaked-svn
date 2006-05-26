//
// C++ Interface: delay_line_effect
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDDELAY_LINE_PLUGIN_H
#define RESHAKEDDELAY_LINE_PLUGIN_H

#include "engine/sound_plugin.h"

namespace ReShaked {

/**
	@author red <red@killy>
 */
class DelayLinePlugin : public SoundPlugin {
			    
	/* A bit memory intensive FX I'm afraid, may need a few megabytes of mem */
	
	enum {
		
		MAX_DELAY_MS=1500,
		MAX_TAPS=2
	};
	
	/* plugs */
	AudioPlug *input_plug;
	AudioPlug *output_plug;
	
	/* controls */
	LocalProperty main_active;
	LocalProperty main_level;
	LocalProperty main_pan;
	LocalProperty main_pan_depth;
	
	LocalProperty tap_1_active;
	LocalProperty tap_1_delay;
	LocalProperty tap_1_level;
	LocalProperty tap_1_pan;
	LocalProperty tap_1_pan_depth;
	
	LocalProperty tap_2_active;
	LocalProperty tap_2_delay;
	LocalProperty tap_2_level;
	LocalProperty tap_2_pan;
	LocalProperty tap_2_pan_depth;
	
	LocalProperty feedback_active;
	LocalProperty feedback_delay;
	LocalProperty feedback_level;
	LocalProperty feedback_lowpass;
	
	
	std::vector<Property*> properties;
	
	/* delay buffer */
	AudioBuffer ring_buffer;
	
	unsigned int ring_buffer_pos;
	unsigned int ring_buffer_mask;
	
	/* feedback buffer */
	AudioBuffer feedback_buffer;
	
	unsigned int feedback_buffer_size;
	unsigned int feedback_buffer_pos;
	
	
	float mix_rate;
	bool bpm_based;
	
public:

	static const SoundPluginInfo *create_info_bpm();
	static const SoundPluginInfo *create_info_msec();

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

	DelayLinePlugin(const SoundPluginInfo *p_info,int p_channels,bool p_bpm_based);
	~DelayLinePlugin();

};

}

#endif
