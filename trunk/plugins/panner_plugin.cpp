//
// C++ Implementation: panner_plugin
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "panner_plugin.h"

#include <math.h>
#include "icon_panner.xpm"

namespace ReShaked {

static SoundPlugin* create_panner(const SoundPluginInfo *p_info,int p_channels) {
	
	return new PannerPlugin(p_info,p_channels);
	
}

const SoundPluginInfo *PannerPlugin::create_info() {
	
	static SoundPluginInfo info;
	
	info.caption="Panner";
	info.description="Adjust Panning or Balance";
	info.long_description="Adjust panning or balance for stereo and quad signals.";
	info.unique_ID="INTERNAL_panner"; 
	info.category="Effects"; 
	info.can_custom_channels=true;
	info.custom_channels.push_back(2);
	info.custom_channels.push_back(4);
	info.has_internal_UI=false; 
	info.is_synth=false;
	info.xpm_preview=(const char**)icon_panner_xpm;
	info.creation_func=&create_panner;
	info.version=1;	
	return &info;
}


int PannerPlugin::get_input_plug_count() {
	
	return 1;
}
int PannerPlugin::get_output_plug_count() {
	
	return 1;
	
}
AudioPlug *PannerPlugin::get_input_plug(int p_index) {
	
	return input_plug;	
}

AudioPlug *PannerPlugin::get_output_plug(int p_index) {
	
	return output_plug;	
}

int PannerPlugin::get_port_count() {
	
	return property_list.size();
}
Property& PannerPlugin::get_port(int p_index) {
	
	static LocalProperty error_prop;
	
	ERR_FAIL_INDEX_V(p_index,property_list.size(),error_prop);
	
	return *property_list[p_index];	
}
SoundPlugin::PortType PannerPlugin::get_port_type(int p_index) {
	
	return TYPE_WRITE;
	
}

/* Setting up */
void PannerPlugin::set_mixing_rate(float p_mixing_rate) { //sort of useless
	
}



/* Processing */

void PannerPlugin::process_2(int p_frames) {
	
	float *src_l=input_plug->get_buffer()->get_buffer( 0 );
	float *src_r=input_plug->get_buffer()->get_buffer( 1 );
	
	float *dst_l=output_plug->get_buffer()->get_buffer( 0 );
	float *dst_r=output_plug->get_buffer()->get_buffer( 1 );
	
	float pan_f= pan.get();
	
	bool pan_mode=(mode.get()==0);
	
	float left=(pan_f<0.5)?1.0:(1.0-(pan_f-0.5)*2.0);
	float right=(pan_f>0.5)?1.0:pan_f*2.0;
	

	if (pan_mode) { //pan mode

		for (int i=0;i<p_frames;i++) {
			
			
			dst_l[i]=src_l[i]*left+src_r[i]*(1.0-right);
			dst_r[i]=src_r[i]*right+src_l[i]*(1.0-left);
			
		}
		
	} else { //balance mode

		
		for (int i=0;i<p_frames;i++) {
			
			dst_l[i]=src_l[i]*left;			
			dst_r[i]=src_r[i]*right;			
			
		}
		
	}
	
	
}

void PannerPlugin::process_4(int p_frames) {
	
	float *src_front_l=input_plug->get_buffer()->get_buffer( 0 );
	float *src_front_r=input_plug->get_buffer()->get_buffer( 1 );
	float *src_rear_l=input_plug->get_buffer()->get_buffer( 2 );
	float *src_rear_r=input_plug->get_buffer()->get_buffer( 3 );
	
	float *dst_front_l=output_plug->get_buffer()->get_buffer( 0 );
	float *dst_front_r=output_plug->get_buffer()->get_buffer( 1 );
	float *dst_rear_l=output_plug->get_buffer()->get_buffer( 2 );
	float *dst_rear_r=output_plug->get_buffer()->get_buffer( 3 );
	
	float pan_f= pan.get();
	float depth_f= depth.get();
	
	bool pan_mode=(mode.get()==0);
	
	float left=(pan_f<0.5)?1.0:(1.0-(pan_f-0.5)*2.0);
	float right=(pan_f>0.5)?1.0:pan_f*2.0;
	float front=(depth_f<0.5)?1.0:(1.0-(depth_f-0.5)*2.0);
	float rear=(depth_f>0.5)?1.0:depth_f*2.0;
	
	float front_l=left*front;
	float front_r=right*front;
	float rear_l=left*rear;
	float rear_r=right*rear;

	if (pan_mode) { //pan mode

		for (int i=0;i<p_frames;i++) {
			
			float inv_front_l=src_front_l[i]*(1.0-front_l);
			float inv_front_r=src_front_r[i]*(1.0-front_r);
			float inv_rear_l=src_rear_l[i]*(1.0-rear_l);
			float inv_rear_r=src_rear_r[i]*(1.0-rear_r);

			
			dst_front_l[i]=src_front_l[i]*front_l +inv_front_r+inv_rear_l+inv_rear_r;
			dst_front_r[i]=inv_front_l+ src_front_r[i]*front_r +inv_rear_l+inv_rear_r;
			dst_rear_l[i]=inv_front_l+inv_front_r+ src_rear_l[i]*rear_l +inv_rear_r;
			dst_rear_r[i]=inv_front_l+inv_front_r+inv_rear_l+ src_rear_r[i]*rear_r;

		}
		
	} else { //balance mode

		
		for (int i=0;i<p_frames;i++) {
			
			dst_front_l[i]=src_front_l[i]*front_l;			
			dst_front_r[i]=src_front_r[i]*front_r;			
			dst_rear_l[i]=src_rear_l[i]*rear_l;			
			dst_rear_r[i]=src_rear_r[i]*rear_r;			
			
		}
		
	}
	
}

void PannerPlugin::process(int p_frames) {
	
	if (skips_processing()) {
	
		output_plug->get_buffer()->copy_from( input_plug->get_buffer(), p_frames ); 
		return;
	}
	
	
	if (get_channels_created()==2)
		process_2(p_frames);
	else if (get_channels_created()==4)
		process_4(p_frames);
	
}


PannerPlugin::PannerPlugin(const SoundPluginInfo *p_info,int p_channels) : SoundPlugin(p_info,p_channels) {
	
	if (p_channels<1)
		p_channels=1;
	
	input_plug = new AudioPlug(p_channels,AudioPlug::TYPE_INPUT,this);
	output_plug = new AudioPlug(p_channels,AudioPlug::TYPE_OUTPUT,this);
	
	pan.set_all( 0.5, 0, 1, 0.5, 0.01, Property::DISPLAY_SLIDER, "pan","Pan","","Left","Right");
	depth.set_all( 0.5, 0, 1, 0.5, 0.01, Property::DISPLAY_SLIDER, "depth","Depth","","Front","Rear");
	mode.set_all( 0, 0, 1, 0, 1, Property::DISPLAY_SLIDER, "mode","Mode","","Pan","Balance");
	
	property_list.push_back(&pan);
	property_list.push_back(&depth);
	property_list.push_back(&mode);
	
}


PannerPlugin::~PannerPlugin() { 
	
	delete input_plug;
	delete output_plug;

	
}


}
