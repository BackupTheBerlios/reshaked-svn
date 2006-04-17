//
// C++ Interface: simpler
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSIMPLER_H
#define RESHAKEDSIMPLER_H

#include "engine/sound_plugin.h"
#include "dsp/midi_synth.h"

#include "dsp/sample.h"	
#include "dsp/adsr.h"	

namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
 */
class Simpler : public SoundPlugin {
				       
	
	enum {
		
		
		FRAC_BITS=12, //work with 12 bits of fixed point precision
		MAX_CHANS=4,
		MAX_ENV_STAGE_SECONDS=9
	};
	
	AudioPlug *output_plug;
	
	struct Data {
		
		Sample sample;
		LocalProperty attack;		    
		LocalProperty decay;		    
		LocalProperty sustain;		    
		LocalProperty release;		    
		
		LocalProperty pan;		    
		LocalProperty depth;		    
		
	};
	
	
	class Voice : public MidiSynth::Voice {
		
		bool done;
		
		double cycles;
		float prev_vol[MAX_CHANS]; //previous volumes
	
		Data *data;
		
		ADSR adsr;
		
		bool first_mix;
		void set_volumes_from_pan(float *p_to);
		
	public:
		
	
		virtual void event(Event p_event);	
		virtual void process(int p_frames);	
	
	
		virtual float priority(); ///< 1 is max priority, 0 means it finished
	
		Voice(Data *data);
	};
	
	Data data;
	
	MidiSynth *midi_synth;
	
	
	void reset();		
	
	void save(TreeSaver *p_saver);	
	void load(TreeLoader *p_loader);	
	
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

	Sample& get_sample();
	
	
	Simpler(const SoundPluginInfo *p_info,int p_channels);
	~Simpler();

};

}


#endif
