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
#include "dsp/oscillator.h"
#include "dsp/lfo.h"
#include "dsp/envelope.h"
#include "dsp/region.h"
#include "property.h"
#include "value_stream.h"


namespace ReShaked {

/**
	@author red <red@killy>
*/
class ChionicParams {
public:
	enum {
		
		MAX_LAYERS=6,
		MAX_VARIATORS=4
	};
	
	struct Source {
		
		enum Type {
			TYPE_SAMPLE,
			TYPE_OSC
		};
		
		Type type;
		
		Sample *sample; ///<sample, NULL if source is not sample
		Oscillator *osc;///<oscillator, NULL if source is not oscillator
		
		String name; //must have a name!
		
		void copy_from(const Source &p_source) { 
			
			clear();
			
			type = p_source.type; 
			name=p_source.name; 
			if (type==TYPE_SAMPLE) { 
				sample = new Sample;
				*sample=*p_source.sample;
				osc=NULL;
			} else {
				
				osc = new Oscillator;
				*osc=*p_source.osc;
				sample=NULL;
	
			}
		}		
				
		void clear() {
			
			if (osc)
				delete osc;
			if (sample)
				delete sample;
			osc=NULL;
			sample=NULL;
			name="";
			
		}
		Source() { type=TYPE_SAMPLE; sample=NULL; osc=NULL; }
	};
	
	struct Global {
		
		std::vector<Source> sources;
		
		struct Volume {
			
			LocalProperty send;
			LocalProperty sens;
			
			
		} volume;
		
		struct Pan {
			
			LocalProperty pos;
			LocalProperty depth;
			LocalProperty pitch_scale;
			NoteProperty pitch_scale_center;
			
		} pan;
		
		struct Pitch {
			
			LocalProperty tune_coarse;
			LocalProperty tune_fine;
	
		} pitch;
		
		//the extra "layer" added is the input/output
		struct ModulationSources {
			
			std::vector<int> read_from; //read from 0 to MAX_LAYERS-1 , a layer, read from MAX_LAYERS means reading from input
		} modulation_source[MAX_LAYERS+1]; // source MAX_LAYERS is the output
		
	} global;
	
	struct Layer {
		
		struct Regions {
			
			struct Blending {
				
				
				bool enabled;
				LocalProperty curve;
			} blending;
			
			RegionMap map;
			

		} regions;
		
		struct Parameters {
			
			
			struct Volume {
				
				LocalProperty send;
				
				LocalProperty velocity_range_begin;
				LocalProperty velocity_range_end;
				
				LocalProperty velocity_range_begin_level;
				LocalProperty velocity_range_end_level;
							
				LocalProperty pitch_scale;
				
				LocalProperty random;
				
				Envelope envelope;
				LFO lfo;
			} volume;
			
			struct Filter {
				
				OptionsProperty type;
				
				LocalProperty cutoff;
				LocalProperty resonance;

				LocalProperty cutoff_velsens;
				LocalProperty resonance_velsens;
				
				LocalProperty freq_tracking;
				
				LocalProperty random;
				
				Envelope envelope_cutoff;
				Envelope envelope_resonance;
				LFO lfo_cutoff;
				LFO lfo_resonance;
				
			} filter;
			
			struct Pan {
				
				
				LocalProperty pos;
				LocalProperty depth; //if working in 4 chans, only
				
				LocalProperty random;
				
				Envelope envelope;
				Envelope envelope_depth;
				LFO lfo;
				LFO lfo_depth;
				
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
		
		enum ModulationMode {
			MODE_OFF,
			MODE_ON_ADD,
			MODE_RING,
			MODE_FM
		};
		
		ModulationMode modulation_mode;
	
		void copy_from(Layer *p_from);
		
		
	} layer[MAX_LAYERS];
	
	
	
	ChionicParams();
	~ChionicParams();
};

}

#endif
