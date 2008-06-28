//
// C++ Interface: reverb_plugin
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDREVERB_PLUGIN_H
#define RESHAKEDREVERB_PLUGIN_H


#include "engine/sound_plugin.h"
#include "dsp/reverb.h"

namespace ReShaked {

/**
	@author red <red@killy>
 */
class ReverbPlugin : public SoundPlugin {
			    
	AudioPlug *input_plug;
	AudioPlug *output_plug;
	
	LocalProperty predelay;   
	LocalProperty predelay_fb;   
	LocalProperty hpf;
	LocalProperty room_size;   
	LocalProperty damping;
	LocalProperty spread;
	LocalProperty dry;   
	LocalProperty wet;
	
	Reverb *reverb;
	
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

	ReverbPlugin(const SoundPluginInfo *p_info,int p_channels);
	~ReverbPlugin();

};

}

#endif
