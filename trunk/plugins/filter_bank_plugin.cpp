//
// C++ Implementation: filter_bank_plugin
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "filter_bank_plugin.h"

#include <math.h>

namespace ReShaked {

static SoundPlugin* create_amplifier(const SoundPluginInfo *p_info,int p_channels) {
	
	return new FilterBankPlugin(p_info,p_channels);
	
}

const SoundPluginInfo *FilterBankPlugin::create_info() {
	
	static SoundPluginInfo info;
	
	info.caption="Filter Bank";
	info.description="Chain up to 4 Filters";
	info.long_description="Create your own equalization shape by carefully positioning each of the filters";
	info.unique_ID="INTERNAL_filterbank"; 
	info.category="Effects"; 
	info.can_custom_channels=true;
	info.has_internal_UI=false; 
	info.is_synth=false;
	info.xpm_preview=NULL;
	info.creation_func=&create_amplifier;
	info.version=1;	
	
	return &info;
}


int FilterBankPlugin::get_input_plug_count() {
	
	return 1;
}
int FilterBankPlugin::get_output_plug_count() {
	
	return 1;
	
}
AudioPlug *FilterBankPlugin::get_input_plug(int p_index) {
	
	return input_plug;	
}

AudioPlug *FilterBankPlugin::get_output_plug(int p_index) {
	
	return output_plug;	
}

int FilterBankPlugin::get_port_count() {
	
	return property_list.size();
}
Property& FilterBankPlugin::get_port(int p_index) {
	
	
	static LocalProperty error;
	
	ERR_FAIL_INDEX_V(p_index,get_port_count(),error);
	
	return *property_list[p_index];
	
}
SoundPlugin::PortType FilterBankPlugin::get_port_type(int p_index) {
	
	return TYPE_WRITE;
	
}

/* Setting up */
void FilterBankPlugin::set_mixing_rate(float p_mixing_rate) { //sort of useless
	
	mix_rate=p_mixing_rate;
	reset();
		
}

void FilterBankPlugin::reset() {
	
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

/* Processing */
void FilterBankPlugin::process(int p_frames) {

	

	if (skips_processing()) {
		
		output_plug->get_buffer()->copy_from( input_plug->get_buffer(), p_frames ); 
		
		return;
		
	}
	
	if (p_frames>INTERNAL_BUFFER_SIZE)
		return;
	
	output_plug->get_buffer()->clear( p_frames ); //clear
	
	for (int i=0;i<MAX_FILTERS;i++) {
		
		
		
		FilterData &f=filter[i];
		
		if (!f.active.get())
			continue;
		
		Filter::Mode current_mode;
		
		bool interpolate_coeffs=true;
		
		/* Determine Mode */
		switch (f.type.get_current()) {
			
			case 0: current_mode=Filter::LOWPASS; break;
			case 1: current_mode=Filter::BANDPASS; break;
			case 2: current_mode=Filter::HIGHPASS; break;
			case 3: current_mode=Filter::NOTCH; break;
			default: current_mode=Filter::LOWPASS; break;
	
		}
		
		/* Create filter */
		Filter filter;
		Filter::Coeffs c;
		
		float cutoff=f.cutoff.get();
		
		cutoff*=powf( 2 , cutoff_offset.get() );
		
		filter.set_cutoff( cutoff );
		filter.set_mode( current_mode );
		filter.set_sampling_rate( mix_rate );
		filter.set_resonance( f.resonance.get() );
		
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
							
		
		int stages=f.stages.get_current()+1;
		
		for (int j=0;j<get_channels_created();j++) {
			
			
			memcpy(internal_buffer,input_plug->get_buffer()->get_buffer(j),sizeof(float)*p_frames);
			float *output=output_plug->get_buffer()->get_buffer(j);
			float *input=internal_buffer;
			
					
			
			for (int k=0;k<stages;k++) {
			
				FilterData::History::Stage &h=f.history[j].stage[k];
				
				if (k>=f.prev_stages) { //clear history if stages changed
					
					h.ha1=0;
					h.ha2=0;
					h.hb1=0;
					h.hb2=0;
								
				}
				
				
				if (!interpolate_coeffs) {
					
					for (int l=0;l<p_frames;l++) {
						
			
						float pre=input[l];
						input[l] = (input[l] * c.b0 + h.hb1 * c.b1  + h.hb2 * c.b2 + h.ha1 * c.a1 + h.ha2 * c.a2); 	
						h.ha2=h.ha1;	
						h.hb2=h.hb1;
						h.hb1=pre;
						h.ha1=input[l];
						
					}
					
					
				} else {
					
					Filter::Coeffs cp=f.prev_coeffs; //previous coeffs
					
					float a1ramp=(c.a1-f.prev_coeffs.a1)/(float)p_frames;
					float a2ramp=(c.a2-f.prev_coeffs.a2)/(float)p_frames;
					float b0ramp=(c.b0-f.prev_coeffs.b0)/(float)p_frames;
					float b1ramp=(c.b1-f.prev_coeffs.b1)/(float)p_frames;
					float b2ramp=(c.b2-f.prev_coeffs.b2)/(float)p_frames;
					
					for (int l=0;l<p_frames;l++) {
						
			
						float pre=input[l];
						input[l] = (input[l] * cp.b0 + h.hb1 * cp.b1  + h.hb2 * cp.b2 + h.ha1 * cp.a1 + h.ha2 * cp.a2); 	
						h.ha2=h.ha1;	
						h.hb2=h.hb1;
						h.hb1=pre;
						h.ha1=input[l];
						
						cp.a1+=a1ramp;
						cp.a2+=a2ramp;
						cp.b0+=b0ramp;
						cp.b1+=b1ramp;
						cp.b2+=b2ramp;
					}									
				}
				
				//undenormalize to avoid high cpu peaks
				h.ha1=undenormalise(h.ha1);
				h.ha2=undenormalise(h.ha2);
				h.hb1=undenormalise(h.hb1);
				h.hb2=undenormalise(h.hb2);
							
			}
			
			for (int idx=0;idx<p_frames;idx++) {
				
				output[idx]+=internal_buffer[idx];
			}
		}	
			
		f.prev_stages=stages;
		f.prev_mode=current_mode;
		f.prev_coeffs=c;
	}
	
	force_new_coeffs=false;
}


FilterBankPlugin::FilterBankPlugin(const SoundPluginInfo *p_info,int p_channels) : SoundPlugin(p_info,p_channels) {
	
	if (p_channels<1)
		p_channels=1;
	
	static const float filter_defaults[MAX_FILTERS]={ 200,900,2000,4000 };
	
	cutoff_offset.set_all( 0, -4.0, 4.0, 0, 0.01, Property::DISPLAY_SLIDER, "global_cutoff","Global Cutoff","oct");
	property_list.push_back(&cutoff_offset);
	
	for (int i=0;i<MAX_FILTERS;i++) {
		
		String c_prefix=String("Filter ")+String::num(i+1)+"/"; //caption
		String n_prefix=String("filter_")+String::num(i+1)+"_";
		
		c_prefix="";
		
		FilterData &f=filter[i];
		
		f.active.set_all( 0, 0, 1, 0, 1, Property::DISPLAY_SLIDER, n_prefix+"active",c_prefix+"Active","","Off","On");
		f.cutoff.set_all( filter_defaults[i],0,16000,0, 0.1, Property::DISPLAY_SLIDER, n_prefix+"cutoff",c_prefix+"Cutoff","hz");
		f.resonance.set_all( 1,0,3, 1, 0.01, Property::DISPLAY_SLIDER, n_prefix+"resonance",c_prefix+"Resonance");
		
		std::vector<String> filter_type;
		filter_type.push_back("LowPass");
		filter_type.push_back("BandPass");
		filter_type.push_back("HighPass");
		filter_type.push_back("Notch");
		
		f.type.set_all(n_prefix+"mode",c_prefix+"Mode",filter_type,1);
		
		std::vector<String> filter_stages;
		filter_stages.push_back("1");
		filter_stages.push_back("2");
		filter_stages.push_back("3");
		filter_stages.push_back("4");
		
		f.stages.set_all(n_prefix+"stages",c_prefix+"Stages",filter_stages,0);
		
		f.history.resize(p_channels);
		
		property_list.push_back(&f.active);
		property_list.push_back(&f.cutoff);
		property_list.push_back(&f.resonance);
		property_list.push_back(&f.type);
		property_list.push_back(&f.stages);
		
		f.prev_stages=-1;
	}
	
	input_plug = new AudioPlug(p_channels,AudioPlug::TYPE_INPUT,this);
	output_plug = new AudioPlug(p_channels,AudioPlug::TYPE_OUTPUT,this);
	
	set_mixing_rate(44100);
	
	internal_buffer = new float[INTERNAL_BUFFER_SIZE];
}


FilterBankPlugin::~FilterBankPlugin(){ 
	
	delete input_plug;
	delete output_plug;
	delete[] internal_buffer;
}


}
