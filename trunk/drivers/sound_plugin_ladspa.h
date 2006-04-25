//
// C++ Interface: sound_plugin_ladspa
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSOUND_PLUGIN_LADSPA_H
#define RESHAKEDSOUND_PLUGIN_LADSPA_H

#include "version.h"
#ifdef LADSPA_ENABLED
#include "engine/sound_plugin.h"
#include "drivers/ladspa.h"

namespace ReShaked {

/**
	@author red <red@killy>
 */
class SoundPlugin_LADSPA : public SoundPlugin {
			    
		
	std::vector<LADSPA_Handle> instances;
		
	struct SharedProperty {
		
		PropertyVarPtr<float> property;
		float value; //shared by plugin and property
		
		float freq_adj; //for gayfrequency ports
		bool use_freq_adj;
		
		SharedProperty() { value=0; freq_adj=0; use_freq_adj=false; }
		
	};
		
	
	
	std::vector<SharedProperty*> input_ports;
	std::vector<SharedProperty*> output_ports;
	
	std::vector<AudioPlug*> input_plugs;
	std::vector<AudioPlug*> output_plugs;
	
	bool stereo_mode;
	void *library;
	const LADSPA_Descriptor *descriptor;
	bool working;
	
	float mix_freq;
	
	void connect_audio_ports();
public:

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

	void reset();
	
	SoundPlugin_LADSPA(const SoundPluginInfo *p_info,String p_path,int p_index,int p_channels);
	~SoundPlugin_LADSPA();

};

}

#endif
#endif
