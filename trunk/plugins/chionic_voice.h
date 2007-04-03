//
// C++ Interface: chionic_voice
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDCHIONIC_VOICE_H
#define RESHAKEDCHIONIC_VOICE_H

#include "dsp/midi_synth.h"
#include "plugins/chionic_params.h"
#include "engine/audio_buffer.h"
#include "dsp/filter.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/

struct ChionicVoiceBuffers {
	
	AudioBuffer *input;
	struct Voice {
	
		AudioBuffer buffer;
		
	} voice[ChionicParams::MAX_LAYERS];
};



class ChionicVoice : public MidiSynth::Voice {
	
	enum {
		
		
		FRAC_BITS=12, //work with 12 bits of fixed point precision
		MAX_CHANS=4,
		VEL_SENS_SEMITONES=128
	};
	
	bool done;
		
	double cycles;
	bool first_mix;
	unsigned long usecs;
		
	ChionicParams *params;
	ChionicVoiceBuffers *buffers;		
	
	
	struct Layer {
		
		bool mixed;
		
		int source_idx_below;
		int source_idx_above;
		bool done;
		
		int filter_mode;
		
		struct FilterH {
				
			Filter::Coeffs old;
			float ha1,ha2,hb1,hb2; //history

		} filter[MAX_CHANS];
		
		float current_volumes[MAX_CHANS];
		float current_volumes_incr[MAX_CHANS];
		
		float current_volumes_above[MAX_CHANS];
		float current_volumes_incr_above[MAX_CHANS];
		
		EnvelopeProcess env_vol;
		EnvelopeProcess env_pan;
		EnvelopeProcess env_depth;
		EnvelopeProcess env_cutoff;
		EnvelopeProcess env_reso;
		EnvelopeProcess env_pitch;
		
	} layer[ChionicParams::MAX_LAYERS];
	
	int frames_to_mix;
	double cycles_to_mix;
	float freq_for_mix;
	float mix_rate;
	unsigned long usecs_to_mix;
	
	inline bool process_layer_source(int p_layer,int p_source,float p_note,float p_freq_adjust,float p_gain_adjust);
	inline void set_volumes_from_pan(float p_pan,float p_depth,float *p_to);
	inline void process_layer(int p_layer);
	
public:
	

	virtual void event(Event p_event);	
	virtual void process(int p_frames);	


	virtual float priority(); ///< 1 is max priority, 0 means it finished
	
	ChionicVoice(ChionicParams *p_params,ChionicVoiceBuffers *p_buffers);
	
	~ChionicVoice();

};

}

#endif
