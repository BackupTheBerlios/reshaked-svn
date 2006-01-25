//
// C++ Interface: sine_synth
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSINE_SYNTH_H
#define RESHAKEDSINE_SYNTH_H

#include "engine/sound_plugin.h"
#include "dsp/midi_synth.h"


namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class SineSynth : public SoundPlugin {
				       
	AudioPlug *output_plug;
	
	class Voice : public MidiSynth::Voice {
		
		
		float amp,amp_decr;
		int ofs;
		
		Property *release;		    
	public:
		
		
		virtual void event(Event p_event);	
		virtual void process(int p_frames);	
		
		
		virtual float priority(); ///< 1 is max priority, 0 means it finished
		
		Voice(Property *p_release);
	};
	
	
	MidiSynth *midi_synth;
		
	LocalProperty release;		    
	
	
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

       
	SineSynth(const SoundPluginInfo *p_info,int p_channels);
	~SineSynth();

};

}

#endif
