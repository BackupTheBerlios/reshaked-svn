//
// C++ Interface: distortion_plugin
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDDISTORTION_PLUGIN_H
#define RESHAKEDDISTORTION_PLUGIN_H

#include "engine/sound_plugin.h"

namespace ReShaked {

/**
	@author red <red@killy>
 */
class DistortionPlugin : public SoundPlugin {
			    
	enum {
		
		OVERSAMPLE=8,
		MAX_LEN=1024,
	};
	
	enum {
		TYPE_CLIP,
		TYPE_ATAN,
		TYPE_DECIMATE,
		TYPE_OVERDRIVE,
		TYPE_WAVESHAPE_1,
	};
	
	AudioPlug *input_plug;
	AudioPlug *output_plug;
	
	LocalProperty pre_gain;
	OptionsProperty type;
	LocalProperty pre_lpf;
	LocalProperty drive;
	
	float *pre;
	float *h; //filter h
	
	std::vector<Property*> property_list;
	
	AudioBuffer buff;
	
	float mix_rate;
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

	DistortionPlugin(const SoundPluginInfo *p_info,int p_channels);
	~DistortionPlugin();

};

}

#endif
