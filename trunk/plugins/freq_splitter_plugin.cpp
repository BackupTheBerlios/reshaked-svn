//
// C++ Implementation: freq_splitter_plugin
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "freq_splitter_plugin.h"

#include <math.h>
#include "pixmaps/icon_freq_split.xpm"

namespace ReShaked {

static SoundPlugin* create_freq_split(const SoundPluginInfo *p_info,int p_channels) {
	
	return new FreqSplitterPlugin(p_info,p_channels);
	
}

const SoundPluginInfo *FreqSplitterPlugin::create_info() {
	
	static SoundPluginInfo info;
	
	info.caption="Frequency Splitter";
	info.description="Split Signal Into High/Low Freqs.";
	info.long_description="Split a signal into high frequency and low frequency signals. Useful to apply effects and masterize.";
	info.unique_ID="INTERNAL_frequency_splitter"; 
	info.category="Effects"; 
	info.can_custom_channels=true;
	info.has_internal_UI=false; 
	info.is_synth=false;
	info.xpm_preview=(const char**)icon_freq_split_xpm;
	info.creation_func=&create_freq_split;
	info.version=1;	
	return &info;
}


int FreqSplitterPlugin::get_input_plug_count() {
	
	return 1;
}
int FreqSplitterPlugin::get_output_plug_count() {
	
	return 2;
	
}
AudioPlug *FreqSplitterPlugin::get_input_plug(int p_index) {
	
	return input_plug;	
}

AudioPlug *FreqSplitterPlugin::get_output_plug(int p_index) {
	
	if (p_index==0)
		return lpf_output_plug;
	else
		return hpf_output_plug;

}

int FreqSplitterPlugin::get_port_count() {
	
	return 1;
}
Property& FreqSplitterPlugin::get_port(int p_index) {
	
	return pivot;	
}
SoundPlugin::PortType FreqSplitterPlugin::get_port_type(int p_index) {
	
	return TYPE_WRITE;
	
}

/* Setting up */
void FreqSplitterPlugin::set_mixing_rate(float p_mixing_rate) { //sort of useless
	
	mix_rate=p_mixing_rate;
}

/* Processing */
void FreqSplitterPlugin::process(int p_frames) {
	
	
	float c=expf(-2.0*M_PI*pivot.get()/mix_rate); // 0 .. 10khz
	float ic=1.0-c;
	for (int j=0;j<get_channels_created();j++) {
		
		float *src=input_plug->get_buffer()->get_buffer( j );
		float *dst_lpf=lpf_output_plug->get_buffer()->get_buffer( j );
		float *dst_hpf=hpf_output_plug->get_buffer()->get_buffer( j );
		
		
		for (int i=0;i<p_frames;i++) {
			
			float out=undenormalise(src[i]*ic+c*h[j]);
			h[j]=out;
			dst_lpf[i]=out;
			dst_hpf[i]=src[i]-out;
			
		}
	}	
}


FreqSplitterPlugin::FreqSplitterPlugin(const SoundPluginInfo *p_info,int p_channels) : SoundPlugin(p_info,p_channels) {
	
	if (p_channels<1)
		p_channels=1;
	
	input_plug = new AudioPlug(p_channels,AudioPlug::TYPE_INPUT,this);
	lpf_output_plug = new AudioPlug(p_channels,AudioPlug::TYPE_OUTPUT,this);
	lpf_output_plug->set_name("LPF");
	hpf_output_plug = new AudioPlug(p_channels,AudioPlug::TYPE_OUTPUT,this);
	hpf_output_plug->set_name("HPF");
	
	pivot.set_all( 1000, 0.1, 12000, 1000, 0.1, Property::DISPLAY_SLIDER, "pivot","Pivot","hz");
	pivot.set_quad_coeff( true );
	mix_rate=44100;
	h = new float[p_channels];
	for (int i=0;i<p_channels;i++)
		h[i]=0;
}


FreqSplitterPlugin::~FreqSplitterPlugin() { 
	
	delete input_plug;
	delete lpf_output_plug;
	delete hpf_output_plug;
	delete[] h;
}


}
