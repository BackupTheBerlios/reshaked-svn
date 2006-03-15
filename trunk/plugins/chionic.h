//
// C++ Interface: chionic
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDCHIONIC_H
#define RESHAKEDCHIONIC_H

#include "engine/sound_plugin.h"
#include "dsp/midi_synth.h"
#include "plugins/chionic_params.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
class Chionic : public SoundPlugin {
	
	AudioPlug *input_plug;
	AudioPlug *output_plug;
	
	ChionicParams params;
	
//	MidiSynth *midi_synth;
	
	void reset();		
	

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


	ChionicParams* get_params();
	
	Chionic(const SoundPluginInfo *p_info,int p_channels);
	~Chionic();
	

};

}

#endif
