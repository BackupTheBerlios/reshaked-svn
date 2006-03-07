//
// C++ Implementation: chionic
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "chionic.h"
#include "pixmaps/icon_chionic_2.xpm"
namespace ReShaked {


static SoundPlugin* create_chionic(const SoundPluginInfo *p_info,int p_channels) {
	
	/* Do not allow instancing at wrong channel numbers */
	if ( p_channels!=2 && p_channels !=4)
		return NULL;
	
	return new Chionic(p_info,p_channels);
}

const SoundPluginInfo *Chionic::create_info() {
	
	static SoundPluginInfo info;
	
	info.caption="Chionic 2";
	info.description="Advanced Sampler and Additive Synthesizer";
	info.long_description="Chionic 2 is an advanced sampler and additive synthesizer. It's capable of multi layered voices and many kind of modulations between them, as well as modulating an audio source. It also supports many features intended for producing extremely realistic instruments";
	info.unique_ID="INTERNAL_chionic"; 
	info.category_path="/synthesizers"; 
	info.can_custom_channels=true;
	info.custom_channels.push_back(2);
	info.custom_channels.push_back(4);
	info.has_internal_UI=false; 
	info.is_synth=true;
	info.xpm_preview=(const char**)icon_chionic_2_xpm;
	info.creation_func=&create_chionic;
	info.version=1;
	
	return &info;
}


/* Plugs */	

int Chionic::get_input_plug_count()  {
	
	return 1;	
	
}
int Chionic::get_output_plug_count()  {
	
	return 1;
	
}
AudioPlug *Chionic::get_input_plug(int p_index)  {
	
	return input_plug;
	
}
AudioPlug *Chionic::get_output_plug(int p_index)  {
	
	return output_plug;	
	
}


/* Port Info */
int Chionic::get_port_count()  {
	
	return 0;
	
}
Property& Chionic::get_port(int p_index)  {
	
	static LocalProperty none;
	
	return none;
}

SoundPlugin::PortType Chionic::get_port_type(int p_index)  {
	
	return TYPE_WRITE;
	
}


/* Setting up */
void Chionic::set_mixing_rate(float p_mixing_rate)  {
	
	
	
}

void Chionic::reset() {
	
	
}

/* Processing */
void Chionic::process(int p_frames)  {
	
	
	
}


Chionic::Chionic(const SoundPluginInfo *p_info,int p_channels) : SoundPlugin(p_info,p_channels) {
	
	input_plug = new AudioPlug(p_channels,AudioPlug::TYPE_INPUT,this);
	output_plug = new AudioPlug(p_channels,AudioPlug::TYPE_OUTPUT,this);
	
	
}
Chionic::~Chionic()  {
	
	
	
}


}
