//
// C++ Implementation: filter_bank_node
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "filter_bank_node.h"
#include "dsp/formulas.h"


void FilterBankNode::mix_rate_changed() {

	reset();	
}

void FilterBankNode::reset() {
	
	force_new_coeffs=true;
	
	for (int i=0;i<MAX_FILTERS;i++) {
		
		FilterData &f=filter[i];
		
		for (int j=0;j<f.history.size();j++) {
			
			for (int k=0;k<MAX_STAGES;k++) {
			
				f.history[j].stage[k].ha1=0;
				f.history[j].stage[k].ha2=0;
				f.history[j].stage[k].hb1=0;
				f.history[j].stage[k].hb2=0;
			
			}
		}
	}
}

AudioNode *FilterBankNode::creation_func(int p_channels,const AudioNodeInfo *p_info) {

	return new FilterBankNode(p_channels,p_info);
}


const AudioNodeInfo *FilterBankNode::get_creation_info() {

	static AudioNodeInfo _info;
	_info.caption="Filter Bank";
	_info.short_caption="FilterBank";
	_info.description="Flexible analog-style filterbank.";
	_info.unique_ID="INTERNAL_FilterBankNode";
	_info.creation_func=&FilterBankNode::creation_func;
	_info.icon_string="node_filterbank";
	_info.category="Effects";

	return &_info;
}


template<int STAGES,bool RAMP> 
void FilterBankNode::_process_channel(const sample_t * p_input,sample_t * p_output,int p_frames,Filter::Coeffs &c,Filter::Coeffs &c_inc,FilterData::History::Stage *p_history) {

	FilterData::History::Stage &h1=p_history[0];
	FilterData::History::Stage &h2=p_history[1];
	FilterData::History::Stage &h3=p_history[2];
	FilterData::History::Stage &h4=p_history[3];
	
	while(p_frames--) {
	
		sample_t in=*p_input;
		in+=UNDENORMAL_OFFSET; // minimal error, but avoids filter reaching zero and becoming denormal
		sample_t pre=in;
		
		if (STAGES>=1) {
		
			in = (in * c.b0 + h1.hb1 * c.b1  + h1.hb2 * c.b2 + h1.ha1 * c.a1 + h1.ha2 * c.a2);
			h1.ha2=h1.ha1;	
			h1.hb2=h1.hb1;
			h1.hb1=pre;
			h1.ha1=in;
		}
		
		if (STAGES>=2) {
			pre=in;			
			in = (in * c.b0 + h2.hb1 * c.b1  + h2.hb2 * c.b2 + h2.ha1 * c.a1 + h2.ha2 * c.a2);
			h2.ha2=h2.ha1;	
			h2.hb2=h2.hb1;
			h2.hb1=pre;
			h2.ha1=in;
		}
		
		if (STAGES>=3) {
			pre=in;			
			in = (in * c.b0 + h3.hb1 * c.b1  + h3.hb2 * c.b2 + h3.ha1 * c.a1 + h3.ha2 * c.a2);
			h3.ha2=h3.ha1;	
			h3.hb2=h3.hb1;
			h3.hb1=pre;
			h3.ha1=in;
		}
		
		if (STAGES>=4) {
			pre=in;			
			in = (in * c.b0 + h4.hb1 * c.b1  + h4.hb2 * c.b2 + h4.ha1 * c.a1 + h4.ha2 * c.a2);
			h4.ha2=h4.ha1;	
			h4.hb2=h4.hb1;
			h4.hb1=pre;
			h4.ha1=in;
		}
			
		*p_output+=in;
			
		if (RAMP) {
		
			c.a1+=c_inc.a1;
			c.a2+=c_inc.a2;
			c.b0+=c_inc.b0;
			c.b1+=c_inc.b1;
			c.b2+=c_inc.b2;
		}
		
		p_output++;
		p_input++;
	}
	
}

void FilterBankNode::process(const ProcessInfo& p_info) {

	ERR_FAIL_COND( p_info.audio_buffer_size>INTERNAL_BUFFER_SIZE );


	// quickly clear output buffers
	for (int i=0;i<get_channels();i++) {
	
		memset(get_audio_buffer( PORT_OUT, 0, i ),0,sizeof(sample_t)*p_info.audio_buffer_size);
	}
	
		
	for (int i=0;i<MAX_FILTERS;i++) {
		
		
		
		FilterData &f=filter[i];
		
		Filter::Mode current_mode;
		
		bool interpolate_coeffs=true;
		bool disabled=false;
		/* Determine Mode */
		switch (f.mode()) {
			
			case 0: disabled=true; break;
			case 1: current_mode=Filter::LOWPASS; break;
			case 2: current_mode=Filter::BANDPASS; break;
			case 3: current_mode=Filter::HIGHPASS; break;
			case 4: current_mode=Filter::NOTCH; break;
			default: current_mode=Filter::LOWPASS; break;
	
		}
		
		if(disabled)
			continue;
		
		/* Create filter */
		Filter filter;
		Filter::Coeffs c;
		
		float cutoff=f.cutoff();
		
		cutoff*=pow( 2 , cutoff_offset() );
		if (cutoff<50)
			cutoff=50; // shitty things happen with very low cutoffs
		
		filter.set_cutoff( cutoff );
		filter.set_mode( current_mode );
		filter.set_sampling_rate( get_mix_rate() );
		filter.set_resonance( f.resonance() );
		filter.set_stages( f.stages() + 1 );
		
		filter.prepare_coefficients( &c );
		
		/* if any of the conditions is given, the coefficients wont be interpolated */
		if (	current_mode!=f.prev_mode ||
			force_new_coeffs ||
			(     c.a1==f.prev_coeffs.a1  &&
			      c.a2==f.prev_coeffs.a2  &&
			      c.b0==f.prev_coeffs.b0  &&
			      c.b1==f.prev_coeffs.b1  &&
			      c.b2==f.prev_coeffs.b2  )
				      
		   ) {
				      
			interpolate_coeffs=false;
			
		}
							
		Filter::Coeffs c_inc; // increment
		Filter::Coeffs c_base; // increment
		
		if (interpolate_coeffs) {
					
			c_inc.a1=(c.a1-f.prev_coeffs.a1)/(float)p_info.audio_buffer_size;
			c_inc.a2=(c.a2-f.prev_coeffs.a2)/(float)p_info.audio_buffer_size;
			c_inc.b0=(c.b0-f.prev_coeffs.b0)/(float)p_info.audio_buffer_size;
			c_inc.b1=(c.b1-f.prev_coeffs.b1)/(float)p_info.audio_buffer_size;
			c_inc.b2=(c.b2-f.prev_coeffs.b2)/(float)p_info.audio_buffer_size;
			
			c_base=f.prev_coeffs;		
		} else {
		
			c_base=c;
		}
		
		int stages=f.stages()+1;
		
		//printf("filter %i, stages %i, mode %i, cutoff %f\n",i,stages,f.mode(),cutoff);
		for (int j=0;j<get_channels();j++) {
			
			Filter::Coeffs c_chan=c_base;
			
			sample_t *output=get_audio_buffer( PORT_OUT, 0, j );
			sample_t *input=get_audio_buffer( PORT_IN, 0, j );			
					
			for (int k=0;k<stages;k++) {
			
				FilterData::History::Stage &h=f.history[j].stage[k];
				
				if (k>=f.prev_stages) { //clear history if stages changed
					
					h.ha1=0;
					h.ha2=0;
					h.hb1=0;
					h.hb2=0;
								
				}
			}					
			
			if (interpolate_coeffs) {
			
				switch(stages) {
				
					case 1: _process_channel<1,false>(input,output,p_info.audio_buffer_size,c_chan,c_inc,f.history[j].stage); break;
					case 2: _process_channel<2,false>(input,output,p_info.audio_buffer_size,c_chan,c_inc,f.history[j].stage); break;
					case 3: _process_channel<3,false>(input,output,p_info.audio_buffer_size,c_chan,c_inc,f.history[j].stage); break;
					case 4: _process_channel<4,false>(input,output,p_info.audio_buffer_size,c_chan,c_inc,f.history[j].stage); break;

				}
			} else {
			
				switch(stages) {
				
					case 1: _process_channel<1,true>(input,output,p_info.audio_buffer_size,c_chan,c_inc,f.history[j].stage); break;
					case 2: _process_channel<2,true>(input,output,p_info.audio_buffer_size,c_chan,c_inc,f.history[j].stage); break;
					case 3: _process_channel<3,true>(input,output,p_info.audio_buffer_size,c_chan,c_inc,f.history[j].stage); break;
					case 4: _process_channel<4,true>(input,output,p_info.audio_buffer_size,c_chan,c_inc,f.history[j].stage); break;

				}
			}			
			
		}
			
		f.prev_stages=stages;
		f.prev_mode=current_mode;
		f.prev_coeffs=c;
	}
	
	force_new_coeffs=false;
}

FilterBankNode::FilterBankNode(int p_instanced_channels,const AudioNodeInfo *p_info) : HL_AudioNode(p_instanced_channels,p_info) {

	if (p_instanced_channels<1)
		p_instanced_channels=1;
	
	add_audio_port("Input",PORT_IN);
	add_audio_port("Output",PORT_OUT);
			
	static const float filter_defaults[MAX_FILTERS]={ 200,900,2000,4000 };
				
	cutoff_offset.set_all( 0, -4.0, 4.0, 0.01, ControlPort::HINT_RANGE, "Offset","o");
			
			
	for (int i=0;i<MAX_FILTERS;i++) {
		
		String c_prefix=String("Filter")+String::num(i+1)+"/"; //caption
		
		FilterData &f=filter[i];
		
		f.cutoff.set_all( filter_defaults[i],20,16000,0.1, ControlPort::HINT_RANGE,c_prefix+"Cutoff","hz");
		f.resonance.set_all( 1,0,4, 0.01, ControlPort::HINT_RANGE, c_prefix+"Resonance");
		
		std::vector<String> filter_mode;
		filter_mode.push_back("OFF");
		filter_mode.push_back("Low");
		filter_mode.push_back("Band");
		filter_mode.push_back("High");
		filter_mode.push_back("Notch");
		
		f.mode.set_enum(c_prefix+"Mode",filter_mode);
		
		std::vector<String> filter_stages;
		filter_stages.push_back("1");
		filter_stages.push_back("2");
		filter_stages.push_back("3");
		filter_stages.push_back("4");
		
		f.stages.set_enum(c_prefix+"Stages",filter_stages);
		
		f.history.resize(p_instanced_channels);
		
		add_control_port( PORT_IN, &f.mode);		
		add_control_port( PORT_IN, &f.cutoff);
		add_control_port( PORT_IN, &f.resonance);
		add_control_port( PORT_IN, &f.stages);
		
		f.prev_stages=-1;
	}
	
	add_control_port( PORT_IN, &cutoff_offset);

}


FilterBankNode::~FilterBankNode() {
		

}
