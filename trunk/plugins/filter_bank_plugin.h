//
// C++ Interface: filter_bank_plugin
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDFILTER_BANK_PLUGIN_H
#define RESHAKEDFILTER_BANK_PLUGIN_H

#include "engine/sound_plugin.h"
#include "dsp/filter.h"

namespace ReShaked {

/**
	@author red <red@killy>
 */
class FilterBankPlugin : public SoundPlugin {
			    
	
	enum {
		
		MAX_FILTERS=4,
		MAX_STAGES=4,
		INTERNAL_BUFFER_SIZE=1024
	};
	
	AudioPlug *input_plug;
	AudioPlug *output_plug;
	
	struct FilterData {

		LocalProperty active;   

		LocalProperty cutoff;   
		LocalProperty resonance;   
		OptionsProperty type;
		OptionsProperty stages;
	
		Filter::Coeffs prev_coeffs;
		Filter::Mode prev_mode;
		int prev_stages;
		
		struct History {
			
			struct Stage {
			
				float ha1,ha2,hb1,hb2; //history			
			} stage[MAX_STAGES];
		};
		
		std::vector<History> history;
		
	} filter[MAX_FILTERS];
	
	LocalProperty cutoff_offset;   
	
	bool force_new_coeffs;
	float mix_rate;
	std::vector<Property*> property_list;
	
	float *internal_buffer;
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
	void reset();
	
	/* Processing */
	void process(int p_frames);
	
	

	FilterBankPlugin(const SoundPluginInfo *p_info,int p_channels);
	~FilterBankPlugin();

};

}

#endif
