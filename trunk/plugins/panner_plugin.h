//
// C++ Interface: panner_plugin
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDPANNER_PLUGIN_H
#define RESHAKEDPANNER_PLUGIN_H

#include "engine/sound_plugin.h"

namespace ReShaked {

/**
	@author red <red@killy>
 */
class PannerPlugin : public SoundPlugin {
			    
	
	enum {
		
		MAX_DELAY_MS=50
	};
	
	AudioPlug *input_plug;
	AudioPlug *output_plug;
	
	LocalProperty pan;   
	LocalProperty depth;
	LocalProperty mode;
	
	
	std::vector<Property*> property_list;
	
	void process_2(int p_frames); //stereo process
	void process_4(int p_frames); //quad process
	
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

	PannerPlugin(const SoundPluginInfo *p_info,int p_channels);
	~PannerPlugin();

};

}

#endif
