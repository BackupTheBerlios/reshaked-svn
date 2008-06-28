//
// C++ Interface: amplifier_plugin
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDAMPLIFIER_PLUGIN_H
#define RESHAKEDAMPLIFIER_PLUGIN_H

#include "engine/sound_plugin.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
class AmplifierPlugin : public SoundPlugin {
			    
	AudioPlug *input_plug;
	AudioPlug *output_plug;
	
	LocalProperty gain;		    
	LocalProperty signal_energy;
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
	
	AmplifierPlugin(const SoundPluginInfo *p_info,int p_channels);
	~AmplifierPlugin();

};

}

#endif
