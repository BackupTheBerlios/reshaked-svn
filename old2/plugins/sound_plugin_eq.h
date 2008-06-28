//
// C++ Interface: sound_plugin_eq
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSOUND_PLUGIN_EQ_H
#define RESHAKEDSOUND_PLUGIN_EQ_H

#include "engine/sound_plugin.h"
#include "dsp/eq.h"

namespace ReShaked {

/**
	@author red <red@killy>
 */
class SoundPluginEQ : public SoundPlugin {

	AudioPlug *input_plug;
	AudioPlug *output_plug;
	
	
	struct Band {
		
		LocalProperty gain;
		std::vector<EQ::BandProcess> channel_process; 	
		
	};
	
	Band *band; //bands
	int band_count;
	
	EQ eq;
	
public:

	enum CreateMode {
		CREATE_EQ_6,
		CREATE_EQ_10,
	};

	static const SoundPluginInfo *create_info_6();
	static const SoundPluginInfo *create_info_10();

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

	SoundPluginEQ(const SoundPluginInfo *p_info,int p_channels,CreateMode p_create_mode);
	~SoundPluginEQ();

};

}

#endif
