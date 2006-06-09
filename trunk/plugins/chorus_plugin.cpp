//
// C++ Implementation: chorus_plugin
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "chorus_plugin.h"
#include <math.h>
#include "pixmaps/icon_chorus.xpm"

namespace ReShaked {

static SoundPlugin* create_chorus(const SoundPluginInfo *p_info,int p_channels) {
	
	return new ChorusPlugin(p_info,p_channels);
	
}

const SoundPluginInfo *ChorusPlugin::create_info() {
	
	static SoundPluginInfo info;
	
	info.caption="Chorus";
	info.description="Creates Artificial Ensembles";
	info.long_description="The chorus effects creates artificial ensembles, up to 4 extra voices, for any given input.";
	info.unique_ID="INTERNAL_chorus"; 
	info.category="Effects"; 
	info.can_custom_channels=true;
	info.custom_channels.push_back(1);
	info.custom_channels.push_back(2);
	info.custom_channels.push_back(4);
	info.has_internal_UI=false; 
	info.is_synth=false;
	info.xpm_preview=(const char**)icon_chorus_xpm;
	info.creation_func=&create_chorus;
	info.version=1;	
	return &info;
}


int ChorusPlugin::get_input_plug_count() {
	
	return 1;
}
int ChorusPlugin::get_output_plug_count() {
	
	return 1;
	
}
AudioPlug *ChorusPlugin::get_input_plug(int p_index) {
	
	return input_plug;	
}

AudioPlug *ChorusPlugin::get_output_plug(int p_index) {
	
	return output_plug;	
}

int ChorusPlugin::get_port_count() {
	
	return properties.size();
}
Property& ChorusPlugin::get_port(int p_index) {
	
	static LocalProperty error;
	ERR_FAIL_INDEX_V(p_index,properties.size(),error);
	
	return *properties[p_index];
	
}
SoundPlugin::PortType ChorusPlugin::get_port_type(int p_index) {
	
	return TYPE_WRITE;
	
}

void ChorusPlugin::reset() { //sort of useless

	float ring_buffer_max_size=MAX_DELAY_MS+MAX_DEPTH_MS+MAX_WIDTH_MS*get_channels_created();
	ring_buffer_max_size*=2; //just to avoid complications
	ring_buffer_max_size/=1000.0;//convert to seconds
	ring_buffer_max_size*=mix_rate;
	
	int ringbuff_size=ring_buffer_max_size;
	
	int bits=0;
	
	while(ringbuff_size>0) {
		bits++;
		ringbuff_size/=2;
	}
	
	ringbuff_size=1<<bits;
	buffer_mask=ringbuff_size-1;
	buffer_pos=0;
	
	ring_buffer.setup(get_channels_created(), ringbuff_size );
	
	for (int i=0;i<MAX_VOICES;i++) {
		
		voice[i].cycles=0;
		
	}
	
}
/* Setting up */
void ChorusPlugin::set_mixing_rate(float p_mixing_rate) { //sort of useless
	
	mix_rate=p_mixing_rate;
	reset();		
}

/* Processing */

void ChorusPlugin::process(int p_frames) {
	
	if (get_channels_created()>MAX_CHANNELS)
		return;
	
	if (skips_processing()) {
	
		output_plug->get_buffer()->copy_from( input_plug->get_buffer(), p_frames ); 
		return;
	}
	

	float dry_send=dry.get();
	
	/* Enter data into ringbuffer */
	for (int c=0;c<ring_buffer.get_channels();c++) {
	
		float *dst_buff=output_plug->get_buffer()->get_buffer( c );				
		float *src_buff=input_plug->get_buffer()->get_buffer( c );
		float *rb_buff=ring_buffer.get_buffer( c );
			
		for (int i=0;i<p_frames;i++) {
			
			rb_buff[(buffer_pos+i)&buffer_mask]=src_buff[i]; //place input
			dst_buff[i]=dry_send*src_buff[i]; 				
		}
	}
	
	/* process chorus (wet) */
	for (int vc=0;vc<MAX_VOICES;vc++) {
	
		Voice &v=voice[vc];
		
		if (v.level.get()==0)
			continue;
		
		double time_to_mix=(float)p_frames/mix_rate;
		double cycles_to_mix=time_to_mix*v.rate.get();
		
		
		for (int c=0;c<ring_buffer.get_channels();c++) {
			
			unsigned int local_rb_pos=buffer_pos;
			float *dst_buff=output_plug->get_buffer()->get_buffer( c );
			float *rb_buff=ring_buffer.get_buffer( c );
	
			double delay_msec=v.delay.get()+(v.width.get()/(double)ring_buffer.get_channels())*(double)c;
			unsigned int delay_frames=lrint((delay_msec/1000.0)*mix_rate);
			float max_depth_frames=(v.depth.get()/1000.0)*mix_rate;
				
			unsigned long long local_cycles=v.cycles;
			unsigned long long increment=llrint(cycles_to_mix/(double)p_frames*(double)(1<<CYCLES_FRAC));
			
			//check the LFO doesnt read ahead of the write pos
			if ((((int)max_depth_frames)+10)>delay_frames) { //10 as some threshold to avoid precision stuff
				delay_frames+=(int)max_depth_frames-delay_frames;
				delay_frames+=10; //threshold to avoid precision stuff
				
			}
			
			
			
			//low pass filter
			if (v.cutoff.get()==0)
				continue;
			float auxlp=expf(-2.0*M_PI*v.cutoff.get()/mix_rate);
			float c1=1.0-auxlp;
			float c2=auxlp;
			float h=v.filter_h[c];
			if (v.cutoff.get()==v.cutoff.get_max()) {
				c1=1.0; c2=0.0;
			}
			
			//vol modifier
			float vol_modifier=wet.get();
			
			
			//pan - for 2 and 4 channels only.. for mono there's no change
			
			if (get_channels_created()==2) {
				
				if (c==0) {
					
					vol_modifier*=(1.0-v.pan.get())*2.0;
				} else if (c==1) {
					
					vol_modifier*=v.pan.get()*2.0;
				}
				

				
			} else if (get_channels_created()==4) {
				
				float pan=v.pan.get();
				float depth=v.depth.get();
				
				float l=1.0-pan;
				float r=pan;
				
				
				if (c==0) {
					
					vol_modifier*=(1.0-depth)*l*4.0;
					
				} else if (c==1) {
					
					vol_modifier*=(1.0-depth)*r*4.0;
				} else if (c==2) {
					
					vol_modifier*=depth*l*4.0;
				} else if (c==3) {
					
					vol_modifier*=depth*r*4.0;
				}
				
			}
			
			for (int i=0;i<p_frames;i++) {
				
				/** COMPUTE WAVEFORM **/
				
				float phase=(float)(local_cycles&CYCLES_MASK)/(float)(1<<CYCLES_FRAC);
						
				float wave_delay=sinf(phase*2.0*M_PI)*max_depth_frames;
				
				int wave_delay_frames=lrint(floor(wave_delay));
				float wave_delay_frac=wave_delay-(float)wave_delay_frames;
				
				/** COMPUTE RINGBUFFER POS**/
				
				unsigned int rb_source=local_rb_pos;
				rb_source-=delay_frames;

				rb_source-=wave_delay_frames;
				
				/** READ FROM RINGBUFFER, LINEARLY INTERPOLATE */
				
				float val=rb_buff[rb_source&buffer_mask];
				float val_next=rb_buff[(rb_source-1)&buffer_mask];
				
				val+=(val_next-val)*wave_delay_frac;
				
				val=c1*val+h*c2;
				h=val;
				
				/** MIX VALUE TO OUTPUT **/
				
				dst_buff[i]+=val*vol_modifier;
				
				local_cycles+=increment;
				local_rb_pos++;
				
			}
			
			v.filter_h[c]=h;
		}
		
		v.cycles+=llrint(cycles_to_mix*(double)(1<<CYCLES_FRAC));
	}
	
	buffer_pos+=p_frames;
	//in*dry_send;
}



ChorusPlugin::ChorusPlugin(const SoundPluginInfo *p_info,int p_channels) : SoundPlugin(p_info,p_channels) {
	
	if (p_channels<1)
		p_channels=1;
	
	input_plug = new AudioPlug(p_channels,AudioPlug::TYPE_INPUT,this);
	output_plug = new AudioPlug(p_channels,AudioPlug::TYPE_OUTPUT,this);
	
	for (int i=0;i<MAX_VOICES;i++) {
		
		String name_prefix=String("voice_")+String::num(i+1)+"_";
		String caption_prefix=String("Voice ")+String::num(i+1)+"/";
		voice[i].delay.set_all( 12, 0, MAX_DELAY_MS, 0, 1, Property::DISPLAY_SLIDER, name_prefix+"delay",caption_prefix+"Delay","ms");
		voice[i].delay.set_quad_coeff(true);
		voice[i].rate.set_all( 1, 0.1, 20, 1, 0.1, Property::DISPLAY_SLIDER, name_prefix+"rate",caption_prefix+"Rate","hz");
		voice[i].rate.set_quad_coeff(true);
		voice[i].depth.set_all( 0, 0, MAX_DEPTH_MS, 0, 0.1, Property::DISPLAY_SLIDER, name_prefix+"depth",caption_prefix+"Depth","ms");
		voice[i].depth.set_quad_coeff(true);
		voice[i].level.set_all( 0, 0, 4, 0, 0.01, Property::DISPLAY_SLIDER, name_prefix+"level",caption_prefix+"Level","","Off");
		voice[i].width.set_all( 0, 0, MAX_WIDTH_MS, 0, 1, Property::DISPLAY_SLIDER, name_prefix+"width",caption_prefix+"Width","ms");
		voice[i].width.set_quad_coeff(true);
		voice[i].cutoff.set_all( 16000, 1, 16000, 0, 1, Property::DISPLAY_SLIDER, name_prefix+"cutoff",caption_prefix+"Cutoff","hz","","Off");
		voice[i].cutoff.set_quad_coeff(true);
		
		voice[i].pan.set_all( 0.5, 0, 1, 0.5, 0.01, Property::DISPLAY_SLIDER, name_prefix+"pan",caption_prefix+"Pan");
		voice[i].pan_depth.set_all( 0.5, 0, 1, 0.5, 0.01, Property::DISPLAY_SLIDER, name_prefix+"pan_depth",caption_prefix+"Pan Depth");
		
		properties.push_back( &voice[i].delay );		
		properties.push_back( &voice[i].rate );		
		properties.push_back( &voice[i].depth );		
		properties.push_back( &voice[i].level );		
		properties.push_back( &voice[i].width );		
		properties.push_back( &voice[i].cutoff );		
			properties.push_back( &voice[i].pan );		
		properties.push_back( &voice[i].pan_depth );
		
		voice[i].filter_h.resize(p_channels);
		for (int j=0;j<voice[i].filter_h.size();j++)
			voice[i].filter_h[j]=0;
	}
	
	wet.set_all( 0, 0, 1, 0, 0.01, Property::DISPLAY_SLIDER, "wet_send","Wet Send");
	properties.push_back( &wet );		
	dry.set_all( 0, 0, 1, 0, 0.01, Property::DISPLAY_SLIDER, "dry_send","Dry Send");
	properties.push_back( &dry );		
	
	mix_rate=44100;
	reset();
}


ChorusPlugin::~ChorusPlugin(){ 
	
	delete input_plug;
	delete output_plug;
}


}
