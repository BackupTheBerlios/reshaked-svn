//
// C++ Interface: compressor_plugin
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDCOMPRESSOR_PLUGIN_H
#define RESHAKEDCOMPRESSOR_PLUGIN_H

#include "engine/sound_plugin.h"

namespace ReShaked {

/**
	@author red <red@killy>
 */
class CompressorPlugin : public SoundPlugin {
			    
	enum {
		
		MAX_AVG_BUFFER_MS=100
	};
	
	
	OptionsProperty mode;
	LocalProperty pre_gain;
	LocalProperty threshold;
	LocalProperty ratio;
	LocalProperty response;
	LocalProperty post_gain;
	int current_avg_len;
	
	float *avg_buffer;
	int avg_size;
	int avg_pos;
	double avg; //helps precision
	
	AudioPlug *input_plug;
	AudioPlug *sidechain_plug;
	AudioPlug *output_plug;
	
	float mix_rate;
	
	std::vector<Property*> property_list;
	
	void process_1(int p_frames);
	void process_2(int p_frames);
	void process_4(int p_frames);
	
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

	CompressorPlugin(const SoundPluginInfo *p_info,int p_channels);
	~CompressorPlugin();

};

}

#endif
