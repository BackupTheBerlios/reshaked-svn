//
// C++ Implementation: reverb_plugin
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "reverb_plugin.h"


namespace ReShaked {

static SoundPlugin* create_reverb(const SoundPluginInfo *p_info,int p_channels) {
	
	return new ReverbPlugin(p_info,p_channels);
	
}

const SoundPluginInfo *ReverbPlugin::create_info() {
	
	static SoundPluginInfo info;
	
	info.caption="Reverb";
	info.description="Room Reflections Simulator";
	info.long_description="Simulates the spreading of sound waves and its reflections in a virtual room";
	info.unique_ID="INTERNAL_reverb"; 
	info.category="Internal"; 
	info.can_custom_channels=true;
	info.has_internal_UI=false; 
	info.is_synth=false;
	info.xpm_preview=NULL;
	info.creation_func=&create_reverb;
	info.version=1;	
	return &info;
}


int ReverbPlugin::get_input_plug_count() {
	
	return 1;
}
int ReverbPlugin::get_output_plug_count() {
	
	return 1;
	
}
AudioPlug *ReverbPlugin::get_input_plug(int p_index) {
	
	return input_plug;	
}

AudioPlug *ReverbPlugin::get_output_plug(int p_index) {
	
	return output_plug;	
}

int ReverbPlugin::get_port_count() {
		
	return property_list.size();
}
Property& ReverbPlugin::get_port(int p_index) {
	
	static LocalProperty error;
	
	ERR_FAIL_INDEX_V(p_index,get_port_count(),error);
	
	return *property_list[p_index];
	
}
SoundPlugin::PortType ReverbPlugin::get_port_type(int p_index) {
	
	return TYPE_WRITE;
	
}

/* Setting up */
void ReverbPlugin::set_mixing_rate(float p_mixing_rate) { //sort of useless
	
	for (int i=0;i<get_channels_created();i++) {
					
		reverb[i].set_mix_rate(p_mixing_rate);
	}
}

/* Processing */
void ReverbPlugin::process(int p_frames) {
	
	for (int i=0;i<get_channels_created();i++) {
		
		Reverb &r=reverb[i];
		
		r.set_predelay( predelay.get() );
		r.set_predelay_feedback( predelay_fb.get() );
		r.set_room_size( room_size.get() );
		r.set_damp( damping.get() );
		r.set_wet( wet.get() );
		r.set_dry( dry.get() );
		
		r.process( input_plug->get_buffer()->get_buffer( i ), output_plug->get_buffer()->get_buffer( i ), p_frames );
		
	}
	
}


ReverbPlugin::ReverbPlugin(const SoundPluginInfo *p_info,int p_channels) : SoundPlugin(p_info,p_channels) {
	
	if (p_channels<1)
		p_channels=1;
	
	input_plug = new AudioPlug(p_channels,AudioPlug::TYPE_INPUT,this);
	output_plug = new AudioPlug(p_channels,AudioPlug::TYPE_OUTPUT,this);
	
	reverb = new Reverb[p_channels];
	
	for (int i=0;i<p_channels;i++) {
		
		reverb[i].set_mix_rate( 44100.0 ); //some default
		reverb[i].set_extra_spread( (float)i*0.000521 ); //gives stereoysh effect

	}
	
	predelay.set_all( 100, 20, 250, 100, 1, Property::DISPLAY_SLIDER, "predelay","Predelay");
	predelay_fb.set_all( 0, 0, 1, 0, 0.01, Property::DISPLAY_SLIDER, "predelay_fb","Pre-Feedback");
	room_size.set_all( 0.7, 0, 1, 0.7, 0.01, Property::DISPLAY_SLIDER, "room_size","Room Size");
	damping.set_all( 0.5, 0, 1, 0.5, 0.01, Property::DISPLAY_SLIDER, "damping","LoFrq Damp");
	wet.set_all( 0, 0, 1, 1, 0.01, Property::DISPLAY_SLIDER, "wet","Wet");
	dry.set_all( 1, 0, 1, 1, 0.01, Property::DISPLAY_SLIDER, "dry","Dry");
	
	property_list.push_back(&predelay);
	property_list.push_back(&predelay_fb);
	property_list.push_back(&room_size);
	property_list.push_back(&damping);
	property_list.push_back(&wet);
	property_list.push_back(&dry);
}


ReverbPlugin::~ReverbPlugin(){ 
	
	delete[] reverb;
}


}
