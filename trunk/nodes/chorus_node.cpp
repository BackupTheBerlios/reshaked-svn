
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
#include "chorus_node.h"
#include <math.h>

#include "dsp/formulas.h"


void ChorusNode::mix_rate_changed() {

	
	if (ring_buffers) {
	
		for (int i=0;i<get_channels();i++)
			delete[] ring_buffers;
		delete[] ring_buffers;
	
	}
	
	
	ring_buffers = new sample_t*[get_channels()];
	
	float ring_buffer_max_size=MAX_DELAY_MS+MAX_DEPTH_MS+MAX_WIDTH_MS*get_channels();
	ring_buffer_max_size*=2; //just to avoid complications
	ring_buffer_max_size/=1000.0;//convert to seconds
	ring_buffer_max_size*=get_mix_rate();
	
	int ringbuff_size=ring_buffer_max_size;
	
	int bits=0;
	
	while(ringbuff_size>0) {
		bits++;
		ringbuff_size/=2;
	}
	
	ringbuff_size=1<<bits;
	buffer_mask=ringbuff_size-1;
	buffer_pos=0;
	
	for (int i=0;i<get_channels();i++) {
		ring_buffers[i]=new sample_t[ringbuff_size];
		for (int j=0;j<ringbuff_size;j++)
			ring_buffers[i][j]=0;
	}
	
}

AudioNode *ChorusNode::creation_func(int p_channels,const AudioNodeInfo *p_info) {

	return new ChorusNode(p_channels,p_info);
}


const AudioNodeInfo *ChorusNode::get_creation_info() {

	static AudioNodeInfo _info;
	_info.caption="Stereo Chorus";
	_info.short_caption="Chorus";
	_info.description="Multi-Voiced, Mono/Stereo/Quad Chorus";
	_info.unique_ID="INTERNAL_ChorusNode";
	_info.creation_func=&ChorusNode::creation_func;
	_info.icon_string="node_chorus";
	_info.category="Effects";
	_info.custom_channels.push_back(1);
	_info.custom_channels.push_back(2);
	_info.custom_channels.push_back(4);
	
	return &_info;
}

void ChorusNode::process(const ProcessInfo& p_info) {

	if (get_channels()>MAX_CHANNELS)
		return;
		
	float dry_send=dry.get();
	
	/* Enter data into ringbuffer */
	for (int c=0;c<get_channels();c++) {
	
		sample_t *dst_buff=get_audio_buffer( PORT_OUT, 0 , c );			
		sample_t *src_buff=get_audio_buffer( PORT_IN, 0 , c );			
		sample_t *rb_buff=ring_buffers[c];
			
		for (int i=0;i<p_info.audio_buffer_size;i++) {
			
			rb_buff[(buffer_pos+i)&buffer_mask]=src_buff[i]; //place input
			dst_buff[i]=dry_send*src_buff[i]; 				
		}
	}
	
	/* process chorus (wet) */
	for (int vc=0;vc<MAX_VOICES;vc++) {
	
		Voice &v=voice[vc];
		
		if (v.level()==0)
			continue;
		
		double time_to_mix=(float)p_info.audio_buffer_size/get_mix_rate();
		double cycles_to_mix=time_to_mix*v.rate();
		
		
		for (int c=0;c<get_channels();c++) {
			
			unsigned int local_rb_pos=buffer_pos;
			sample_t *dst_buff=get_audio_buffer( PORT_OUT, 0, c );
			sample_t *rb_buff=ring_buffers[c];
	
			double delay_msec=v.delay()+(v.width()/(double)get_channels())*(double)c;
			unsigned int delay_frames=(int)((delay_msec/1000.0)*get_mix_rate());
			float max_depth_frames=(v.depth()/1000.0)*get_mix_rate();
				
			unsigned long long local_cycles=v.cycles;
			unsigned long long increment=(int)(cycles_to_mix/(double)p_info.audio_buffer_size*(double)(1<<CYCLES_FRAC));
			
			//check the LFO doesnt read ahead of the write pos
			if ((((int)max_depth_frames)+10)>delay_frames) { //10 as some threshold to avoid precision stuff
				delay_frames+=(int)max_depth_frames-delay_frames;
				delay_frames+=10; //threshold to avoid precision stuff
				
			}
				
			//low pass filter
			if (v.cutoff()==0)
				continue;
			float auxlp=exp(-2.0*M_PI*v.cutoff()/get_mix_rate());
			float c1=1.0-auxlp;
			float c2=auxlp;
			float h=v.filter_h[c];
			if (v.cutoff()==v.cutoff.get_max()) {
				c1=1.0; c2=0.0;
			}
			
			//vol modifier
			float vol_modifier=wet();
			
			
			//pan - for 2 and 4 channels only.. for mono there's no change
			
			if (get_channels()==2) {
				
				if (c==0) {
					
					vol_modifier*=(1.0-v.pan())*2.0;
				} else if (c==1) {
					
					vol_modifier*=v.pan()*2.0;
				}
				

				
			} else if (get_channels()==4) {
				
				float pan=v.pan();
				float depth=v.depth();
				
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
			
			for (int i=0;i<p_info.audio_buffer_size;i++) {
				
				/** COMPUTE WAVEFORM **/
				
				float phase=(float)(local_cycles&CYCLES_MASK)/(float)(1<<CYCLES_FRAC);
						
				float wave_delay=sinf(phase*2.0*M_PI)*max_depth_frames;
				
				int wave_delay_frames=fast_floor(wave_delay);
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
		
		v.cycles+=(int)(cycles_to_mix*(double)(1<<CYCLES_FRAC));
	}
	
	buffer_pos+=p_info.audio_buffer_size;
	//in*dry_send;	
	
}

ChorusNode::ChorusNode(int p_instanced_channels,const AudioNodeInfo *p_info) : HL_AudioNode(p_instanced_channels,p_info) {

	if (p_instanced_channels<1)
		p_instanced_channels=1;
	
	add_audio_port("Input",PORT_IN);
	add_audio_port("Output",PORT_OUT);
	
	for (int i=0;i<MAX_VOICES;i++) {
		
		String caption_prefix=String("Voice")+String::num(i+1)+"/";
		voice[i].delay.set_all( 12, 0, MAX_DELAY_MS, 1, HL_ControlPort::HINT_RANGE, caption_prefix+"Delay","ms");
		voice[i].delay.set_exp_range(true);
		voice[i].rate.set_all( 1, 0.1, 20, 0.1, HL_ControlPort::HINT_RANGE, caption_prefix+"Rate","hz");
		voice[i].rate.set_exp_range(true);
		voice[i].depth.set_all( 0, 0, MAX_DEPTH_MS, 0.1, HL_ControlPort::HINT_RANGE,caption_prefix+"Depth","ms");
		voice[i].depth.set_exp_range(true);
		voice[i].level.set_all( 0, 0, 4, 0.01, HL_ControlPort::HINT_RANGE, caption_prefix+"Level","","Off");
		voice[i].phase.set_all( 0, 0, 3.14, 0.01, HL_ControlPort::HINT_RANGE, caption_prefix+"Phase","rad");
		voice[i].width.set_all( 0, 0, MAX_WIDTH_MS, 1, HL_ControlPort::HINT_RANGE, caption_prefix+"Width","ms");
		voice[i].width.set_exp_range(true);
		voice[i].cutoff.set_all( 16000, 1, 16000, 1, HL_ControlPort::HINT_RANGE, caption_prefix+"LPF","hz","","Off");
		voice[i].cutoff.set_exp_range(true);
		
		voice[i].pan.set_all( 0.5, 0, 1, 0.01, HL_ControlPort::HINT_RANGE, caption_prefix+"Pan");
		voice[i].pan_depth.set_all( 0.5, 0, 1, 0.01, HL_ControlPort::HINT_RANGE,caption_prefix+"PanDepth");
		
		add_control_port( PORT_IN, &voice[i].delay );		
		add_control_port( PORT_IN, &voice[i].rate );		
		add_control_port( PORT_IN, &voice[i].depth );		
		add_control_port( PORT_IN, &voice[i].level );		
		add_control_port( PORT_IN, &voice[i].phase );		
		add_control_port( PORT_IN, &voice[i].width );		
		add_control_port( PORT_IN, &voice[i].cutoff );		
		add_control_port( PORT_IN, &voice[i].pan );		
		add_control_port( PORT_IN, &voice[i].pan_depth );
		
		voice[i].filter_h.resize(p_instanced_channels);
		
		for (int j=0;j<voice[i].filter_h.size();j++)
			voice[i].filter_h[j]=0;
	}
	
	wet.set_all( 0, 0, 1, 0.01, HL_ControlPort::HINT_RANGE, "Wet");
	add_control_port( PORT_IN, &wet );		
	dry.set_all( 0, 0, 1, 0.01, HL_ControlPort::HINT_RANGE, "Dry");
	add_control_port( PORT_IN, &dry );			
	
	ring_buffers=NULL;
	
	wet.set_visible(true);
	
}


ChorusNode::~ChorusNode() {
		
	if (ring_buffers) {
	
		for (int i=0;i<get_channels();i++)
			delete[] ring_buffers;
		delete[] ring_buffers;
	
	}
}

