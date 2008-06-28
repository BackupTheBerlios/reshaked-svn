//
// C++ Interface: freq_splitter_plugin
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDFREQ_SPLITTER_PLUGIN_H
#define RESHAKEDFREQ_SPLITTER_PLUGIN_H


#include "engine/sound_plugin.h"

namespace ReShaked {

/**
	@author red <red@killy>
 */
class FreqSplitterPlugin : public SoundPlugin {
			    
	AudioPlug *input_plug;
	AudioPlug *lpf_output_plug;
	AudioPlug *hpf_output_plug;
	
	LocalProperty pivot;		    
	
	float *h; //filter history
	
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

	FreqSplitterPlugin(const SoundPluginInfo *p_info,int p_channels);
	~FreqSplitterPlugin();

};

}

#endif
