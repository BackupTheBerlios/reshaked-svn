//
// C++ Interface: chionic_params
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDCHIONIC_PARAMS_H
#define RESHAKEDCHIONIC_PARAMS_H

#include "dsp/sample.h"
#include "dsp/lfo.h"
#include "dsp/envelope.h"
#include "property.h"
#include "value_stream.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
class ChionicParams {
public:
	enum {
		
		MAX_LAYERS=6
	};
	
	struct Source {
		
		enum Type {
			TYPE_SAMPLE,
			TYPE_OSC
		};
		
		Type type;
		
		Sample *sample; ///<sample, NULL if source is not sample
//		Ocillator *osc;///<oscillator, NULL if source is not oscillator
	};
	
	struct Global {
		
		std::vector<Source> sources;
		
		struct Volume {
			
			LocalProperty send;
			LocalProperty sens;
			
			
		} volume;
		
		struct Pan {
			
			LocalProperty pos;
			LocalProperty pitch_scale;
			LocalProperty pitch_scale_center;
			
		} pan;
		
		struct Pitch {
			
			LocalProperty tune_coarse;
			LocalProperty tune_fine;
	
		} pitch;
		
	};
	
	struct Layer {
		
		struct Regions {
			
			struct Blending {
				
				
				LocalProperty enabled;
				LocalProperty curve;
			} blending;
			
			struct Region {
				
				signed char base_note; //- 0.. 127 -1 is disabled */
				int source_idx;
			};
			
			ValueStream<char,Region> map;
			

		} regions;
		
		struct Parameters {
			
			
			struct Volume {
				
				LocalProperty send;
				
				LocalProperty velocity_range_begin;
				LocalProperty velocity_range_end;
				
				LocalProperty velocity_range_fade;
				
				LocalProperty pitch_scale;
				
				LocalProperty random;
				
				Envelope envelope;
				LFO lfo;
			} volume;
			
			struct Filter {
				
				LocalProperty type;
				
				LocalProperty cutoff;
				LocalProperty resonance;

				LocalProperty cutoff_sens;
				LocalProperty resonance_sens;
				
				LocalProperty freq_tracking;
				
				LocalProperty random;
				
				Envelope envelope;
				LFO lfo;
				
			} filter;
			
			struct Pan {
				
				
				LocalProperty pos;
				
				LocalProperty random;
				
				Envelope envelope;
				LFO lfo;
				
			} pan;
			
			struct Pitch {
				
				LocalProperty tune_coarse;
				LocalProperty tune_fine;
				
				Envelope envelope;
				LFO lfo;
				
			} pitch;
			
			struct Time {
				
				LocalProperty delay;
				LocalProperty random;

			} time;
			
			
		} params;
		
		enum Mode {
			MODE_OFF,
			MODE_ON_ADD,
			MODE_RING,
			MODE_PM
		};
		
		Mode mode;
		std::vector<char> modulation; //list of voices to where this one writes! MAX_LAYERS means it writes to output
		
		
	} layer[MAX_LAYERS];
	
	
	
	ChionicParams();
	~ChionicParams();
};

}

#endif