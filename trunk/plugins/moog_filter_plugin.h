//
// C++ Interface: moog_filter_plugin
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDMOOG_FILTER_PLUGIN_H
#define RESHAKEDMOOG_FILTER_PLUGIN_H

#include "engine/sound_plugin.h"

namespace ReShaked {

/**
	@author red <red@killy>
 */
class MoogFilterPlugin : public SoundPlugin {
			    
	AudioPlug *input_plug;
	AudioPlug *output_plug;
	
	LocalProperty cutoff;		    
	LocalProperty resonance;
	LocalProperty type;
	
	std::vector<Property*> property_list;
	
	struct History  {
		
		float b0, b1, b2, b3, b4; //history for each 1pole
		void clear() { b0=b1=b2=b3=b4=0; }
		History() { clear(); }
	};
	
	History *h;
	
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
	void reset();

	/* Processing */
	void process(int p_frames);

	MoogFilterPlugin(const SoundPluginInfo *p_info,int p_channels);
	~MoogFilterPlugin();

};

}

#endif
