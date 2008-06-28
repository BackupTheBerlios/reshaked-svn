//
// C++ Interface: sinth
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSINTH_H
#define RESHAKEDSINTH_H

#include "engine/sound_plugin.h"
#include "dsp/midi_synth.h"
#include "dsp/oscillator.h"
#include "dsp/adsr.h"
#include "dsp/filter.h"

namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
 */
class Sinth : public SoundPlugin {
				       
	AudioPlug *output_plug;
	
	
	enum {
		/* Fixed Point */	
		FRAC_SIZE=20,
		FRAC_LEN=(1<<20),
		FRAC_MASK=FRAC_LEN-1
	};
public:	//weird gcc template error forced me to this, I think it's a bug
	enum ModulationMode {
		MOD_ADD,
		MOD_RING,
		MOD_FM
	};	
private:
	class Data;
	
	class Voice : public MidiSynth::Voice {
		
		
		bool done;
		
		Uint64 cycles_osc_1;
		Uint64 cycles_osc_2;
		float prev_gain_osc_1;
		float prev_gain_osc_2;
		float prev_gain_global;
	
		Data *data;
		
		ADSR adsr_osc1;
		ADSR adsr_osc2;
		
		bool first_mix;
		
		Property *release;		    
		
		struct AnalogFilter {		
		
			float ha1,ha2,hb1,hb2; //history
			Filter::Coeffs last_coeffs;
			bool enabled;
			Filter::Mode mode;
			
			double env_pos;
			double attack_env_mult;
			double release_env_mult;
			double release_end_pos;
			
			Uint64 lfo_count;
			
		} filter;

		
		
		
		template<ModulationMode t_mode>
		void process_template(int p_frames);
	public:
	
	
		virtual void event(Event p_event);	
		virtual void process(int p_frames);	
	
		virtual float priority(); ///< 1 is max priority, 0 means it finished
	
		Voice(Data *p_data);
	};
	
	struct Data {
	
		struct OSC {
			
			Oscillator osc;
			
			LocalProperty env_attack;
			LocalProperty env_decay;
			LocalProperty env_sustain;
			LocalProperty env_release;
			
			LocalProperty tune_coarse;
			LocalProperty tune_fine;
		} osc[2];
		
		
		struct Mod {
		
			OptionsProperty mode;
			LocalProperty amount;
			
		} modulation;
		
		struct Filt {
		
			OptionsProperty type;
			LocalProperty cutoff;
			LocalProperty resonance;
			
			LocalProperty env_attack_enabled;
			LocalProperty env_attack_time;
			LocalProperty env_attack_level;
			
			LocalProperty env_release_enabled;
			LocalProperty env_release_time;
			LocalProperty env_release_level;
			
			LocalProperty lfo_rate;
			LocalProperty lfo_depth;
			
			
		} filter;
		
	} data;
	
	MidiSynth *midi_synth;
	
	void reset();		
	
	void save(TreeSaver *p_saver);	
	void load(TreeLoader *p_loader);	
		
public:
	
	Oscillator* get_osc(int p_osc);

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


	Sinth(const SoundPluginInfo *p_info,int p_channels);
	~Sinth();

};

}

#endif
