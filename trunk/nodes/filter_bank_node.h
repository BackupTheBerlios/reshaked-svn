//
// C++ Interface: filter_bank_node
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FILTER_BANK_NODE_H
#define FILTER_BANK_NODE_H


#include "engine/hl_audio_node.h"
#include "dsp/filter.h"
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class FilterBankNode : public HL_AudioNode {

	enum {
		
		MAX_FILTERS=4,
		MAX_STAGES=4,
		INTERNAL_BUFFER_SIZE=1024
	};
		
	struct FilterData {

		HL_ControlPort cutoff;   
		HL_ControlPort resonance;   
		HL_EnumControlPort mode;
		HL_EnumControlPort stages;
	
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
	
	HL_ControlPort cutoff_offset;   
	
	bool force_new_coeffs;	
	
	void reset();
	
	virtual void mix_rate_changed(); 
	template<int STAGES,bool RAMP> 
	static void _process_channel(const sample_t * p_input,sample_t * p_output,int p_frames,Filter::Coeffs &c,Filter::Coeffs &c_inc,FilterData::History::Stage *p_history);

	virtual void process(const ProcessInfo& p_info);


	FilterBankNode(int p_instanced_channels,const AudioNodeInfo *p_info);	
	
public:

	static AudioNode *creation_func(int p_channels,const AudioNodeInfo *p_info);
	static const AudioNodeInfo *get_creation_info();


	~FilterBankNode();

};

#endif
