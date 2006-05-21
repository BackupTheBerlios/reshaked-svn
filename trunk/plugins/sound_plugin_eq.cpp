//
// C++ Implementation: sound_plugin_eq
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sound_plugin_eq.h"

#include "dsp/formulas.h"

#include "pixmaps/icon_eq_6.xpm"
#include "pixmaps/icon_eq_10.xpm"

namespace ReShaked {

static SoundPlugin* create_eq_6(const SoundPluginInfo *p_info,int p_channels) {
	
	return new SoundPluginEQ(p_info,p_channels,SoundPluginEQ::CREATE_EQ_6);
	
}
static SoundPlugin* create_eq_10(const SoundPluginInfo *p_info,int p_channels) {
	
	return new SoundPluginEQ(p_info,p_channels,SoundPluginEQ::CREATE_EQ_10);
	
}

const SoundPluginInfo *SoundPluginEQ::create_info_6() {
	
	static SoundPluginInfo info;
	
	info.caption="6 Bands Equalizer";
	info.description="Graphic Equalizer";
	info.long_description="Adjust the frequency by modifying the shape of the bands.";
	info.unique_ID="INTERNAL_eq_6"; 
	info.category="Effects"; 
	info.can_custom_channels=true;
	info.has_internal_UI=false; 
	info.is_synth=false;
	info.xpm_preview=(const char**)icon_eq_6_xpm;
	info.creation_func=&create_eq_6;
	info.version=1;	
	return &info;
}

const SoundPluginInfo *SoundPluginEQ::create_info_10() {
	
	static SoundPluginInfo info;
	
	info.caption="10 Bands Equalizer";
	info.description="Graphic Equalizer";
	info.long_description="Adjust the frequency by modifying the shape of the bands.";
	info.unique_ID="INTERNAL_eq_10"; 
	info.category="Effects"; 
	info.can_custom_channels=true;
	info.has_internal_UI=false; 
	info.is_synth=false;
	info.xpm_preview=(const char**)icon_eq_10_xpm;
	info.creation_func=&create_eq_10;
	info.version=1;	
	return &info;
}

int SoundPluginEQ::get_input_plug_count() {
	
	return 1;
}
int SoundPluginEQ::get_output_plug_count() {
	
	return 1;
	
}
AudioPlug *SoundPluginEQ::get_input_plug(int p_index) {
	
	return input_plug;	
}

AudioPlug *SoundPluginEQ::get_output_plug(int p_index) {
	
	return output_plug;	
}

int SoundPluginEQ::get_port_count() {
	
	return band_count;
}
Property& SoundPluginEQ::get_port(int p_index) {
	
	static LocalProperty error_prop;
	ERR_FAIL_INDEX_V(p_index,band_count,error_prop);
	
	return band[p_index].gain;
}
SoundPlugin::PortType SoundPluginEQ::get_port_type(int p_index) {
	
	return TYPE_WRITE;	
}

/* Setting up */
void SoundPluginEQ::set_mixing_rate(float p_mixing_rate) { 
	
	eq.set_mix_rate( p_mixing_rate );
	reset();
}

void SoundPluginEQ::reset() { 
	
	for (int i=0;i<band_count;i++) {
		
		for (int j=0;j<get_channels_created();j++) {
			
			band[i].channel_process[j]=eq.get_band_processor( i );
		}
	}
}

/* Processing */
void SoundPluginEQ::process(int p_frames) {
	
	output_plug->get_buffer()->clear( p_frames );
	
	for (int i=0;i<get_channels_created();i++) {
		
		for (int j=0;j<band_count;j++) {
			
			float gain=db_to_energy(band[j].gain.get());
			
			EQ::BandProcess &bp=band[j].channel_process[i];
			float *src=input_plug->get_buffer()->get_buffer( i );
			float *dst=output_plug->get_buffer()->get_buffer( i );
			
			for (int k=0;k<p_frames;k++) {
				
				float v=src[k];
				bp.process_one(v);
				v*=gain;
				dst[k]+=v;
			}
			
		}
	}
}


SoundPluginEQ::SoundPluginEQ(const SoundPluginInfo *p_info,int p_channels,CreateMode p_create_mode) : SoundPlugin(p_info,p_channels) {
	
	if (p_channels<1)
		p_channels=1;
	
	switch (p_create_mode) {
		
		case CREATE_EQ_6: band_count=6; eq.set_preset_band_mode( EQ::PRESET_6_BANDS ); break;
		case CREATE_EQ_10: band_count=10; eq.set_preset_band_mode( EQ::PRESET_10_BANDS ); break;
	};
	
	input_plug = new AudioPlug(p_channels,AudioPlug::TYPE_INPUT,this);
	output_plug = new AudioPlug(p_channels,AudioPlug::TYPE_OUTPUT,this);
	
	band = new Band[band_count];
	
	for(int i=0;i<band_count;i++) {
		float freq=eq.get_band_frequency(i);
		String label=( (freq<1000)? (String::num((int)freq)+"hz") : (String::num(freq/1000.0,1)+"khz"));
		band[i].gain.set_all( 0, -60, 24, 0, 0.1, Property::DISPLAY_SLIDER, "gain_"+String::num(i+1),label,"dB");
		band[i].channel_process.resize(p_channels);
	}
	
	set_mixing_rate( 44100 );
	
	
}


SoundPluginEQ::~SoundPluginEQ() { 
	
	delete input_plug;
	delete output_plug;
	delete[] band;
}


}
