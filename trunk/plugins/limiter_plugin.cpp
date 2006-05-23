//
// C++ Implementation: limiter_plugin
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "limiter_plugin.h"

#include <math.h>
//#include "pixmaps/effect_limiter.xpm"
#include "dsp/formulas.h"

#include "pixmaps/icon_limiter.xpm"

namespace ReShaked {

static SoundPlugin* create_limiter(const SoundPluginInfo *p_info,int p_channels) {
	
	return new LimiterPlugin(p_info,p_channels);
	
}

const SoundPluginInfo *LimiterPlugin::create_info() {
	
	static SoundPluginInfo info;
	
	info.caption="Limiter";
	info.description="Limit Signal Amplitude";
	info.long_description="The Limiter Plugin attempts to avoid the input signal level from going higher than a specified amplitude (in dB). It introduces a very small delay (given by the attack parameter) intended for looking ahead level changes and compensating them.";
	info.unique_ID="INTERNAL_limiter"; 
	info.category="Effects"; 
	info.can_custom_channels=true;
	info.custom_channels.push_back(1);
	info.custom_channels.push_back(2);
	info.custom_channels.push_back(4);
	info.has_internal_UI=false; 
	info.is_synth=false;
	info.xpm_preview=(const char**)icon_limiter_xpm;;
	info.creation_func=&create_limiter;
	info.version=1;	
	return &info;
}


int LimiterPlugin::get_input_plug_count() {
	
	return 1;
}
int LimiterPlugin::get_output_plug_count() {
	
	return 1;
	
}
AudioPlug *LimiterPlugin::get_input_plug(int p_index) {
	
	return input_plug;	
}

AudioPlug *LimiterPlugin::get_output_plug(int p_index) {
	
	return output_plug;
}

int LimiterPlugin::get_port_count() {
	
	return property_list.size();
}
Property& LimiterPlugin::get_port(int p_index) {
	
	static LocalProperty error;
	
	ERR_FAIL_INDEX_V(p_index,property_list.size(),error);
	
	return *property_list[ p_index ];
	
}
SoundPlugin::PortType LimiterPlugin::get_port_type(int p_index) {
	
	return TYPE_WRITE;
	
}

/* Setting up */
void LimiterPlugin::set_mixing_rate(float p_mixing_rate) { //sort of useless
	
	mix_rate=p_mixing_rate;
	reset();
		
}

void LimiterPlugin::reset() {
	
	
	float ring_buffer_max_size=MAX_DELAY_MS;
	ring_buffer_max_size/=1000.0;//convert to seconds
	ring_buffer_max_size*=mix_rate;
	
	int ringbuff_size=(int)ring_buffer_max_size;
	
	int bits=0;
	
	while(ringbuff_size>0) {
		bits++;
		ringbuff_size/=2;
	}
	
	ringbuff_size=1<<bits;
	ring_mask=ringbuff_size-1;
	ring_pos=0;
	
	ring.setup(get_channels_created(), ringbuff_size );
	peaked=false;
	
}

/* Processing */

void LimiterPlugin::process(int p_frames) {

	if (skips_processing()) {
	
		output_plug->get_buffer()->copy_from( input_plug->get_buffer(), p_frames ); 
		return;
	}
	
	/* see? this is good use for templates */
	if (get_channels_created()==1)
		process_1(p_frames);
	else if (get_channels_created()==2)
		process_2(p_frames);
	else if (get_channels_created()==4)
		process_4(p_frames);
	
}

float LimiterPlugin::get_max_val() {
	
	float aux_max=max_val;
	max_val=-999;
	
	return aux_max;
	
}

bool LimiterPlugin::has_peaked() {
	
	return peaked;
}

void LimiterPlugin::clear_peaked() {
	
	peaked=false;
}

void LimiterPlugin::process_1(float p_frames) {}
void LimiterPlugin::process_4(float p_frames) {}


void LimiterPlugin::process_2(float p_frames) {
	
	
	float *src_l=input_plug->get_buffer()->get_buffer(0);
	float *src_r=input_plug->get_buffer()->get_buffer(1);
	
	float *rb_l=ring.get_buffer(0);
	float *rb_r=ring.get_buffer(1);
	
	float *dst_l=output_plug->get_buffer()->get_buffer(0);
	float *dst_r=output_plug->get_buffer()->get_buffer(1);
	
	int attack_frames=lrint((attack.get()/1000.0)*mix_rate);
	int release_frames=lrint((release.get()/1000.0)*mix_rate);
	float threshold_f=db_to_energy(threshold.get());
	
	float pregain_f=db_to_energy(pre_gain.get());
	float postgain_f=db_to_energy(post_gain.get());
	
	float max_linear=0;
	
	for (int i=0;i<p_frames;i++) {
		
		float l=src_l[i]*pregain_f;
		float r=src_r[i]*pregain_f;
		
		rb_l[ring_pos&ring_mask]=l;
		rb_r[ring_pos&ring_mask]=r;
		
		float absl=fabsf(l);
		float absr=fabsf(r);
		float nrg=(absl>absr)?absl:absr;
		
		
		if (nrg>threshold_f) {
			
			float desired_gain=threshold_f/nrg;
			
			if (desired_gain<target_gain) { /* Setup ATTACK */
							
				target_gain=desired_gain;
				increment=(target_gain-current_gain)/(float)attack_frames; //reach target gain in N frames
				
			}
		} 
			
		
		
		current_gain+=increment;
		
		if (current_gain<target_gain || current_gain<=0) {
				
			target_gain=1.0; //back to threshold_f
			increment=(target_gain-current_gain)/(float)release_frames; //reach target gain in N
		}
			
		if (current_gain>1.0) {
				
			current_gain=target_gain=1.0;
			increment=0;
		}
		
		
		dst_l[i]=rb_l[(ring_pos-attack_frames)&ring_mask]*current_gain*postgain_f;
		dst_r[i]=rb_r[(ring_pos-attack_frames)&ring_mask]*current_gain*postgain_f;
		
		float abs_l=fabsf(dst_l[i]);
		float abs_r=fabsf(dst_r[i]);
		
		if (abs_l>=1.0 || abs_r>=1.0)
			peaked=true;
		
		if (abs_l>max_linear)
			max_linear=abs_l;
		
		if (abs_r>max_linear)
			max_linear=abs_r;
		
		
		ring_pos++;
	}
	
	float max_val_db=(max_linear>0) ? energy_to_db(max_linear) : -999; //ridiculous value
	if (max_val_db>max_val)
		max_val=max_val_db;
	
}


LimiterPlugin::LimiterPlugin(const SoundPluginInfo *p_info,int p_channels) : SoundPlugin(p_info,p_channels) {
	
	if (p_channels<1)
		p_channels=1;
	
	input_plug = new AudioPlug(p_channels,AudioPlug::TYPE_INPUT,this);
	output_plug = new AudioPlug(p_channels,AudioPlug::TYPE_OUTPUT,this);
	

	pre_gain.set_all( 0, -60, 24, 0, 0.1, Property::DISPLAY_SLIDER, "pre_gain","Pre Gain","dB");
	threshold.set_all( 0, -60, 0, 0, 0.1, Property::DISPLAY_SLIDER, "threshold","Threshold","dB");
	attack.set_all( 5, 0.1, MAX_DELAY_MS, 1, 0.1, Property::DISPLAY_SLIDER, "attack","Attack","ms");
	release.set_all( 50, 0.1, 100, 1, 0.1, Property::DISPLAY_SLIDER, "release","Release","ms");
	post_gain.set_all( 0, -60, 24, 0, 0.1, Property::DISPLAY_SLIDER, "post_gain","Post Gain","dB");
	
	property_list.push_back( &pre_gain );
	property_list.push_back( &threshold );
	property_list.push_back( &attack );
	property_list.push_back( &release );
	property_list.push_back( &post_gain );
	
	mix_rate=44100;
	reset();
	
	current_gain=1.0;
	target_gain=1.0;
	increment=0;
	peaked=false;
	max_val=0;
}


LimiterPlugin::~LimiterPlugin() { 
	
	delete input_plug;
	delete output_plug;
}


}
