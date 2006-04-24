//
// C++ Implementation: amplifier_plugin
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "amplifier_plugin.h"
#include <math.h>
#include "pixmaps/effect_amplifier.xpm"
namespace ReShaked {

static SoundPlugin* create_amplifier(const SoundPluginInfo *p_info,int p_channels) {
	
	return new AmplifierPlugin(p_info,p_channels);
	
}

const SoundPluginInfo *AmplifierPlugin::create_info() {
	
	static SoundPluginInfo info;
	
	info.caption="Amplifier";
	info.description="Changes signal amplitude";
	info.long_description="The amplifier works like any volume knob or slider in your stereo/discman/etc.";
	info.unique_ID="INTERNAL_amplifier"; 
	info.category="Internal"; 
	info.can_custom_channels=true;
	info.has_internal_UI=false; 
	info.is_synth=false;
	info.xpm_preview=(const char**)effect_amplifier_xpm;
	info.creation_func=&create_amplifier;
	info.version=1;	
	return &info;
}


int AmplifierPlugin::get_input_plug_count() {
	
	return 1;
}
int AmplifierPlugin::get_output_plug_count() {
	
	return 1;
	
}
AudioPlug *AmplifierPlugin::get_input_plug(int p_index) {
	
	return input_plug;	
}

AudioPlug *AmplifierPlugin::get_output_plug(int p_index) {
	
	return output_plug;	
}

int AmplifierPlugin::get_port_count() {
	
	return 2;
}
Property& AmplifierPlugin::get_port(int p_index) {
	
	if (p_index==0) {
		
		return gain;
	} else {
		
		return signal_energy;
	}
	
}
SoundPlugin::PortType AmplifierPlugin::get_port_type(int p_index) {
	
	if (p_index==0) {
		
		return TYPE_WRITE;
	} else {
		
		return TYPE_READ;
	}
	
}

/* Setting up */
void AmplifierPlugin::set_mixing_rate(float p_mixing_rate) { //sort of useless
	
		
}

/* Processing */
void AmplifierPlugin::process(int p_frames) {
	
	float gain_linear=powf(10.0,gain.get()/20.0); //convert db -> linear
	
	float max=0;; //cheap way of computing energy, will fix later, this is for test!
	
	for (int i=0;i<input_plug->get_channels();i++) {
		
		float *src=input_plug->get_buffer()->get_buffer(i);
		float *dst=output_plug->get_buffer()->get_buffer(i);
		
		
		for (int j=0;j<p_frames;j++) {
			
			float v=src[j]*gain_linear;
			if (fabsf(v)>max)
				max=fabsf(v);
			dst[j]=v;
		}
	}
	if (max<=0.0)
		max=0.0000001;
	
	signal_energy.set(20*(logf(max)/logf(10.0)));
}


AmplifierPlugin::AmplifierPlugin(const SoundPluginInfo *p_info,int p_channels) : SoundPlugin(p_info,p_channels) {
	
	if (p_channels<1)
		p_channels=1;
	
	input_plug = new AudioPlug(p_channels,AudioPlug::TYPE_INPUT,this);
	output_plug = new AudioPlug(p_channels,AudioPlug::TYPE_OUTPUT,this);
	
	gain.set_all( 0, -60, 24, 0, 0.1, Property::DISPLAY_SLIDER, "gain","Gain","dB");
	signal_energy.set_all( 0, -60, 24, 0, 0.01, Property::DISPLAY_VU, "energy","Energy","dB");
}


AmplifierPlugin::~AmplifierPlugin(){ 
	
	delete input_plug;
	delete output_plug;
}


}
