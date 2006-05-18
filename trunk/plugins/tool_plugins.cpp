//
// C++ Implementation: tool_plugins
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "tool_plugins.h"

namespace ReShaked {

static SoundPlugin* create_splitter(const SoundPluginInfo *p_info,int p_channels) {
	
	return new SplitterPlugin(p_info,p_channels);
	
}

const SoundPluginInfo *SplitterPlugin::create_info() {
	
	static SoundPluginInfo info;
	
	info.caption="Splitter";
	info.description="Split to Mono Components";
	info.long_description="This will split any input into it's mono (single channel) components.";
	info.unique_ID="INTERNAL_splitter"; 
	info.category="Tools"; 
	info.can_custom_channels=true;
	info.has_internal_UI=false; 
	info.is_synth=false;
	info.xpm_preview=NULL;
	info.creation_func=&create_splitter;
	info.version=1;	
	return &info;
}


int SplitterPlugin::get_input_plug_count() {
	
	return 1;
}
int SplitterPlugin::get_output_plug_count() {
	
	return output_plugs.size();
	
}
AudioPlug *SplitterPlugin::get_input_plug(int p_index) {
	
	return input_plug;	
}

AudioPlug *SplitterPlugin::get_output_plug(int p_index) {
	
	ERR_FAIL_INDEX_V(p_index,output_plugs.size(),NULL);
	
	return output_plugs[p_index];	
}

int SplitterPlugin::get_port_count() {
	
	return 0;
}
Property& SplitterPlugin::get_port(int p_index) {
	
	static LocalProperty error;
	
	ERR_PRINT("INVALID PORT");
	
	return error;
	
}
SoundPlugin::PortType SplitterPlugin::get_port_type(int p_index) {
	
	ERR_PRINT("INVALID PORT");
	return TYPE_WRITE;
	
}

/* Setting up */
void SplitterPlugin::set_mixing_rate(float p_mixing_rate) { //sort of useless
	
		
}

/* Processing */
void SplitterPlugin::process(int p_frames) {
	
	
	for (int i=0;i<input_plug->get_channels();i++) {
		
		float *src=input_plug->get_buffer()->get_buffer(i);
		float *dst=output_plugs[i]->get_buffer()->get_buffer(i);
		memcpy(dst,src,p_frames);
	}
}


SplitterPlugin::SplitterPlugin(const SoundPluginInfo *p_info,int p_channels) : SoundPlugin(p_info,p_channels) {
	
	if (p_channels<1)
		p_channels=1;
	
	input_plug = new AudioPlug(p_channels,AudioPlug::TYPE_INPUT,this);
	for (int i=0;i<p_channels;i++) 
		output_plugs.push_back( new AudioPlug(1,AudioPlug::TYPE_OUTPUT,this) );
	
}


SplitterPlugin::~SplitterPlugin() { 
	
	delete input_plug;
	for (int i=0;i<output_plugs.size();i++)
		delete output_plugs[i];
}


/***********************************************************************/
/***********************************************************************/
/***********************************************************************/
/***********************************************************************/

static SoundPlugin* create_merger(const SoundPluginInfo *p_info,int p_channels) {
	
	return new MergerPlugin(p_info,p_channels);
	
}

const SoundPluginInfo *MergerPlugin::create_info() {
	
	static SoundPluginInfo info;
	
	info.caption="Merger";
	info.description="Merge from Mono Components";
	info.long_description="This will merge mono inputs to multichannel plugs.";
	info.unique_ID="INTERNAL_merger"; 
	info.category="Tools"; 
	info.can_custom_channels=true;
	info.has_internal_UI=false; 
	info.is_synth=false;
	info.xpm_preview=NULL;
	info.creation_func=&create_merger;
	info.version=1;	
	return &info;
}


int MergerPlugin::get_output_plug_count() {
	
	return 1;
}
int MergerPlugin::get_input_plug_count() {
	
	return input_plugs.size();
	
}
AudioPlug *MergerPlugin::get_output_plug(int p_index) {
	
	return output_plug;	
}

AudioPlug *MergerPlugin::get_input_plug(int p_index) {
	
	ERR_FAIL_INDEX_V(p_index,input_plugs.size(),NULL);
	
	return input_plugs[p_index];	
}

int MergerPlugin::get_port_count() {
	
	return 0;
}
Property& MergerPlugin::get_port(int p_index) {
	
	static LocalProperty error;
	
	ERR_PRINT("INVALID PORT");
	
	return error;
	
}
SoundPlugin::PortType MergerPlugin::get_port_type(int p_index) {
	
	ERR_PRINT("INVALID PORT");
	return TYPE_WRITE;
	
}

/* Setting up */
void MergerPlugin::set_mixing_rate(float p_mixing_rate) { //sort of useless
	
		
}

/* Processing */
void MergerPlugin::process(int p_frames) {
	
	
	for (int i=0;i<output_plug->get_channels();i++) {
		
		float *src=input_plugs[i]->get_buffer()->get_buffer(i);
		float *dst=output_plug->get_buffer()->get_buffer(i);
		memcpy(dst,src,p_frames);
	}
}


MergerPlugin::MergerPlugin(const SoundPluginInfo *p_info,int p_channels) : SoundPlugin(p_info,p_channels) {
	
	if (p_channels<1)
		p_channels=1;
	
	output_plug = new AudioPlug(p_channels,AudioPlug::TYPE_OUTPUT,this);
	for (int i=0;i<p_channels;i++) 
		input_plugs.push_back( new AudioPlug(1,AudioPlug::TYPE_INPUT,this) );
	
}


MergerPlugin::~MergerPlugin() { 
	
	delete output_plug;
	for (int i=0;i<input_plugs.size();i++)
		delete input_plugs[i];
}


}
