//
// C++ Implementation: stereo_enhancer_plugin
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "stereo_enhancer_plugin.h"


#include <math.h>
#include "icon_stereo_enhance.xpm"
namespace ReShaked {

static SoundPlugin* create_stereo_enhancer(const SoundPluginInfo *p_info,int p_channels) {
	
	return new StereoEnhancerPlugin(p_info,p_channels);
	
}

const SoundPluginInfo *StereoEnhancerPlugin::create_info() {
	
	static SoundPluginInfo info;
	
	info.caption="Stereo Enhancer";
	info.description="Stereo Improvement Tools";
	info.long_description="Supplied are a pan pullout ( forces more panning between channels ) and time pullout , which can work as a delay between channels, or a delayed surround.";
	info.unique_ID="INTERNAL_stereo_enhancer"; 
	info.category="Effects"; 
	info.can_custom_channels=true;
	info.custom_channels.push_back(2);
	info.has_internal_UI=false; 
	info.is_synth=false;
	info.xpm_preview=(const char**)icon_stereo_enhance_xpm;
	info.creation_func=&create_stereo_enhancer;
	info.version=1;	
	return &info;
}


int StereoEnhancerPlugin::get_input_plug_count() {
	
	return 1;
}
int StereoEnhancerPlugin::get_output_plug_count() {
	
	return 1;
	
}
AudioPlug *StereoEnhancerPlugin::get_input_plug(int p_index) {
	
	return input_plug;	
}

AudioPlug *StereoEnhancerPlugin::get_output_plug(int p_index) {
	
	return output_plug;	
}

int StereoEnhancerPlugin::get_port_count() {
	
	return property_list.size();
}
Property& StereoEnhancerPlugin::get_port(int p_index) {
	
	static LocalProperty error_prop;
	
	ERR_FAIL_INDEX_V(p_index,property_list.size(),error_prop);
	
	return *property_list[p_index];	
}
SoundPlugin::PortType StereoEnhancerPlugin::get_port_type(int p_index) {
	
	return TYPE_WRITE;
	
}

/* Setting up */
void StereoEnhancerPlugin::set_mixing_rate(float p_mixing_rate) { //sort of useless
	
	mix_rate=p_mixing_rate;
	reset();		
}

void StereoEnhancerPlugin::reset() {
	
	float ring_buffer_max_size=MAX_DELAY_MS+2;
	ring_buffer_max_size/=1000.0;//convert to seconds
	ring_buffer_max_size*=mix_rate;
	
	int ringbuff_size=(int)ring_buffer_max_size;
	
	int bits=0;
	
	while(ringbuff_size>0) {
		bits++;
		ringbuff_size/=2;
	}
	
	ringbuff_size=1<<bits;
	ringbuff_mask=ringbuff_size-1;
	ringbuff_pos=0;
	
	if (delay_ringbuff)
		delete[] delay_ringbuff;
	
	delay_ringbuff = new float[ ringbuff_size ];

}

/* Processing */
void StereoEnhancerPlugin::process(int p_frames) {
	
	if (skips_processing()) {
	
		output_plug->get_buffer()->copy_from( input_plug->get_buffer(), p_frames ); 
		return;
	}
	
	float *src_l=input_plug->get_buffer()->get_buffer( 0 );
	float *src_r=input_plug->get_buffer()->get_buffer( 1 );
	
	float *dst_l=output_plug->get_buffer()->get_buffer( 0 );
	float *dst_r=output_plug->get_buffer()->get_buffer( 1 );
	
	float intensity=pan_pullout.get()/100.0;
	bool surround_mode=surround.get()>0;
	float surround_amount=surround.get()/100.0;
	unsigned int delay_frames=(time_pullout.get()/1000.0)*mix_rate;
	
	for (int i=0;i<p_frames;i++) {
		
		float l=src_l[i];
		float r=src_r[i];		
		
		float center=(l+r)/2.0f;

		l=( center+(l-center)*intensity ); 
		r=( center+(r-center)*intensity ); 

		if (surround_mode) {
			
			float val=(l+r)/2.0;
			
			delay_ringbuff[ringbuff_pos&ringbuff_mask]=val;
			
			float out=delay_ringbuff[(ringbuff_pos-delay_frames)&ringbuff_mask]*surround_amount;
			
			l+=out;
			r+=-out;
		} else {
			
			float val=r;
			
			delay_ringbuff[ringbuff_pos&ringbuff_mask]=val;
			
			//r is delayed
			r=delay_ringbuff[(ringbuff_pos-delay_frames)&ringbuff_mask];;
			
			
		}
		
		dst_l[i]=l;
		dst_r[i]=r;
		ringbuff_pos++;
		
	}
	
}


StereoEnhancerPlugin::StereoEnhancerPlugin(const SoundPluginInfo *p_info,int p_channels) : SoundPlugin(p_info,p_channels) {
	
	if (p_channels<1)
		p_channels=1;
	
	input_plug = new AudioPlug(p_channels,AudioPlug::TYPE_INPUT,this);
	output_plug = new AudioPlug(p_channels,AudioPlug::TYPE_OUTPUT,this);
	
	pan_pullout.set_all( 100, 0, 400, 100, 1, Property::DISPLAY_SLIDER, "pan_pull","Pan Pull","%","mono");
	time_pullout.set_all( 0, 0, MAX_DELAY_MS, 0, 0.1, Property::DISPLAY_SLIDER, "time_pullout","Time Pull","ms","Off");
	surround.set_all( 0, 0, 100, 0, 1, Property::DISPLAY_SLIDER, "surround","Surround","%","Off");
	
	property_list.push_back(&pan_pullout);
	property_list.push_back(&time_pullout);
	property_list.push_back(&surround);
	
	delay_ringbuff=NULL;
	set_mixing_rate( 44100 );
	
}


StereoEnhancerPlugin::~StereoEnhancerPlugin() { 
	
	delete input_plug;
	delete output_plug;
	delete[] delay_ringbuff;
	
}


}
