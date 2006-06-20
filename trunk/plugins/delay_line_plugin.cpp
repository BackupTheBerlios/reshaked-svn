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
#include "pixmaps/icon_delay.xpm"
#include "pixmaps/icon_delay_bpm.xpm"

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
	info.xpm_preview=(const char**)icon_delay_xpm;
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
	info.xpm_preview=(const char**)icon_delay_bpm_xpm;
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
	ring_buffer_mask=ringbuff_size-1;
	ring_buffer_pos=0;
	
	ring_buffer.setup(get_channels_created(), ringbuff_size );
	feedback_buffer.setup(get_channels_created(), ringbuff_size );
	
	feedback_buffer_pos=0;	
	
	for (int i=0;i<get_channels_created();i++)
		h[i]=0;
	
}
/* Setting up */
void DelayLinePlugin::set_mixing_rate(float p_mixing_rate) { //sort of useless
	
	mix_rate=p_mixing_rate;
	reset();		
}

/* Processing */


float DelayLinePlugin::get_voice_gain_from_pan(int p_channel,float p_pan,float p_depth) {
	
	if (get_channels_created()==1)
		return 1.0;
	else if (get_channels_created()==2) {
		
		switch (p_channel) {
			
			case 0: return (1.0-p_pan)*2.0; //left
			case 1: return p_pan*2.0; //right
		}
		
	} else if (get_channels_created()==4) {
		
		float pan=p_pan;
		float depth=p_depth;
		
		float l=(1.0-pan)*2.0;
		float r=pan*2.0;;
		
		switch (p_channel) {
			
			case 0: return (1.0-depth)*l*2.0; //frontleft
			case 1: return (1.0-depth)*r*2.0; //frontright
			case 2: return depth*l*2.0; //rearleft
			case 3: return depth*l*2.0;//rearright
			
		}
		
	}
	
	return 1.0;
}


void DelayLinePlugin::process(int p_frames) {
	
	
	if (get_channels_created()!=1 && get_channels_created()!=2 && get_channels_created()!=4)
		return;
	
	if (skips_processing()) {
	
		output_plug->get_buffer()->copy_from( input_plug->get_buffer(), p_frames ); 
		return;
	}
	
	const EventBuffer *ebuff=get_event_buffer();
	
	for (int i=0;i<ebuff->get_event_count();i++) {
		
		const Event *e=ebuff->get_event(i);
				
		if (e->type==Event::TYPE_SEQ_TEMPO) {
			
			bpm=e->param.tempo.bpm;
			
		}
	}
		
	float main_level_f=(main_active.get()>0)?(main_level.get()/100.0):0.0;
	
	/* Tap 1 */
	
	float tap_1_level_f=(tap_1_active.get()>0)?(tap_1_level.get()/100.0):0.0;
	
	int tap_1_delay_frames=0;
	int divisor=1;
	if (tap_1_active.get()>0)
		divisor++;
	if (tap_2_active.get()>0)
		divisor++;
	
	int echo_size_limit=(float)((MAX_DELAY_MS/divisor)/1000.0)*mix_rate;
	if (tap_1_active.get()>0) {
		
		if (bpm_based) {
			
			float beat_size=(mix_rate*60.0)/bpm;
			
			int echo_size=lrintf(beat_size*tap_1_delay.get());
			if (echo_size>echo_size_limit)
				echo_size=echo_size_limit;
	
			tap_1_delay_frames=echo_size;
		} else {
			
			tap_1_delay_frames=lrintf((tap_1_delay.get()/1000.0)*mix_rate);
		}
	}
	
	/* Tap 2 */
	
	float tap_2_level_f=(tap_2_active.get()>0)?(tap_2_level.get()/100.0):0.0;
	
	int tap_2_delay_frames=tap_1_delay_frames;
	
	if (tap_2_active.get()>0) {
		
		if (bpm_based) {
			
			float beat_size=(mix_rate*60.0)/bpm;
			
			int echo_size=lrintf(beat_size*tap_2_delay.get());
			if (echo_size>echo_size_limit)
				echo_size=echo_size_limit;
	
			tap_2_delay_frames+=echo_size;
		} else {
			
			tap_2_delay_frames+=lrintf((tap_2_delay.get()/1000.0)*mix_rate);
		}
	}
	
	float feedback_level_f=(feedback_active.get()>0)?(feedback_level.get()/100.0):0.0;
	
	int feedback_delay_frames=tap_2_delay_frames;
	
	
	if (feedback_active.get()>0) {
		
		if (bpm_based) {
			
			float beat_size=(mix_rate*60.0)/bpm;
			
			int echo_size=lrintf(beat_size*feedback_delay.get());
			if (echo_size>echo_size_limit)
				echo_size=echo_size_limit;
			
	
			feedback_delay_frames+=echo_size;
		} else {
			
			feedback_delay_frames+=lrintf((feedback_delay.get()/1000.0)*mix_rate);
		}
	}
	
	if (feedback_delay_frames>feedback_buffer.get_size())
		feedback_delay_frames=feedback_buffer.get_size();
	
	// feedback lowpass here
	float lpf_c=expf(-2.0*M_PI*feedback_lowpass.get()/mix_rate); // 0 .. 10khz
	float lpf_ic=1.0-lpf_c;
		
	
	
	for (int j=0;j<get_channels_created();j++) {
		
		
		float main_gain=get_voice_gain_from_pan(j,main_pan.get(),main_pan_depth.get())*main_level_f;
		float tap_1_gain=get_voice_gain_from_pan(j,tap_1_pan.get(),tap_1_pan_depth.get())*tap_1_level_f;
		float tap_2_gain=get_voice_gain_from_pan(j,tap_2_pan.get(),tap_2_pan_depth.get())*tap_2_level_f;
		
		float *src=input_plug->get_buffer()->get_buffer( j );
		float *dst=output_plug->get_buffer()->get_buffer( j );
		float *rb_buf=ring_buffer.get_buffer( j ); //ringbuffer
		float *fb_buf=feedback_buffer.get_buffer( j ); //feedback buffer
		
		int aux_ring_buffer_pos=ring_buffer_pos;
		int aux_feedback_pos=feedback_buffer_pos;
		
		for (int i=0;i<p_frames;i++) {
		

			rb_buf[aux_ring_buffer_pos&ring_buffer_mask]=src[i];
			
			float main_val=src[i]*main_gain;
			float tap_1_val=rb_buf[(aux_ring_buffer_pos-tap_1_delay_frames)&ring_buffer_mask]*tap_1_gain;
			float tap_2_val=rb_buf[(aux_ring_buffer_pos-tap_2_delay_frames)&ring_buffer_mask]*tap_2_gain;
			
			float out=main_val+tap_1_val+tap_2_val;
			
			out+=fb_buf[ aux_feedback_pos ];
			
			//apply lowpass and feedback gain
			float fb_in=undenormalise(out*feedback_level_f*lpf_ic+lpf_c*h[j]);
			fb_buf[ aux_feedback_pos ]=fb_in;

			
			dst[i]=out;
			
			aux_ring_buffer_pos++;
			
			if ( (++aux_feedback_pos) >= feedback_delay_frames )
				aux_feedback_pos=0;
		
		}
		
	}
	
		
	ring_buffer_pos+=p_frames;
	if (feedback_delay_frames==0)
		feedback_buffer_pos=0;
	else
		feedback_buffer_pos=(feedback_buffer_pos+p_frames)%feedback_delay_frames;
	
	

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
	tap_1_active.set_all( 0, 0, 1, 0, 1, Property::DISPLAY_CHECKBOX, "tap_1_active","Tap 1 Active");
	if (p_bpm_based) 
		tap_1_delay.set_all(1, 0, 4, 0, 0.1, Property::DISPLAY_SLIDER, "tap_1_delay","Tap 1 Delay");
	else
		tap_1_delay.set_all(250, 1, MAX_DELAY_MS/(MAX_TAPS+1), 1, 1, Property::DISPLAY_SLIDER, "tap_1_delay","Tap 1 Delay","ms");
	
	tap_1_level.set_all( 50, 0, 100, 100, 1, Property::DISPLAY_SLIDER, "tap_1_level","Tap 1 Level","%");
	tap_1_pan.set_all( 0.5, 0, 1, 0.5, 0.01, Property::DISPLAY_SLIDER, "tap_1_pan","Tap 1 Pan","","Left","Right");
	tap_1_pan_depth.set_all( 0.5, 0, 1, 0.5, 0.01, Property::DISPLAY_SLIDER, "tap_1_pan_depth","Tap 1 Pan Depth","","Front","Rear");
	
	/* Tap 2 */
	
	tap_2_active.set_all( 0, 0, 1, 0, 1, Property::DISPLAY_CHECKBOX, "tap_2_active","Tap 2 Active");
	if (p_bpm_based) 
		tap_2_delay.set_all(1, 0, 4, 0, 0.1, Property::DISPLAY_SLIDER, "tap_2_delay","Tap 2 Delay");
	else
		tap_2_delay.set_all(250, 1, MAX_DELAY_MS/(MAX_TAPS+1), 1, 1, Property::DISPLAY_SLIDER, "tap_2_delay","Tap 2 Delay","ms");
	
	tap_2_level.set_all( 60, 0, 100, 100, 1, Property::DISPLAY_SLIDER, "tap_2_level","Tap 2 Level","%");
	tap_2_pan.set_all( 0.5, 0, 1, 0.5, 0.01, Property::DISPLAY_SLIDER, "tap_2_pan","Tap 2 Pan","","Left","Right");
	tap_2_pan_depth.set_all( 0.5, 0, 1, 0.5, 0.01, Property::DISPLAY_SLIDER, "tap_2_pan_depth","Tap 2 Pan Depth","","Front","Rear");
	
	
	/* Feedback */
	feedback_active.set_all( 1, 0, 1, 0, 1, Property::DISPLAY_CHECKBOX, "feedback_active","Feedback Active");
	feedback_level.set_all( 40, 0, 99, 99, 1, Property::DISPLAY_SLIDER, "feedback_level","Feedback Level","%");
	if (p_bpm_based) 
		feedback_delay.set_all(1, 0, 4, 0, 0.1, Property::DISPLAY_SLIDER, "feedback_delay","Feedback Delay");
	else
		feedback_delay.set_all(250, 1, MAX_DELAY_MS/(MAX_TAPS+1), 1, 1, Property::DISPLAY_SLIDER, "feedback_delay","Feedback Delay","ms");
	
	feedback_lowpass.set_all( 4000, 20, 16000, 4000, 1, Property::DISPLAY_SLIDER, "feedback_lowpass","Feedback Lowpass","hz","","Off");
	
	properties.push_back(&main_active);
	properties.push_back(&main_level);
	properties.push_back(&main_pan);
	properties.push_back(&main_pan_depth);
	
	properties.push_back(&tap_1_active);
	properties.push_back(&tap_1_delay);
	properties.push_back(&tap_1_level);
	properties.push_back(&tap_1_pan);
	properties.push_back(&tap_1_pan_depth);
	
	properties.push_back(&tap_2_active);
	properties.push_back(&tap_2_delay);
	properties.push_back(&tap_2_level);
	properties.push_back(&tap_2_pan);
	properties.push_back(&tap_2_pan_depth);
	
	properties.push_back(&feedback_active);
	properties.push_back(&feedback_delay);
	properties.push_back(&feedback_level);
	properties.push_back(&feedback_lowpass);
	
	h = new float[p_channels];
	bpm_based=p_bpm_based;
	mix_rate=44100;
	bpm=125;
	reset();
}


DelayLinePlugin::~DelayLinePlugin(){ 
	
	delete input_plug;
	delete output_plug;
	delete[] h;
}


}
