//
// C++ Implementation: delay_line_effect
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "delay_line_plugin.h"

#include <math.h>
#include "pixmaps/icon_chorus.xpm"

namespace ReShaked {

static SoundPlugin* create_delay_line_msec(const SoundPluginInfo *p_info,int p_channels) {
	
	return new DelayLinePlugin(p_info,p_channels,false);
	
}

static SoundPlugin* create_delay_line_bpm(const SoundPluginInfo *p_info,int p_channels) {
	
	return new DelayLinePlugin(p_info,p_channels,true);
	
}

const SoundPluginInfo *DelayLinePlugin::create_info_msec() {
	
	static SoundPluginInfo info;
	
	info.caption="Delay Line (msec)";
	info.description="Delay Line With Feedback";
	info.long_description="3-Stages Delay Line with lowpass-enabled feedback.";
	info.unique_ID="INTERNAL_delay_line_msec"; 
	info.category="Effects"; 
	info.can_custom_channels=true;
	info.custom_channels.push_back(1);
	info.custom_channels.push_back(2);
	info.custom_channels.push_back(4);
	info.has_internal_UI=false; 
	info.is_synth=false;
	info.xpm_preview=NULL;//(const char**)icon_chorus_xpm;
	info.creation_func=&create_delay_line_msec;
	info.version=1;	
	return &info;
}

const SoundPluginInfo *DelayLinePlugin::create_info_bpm() {
	
	static SoundPluginInfo info;
	
	info.caption="Delay Line (BPM-sync)";
	info.description="Delay Line With Feedback";
	info.long_description="3-Stages Delay Line with lowpass-enabled feedback. This version syncs tap and feedback delays to BPM.";
	info.unique_ID="INTERNAL_delay_line_bpm"; 
	info.category="Effects"; 
	info.can_custom_channels=true;
	info.custom_channels.push_back(1);
	info.custom_channels.push_back(2);
	info.custom_channels.push_back(4);
	info.has_internal_UI=false; 
	info.is_synth=false;
	info.xpm_preview=NULL;//(const char**)icon_chorus_xpm;
	info.creation_func=&create_delay_line_bpm;
	info.version=1;	
	return &info;
}

int DelayLinePlugin::get_input_plug_count() {
	
	return 1;
}
int DelayLinePlugin::get_output_plug_count() {
	
	return 1;
	
}
AudioPlug *DelayLinePlugin::get_input_plug(int p_index) {
	
	return input_plug;	
}

AudioPlug *DelayLinePlugin::get_output_plug(int p_index) {
	
	return output_plug;	
}

int DelayLinePlugin::get_port_count() {
	
	return properties.size();
}
Property& DelayLinePlugin::get_port(int p_index) {
	
	static LocalProperty error;
	ERR_FAIL_INDEX_V(p_index,properties.size(),error);
	
	return *properties[p_index];
	
}
SoundPlugin::PortType DelayLinePlugin::get_port_type(int p_index) {
	
	return TYPE_WRITE;
	
}

void DelayLinePlugin::reset() { //sort of useless

	float ring_buffer_max_size=MAX_DELAY_MS+100; //add 100ms of extra room, just in case
	ring_buffer_max_size/=1000.0;//convert to seconds
	ring_buffer_max_size*=mix_rate;
	
	int ringbuff_size=ring_buffer_max_size;
	
	int bits=0;
	
	while(ringbuff_size>0) {
		bits++;
		ringbuff_size/=2;
	}
	
	ringbuff_size=1<<bits;
	buffer_buffer_mask=ringbuff_size-1;
	buffer_buffer_pos=0;
	
	ring_buffer.setup(get_channels_created(), ringbuff_size );
	feedback_buffer.setup(get_channels_created(), ringbuff_size );
	
}
/* Setting up */
void DelayLinePlugin::set_mixing_rate(float p_mixing_rate) { //sort of useless
	
	mix_rate=p_mixing_rate;
	reset();		
}

/* Processing */

void DelayLinePlugin::process(int p_frames) {
	
	
	if (skips_processing()) {
	
		output_plug->get_buffer()->copy_from( input_plug->get_buffer(), p_frames ); 
		return;
	}
	

}



DelayLinePlugin::DelayLinePlugin(const SoundPluginInfo *p_info,int p_channels,bool p_bpm_based) : SoundPlugin(p_info,p_channels) {
	
	if (p_channels<1)
		p_channels=1;
	
	input_plug = new AudioPlug(p_channels,AudioPlug::TYPE_INPUT,this);
	output_plug = new AudioPlug(p_channels,AudioPlug::TYPE_OUTPUT,this);
	
	/* Main */
	main_active.set_all( 1, 0, 1, 0, 1, Property::DISPLAY_CHECKBOX, "main_active","Main Active");
	main_level.set_all( 100, 0, 100, 100, 1, Property::DISPLAY_SLIDER, "main_level","Main Level","%");
	main_pan.set_all( 0.5, 0, 1, 0.5, 0.01, Property::DISPLAY_SLIDER, "main_pan","Main Pan","","Left","Right");
	main_pan_depth.set_all( 0.5, 0, 1, 0.5, 0.01, Property::DISPLAY_SLIDER, "main_pan_depth","Main Pan Depth","","Front","Rear");
	
	/* Tap 1 */
	tap_1_active.set_all( 1, 0, 1, 0, 1, Property::DISPLAY_CHECKBOX, "tap_1_active","Tap 1 Active");
	if (p_bpm_based) 
		tap_1_delay.set_all(0.01, 0, 4, 0, 0.1, Property::DISPLAY_SLIDER, "tap_1_delay","Tap 1 Delay");
	else
		tap_1_delay.set_all(250, 1, MAX_DELAY_MS/3, 1, 1, Property::DISPLAY_SLIDER, "tap_1_delay","Tap 1 Delay","ms");
	
	tap_1_level.set_all( 100, 0, 100, 100, 1, Property::DISPLAY_SLIDER, "tap_1_level","Tap 1 Level","%");
	tap_1_pan.set_all( 0.5, 0, 1, 0.5, 0.01, Property::DISPLAY_SLIDER, "tap_1_pan","Tap 1 Pan","","Left","Right");
	tap_1_pan_depth.set_all( 0.5, 0, 1, 0.5, 0.01, Property::DISPLAY_SLIDER, "tap_1_pan_depth","Tap 1 Pan Depth","","Front","Rear");
	
	/* Tap 2 */
	
	tap_2_active.set_all( 1, 0, 1, 0, 1, Property::DISPLAY_CHECKBOX, "tap_2_active","Tap 2 Active");
	if (p_bpm_based) 
		tap_2_delay.set_all(0.01, 0, 4, 0, 0.1, Property::DISPLAY_SLIDER, "tap_2_delay","Tap 2 Delay");
	else
		tap_2_delay.set_all(250, 1, MAX_DELAY_MS/3, 1, 1, Property::DISPLAY_SLIDER, "tap_2_delay","Tap 2 Delay","ms");
	
	tap_2_level.set_all( 100, 0, 100, 100, 1, Property::DISPLAY_SLIDER, "tap_2_level","Tap 2 Level","%");
	tap_2_pan.set_all( 0.5, 0, 1, 0.5, 0.01, Property::DISPLAY_SLIDER, "tap_2_pan","Tap 2 Pan","","Left","Right");
	tap_2_pan_depth.set_all( 0.5, 0, 1, 0.5, 0.01, Property::DISPLAY_SLIDER, "tap_2_pan_depth","Tap 2 Pan Depth","","Front","Rear");
	
	
	/* Feedback */
	feedback_active.set_all( 1, 0, 1, 0, 1, Property::DISPLAY_CHECKBOX, "feedback_active","Feedback Active");
	feedback_level.set_all( 100, 0, 100, 100, 1, Property::DISPLAY_SLIDER, "feedback_level","Feedback Level","%");
	if (p_bpm_based) 
		feedback_delay.set_all(0.01, 0, 4, 0, 0.1, Property::DISPLAY_SLIDER, "feedback_delay","Feedback Delay");
	else
		feedback_delay.set_all(250, 1, MAX_DELAY_MS/3, 1, 1, Property::DISPLAY_SLIDER, "feedback_delay","Feedback Delay","ms");
	
	feedback_lowpass.set_all( 4000, 20, 16000, 4000, 1, Property::DISPLAY_SLIDER, "feedback_lowpass","Feedback Lowpass","hz","","Off");
	
	
	mix_rate=44100;
	reset();
}


DelayLinePlugin::~DelayLinePlugin(){ 
	
	delete input_plug;
	delete output_plug;
}


}
