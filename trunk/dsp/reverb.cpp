//
// C++ Implementation: reverb
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "reverb.h"
#include <math.h>

namespace ReShaked {


const float Reverb::comb_tunings[MAX_COMBS]={
	//freeverb comb tunings
	0.025306122448979593,
	0.026938775510204082,
	0.028956916099773241,
	0.03074829931972789,
	0.032244897959183672,
	0.03380952380952381,
	0.035306122448979592,
	0.036666666666666667
};

const float Reverb::allpass_tunings[MAX_ALLPASS]={
	//freeverb allpass tunings	
	0.0051020408163265302,
	0.007732426303854875,
	0.01,
	0.012607709750566893
};



void Reverb::process(float *p_src,float *p_dst,int p_frames) {
	
	if (p_frames>INPUT_BUFFER_SIZE)
		p_frames=INPUT_BUFFER_SIZE;
	
	int predelay_frames=(params.predelay/1000.0)*params.mix_rate;
	if (predelay_frames<10)
		predelay_frames=10;
	if (predelay_frames>=echo_buffer_size)
		predelay_frames=echo_buffer_size-1;
	
	for (int i=0;i<p_frames;i++) {
		
		if (echo_buffer_pos>=echo_buffer_size)
			echo_buffer_pos=0;
		
		int read_pos=echo_buffer_pos-predelay_frames;
		while (read_pos<0)
			read_pos+=echo_buffer_size;
		
		float in=echo_buffer[read_pos]*params.predelay_fb+p_src[i];
		
		echo_buffer[echo_buffer_pos]=in;
		
		input_buffer[i]=in;
	
		p_dst[i]=0; //take the chance and clear this
		echo_buffer_pos++;
	}
	
	for (int i=0;i<MAX_COMBS;i++) {
		
		Comb &c=comb[i];
		
		for (int j=0;j<p_frames;j++) {
			
			if (c.pos>=c.size) //reset this now just in case
				c.pos=0;
			
			float out=c.buffer[c.pos]*c.feedback;
			out=out*(1.0-c.damp)+c.damp_h*c.damp; //lowpass
			c.damp_h=out;
			c.buffer[c.pos]=input_buffer[j]+out;
			p_dst[j]+=out;
			c.pos++;
		}
		
	}


	static const int allpass_feedback=0.7;
	
	for (int i=0;i<MAX_ALLPASS;i++) {
		
		AllPass &a=allpass[i];
		
		for (int j=0;j<p_frames;j++) {
		
			if (a.pos>=a.size)
				a.pos=0;
			
			float aux=a.buffer[a.pos];
			a.buffer[a.pos]=allpass_feedback*aux+p_dst[j];
			p_dst[j]=aux-allpass_feedback*a.buffer[a.pos];
			a.pos++;
			
		}
	}
	
	for (int i=0;i<p_frames;i++) {
		
		p_dst[i]=p_dst[i]*params.wet+p_src[i]*params.dry;
	}
	
}


void Reverb::set_room_size(float p_size) {
	
	params.room_size=p_size;
	update_parameters();	

}
void Reverb::set_damp(float p_damp) {
	
	params.damp=p_damp;
	update_parameters();	

}
void Reverb::set_wet(float p_wet) {
	
	params.wet=p_wet;
	
}

void Reverb::set_dry(float p_dry) {
	
	params.dry=p_dry;	
	
}

void Reverb::set_predelay(float p_predelay) {
	
	params.predelay=p_predelay;	
}
void Reverb::set_predelay_feedback(float p_predelay_fb) {
	
	params.predelay_fb=p_predelay_fb;	
	
}


void Reverb::set_mix_rate(float p_mix_rate) {
	
	params.mix_rate=p_mix_rate;
	configure_buffers();
}

void Reverb::set_extra_spread(float p_sec) {
	
	params.extra_spread=p_sec;
	configure_buffers();
}


void Reverb::configure_buffers() {
	
	clear_buffers(); //clear if necesary
	
	for (int i=0;i<MAX_COMBS;i++) {
		
		Comb &c=comb[i];
		
		int len=lrint((comb_tunings[i]+params.extra_spread)*params.mix_rate);
		if (len<5)
			len=5; //may this happen?
		
		c.buffer = new float[len];
		c.pos=0;
		for (int j=0;j<len;j++)
			c.buffer[j]=0;
		c.size=len;
	}
	
	for (int i=0;i<MAX_ALLPASS;i++) {
		
		AllPass &a=allpass[i];
		
		int len=lrint((allpass_tunings[i]+params.extra_spread)*params.mix_rate);
		if (len<5)
			len=5; //may this happen?
		
		a.buffer = new float[len];
		a.pos=0;
		for (int j=0;j<len;j++)
			a.buffer[j]=0;
		a.size=len;
	}
	
	echo_buffer_size=(int)(((float)MAX_ECHO_MS/1000.0)*params.mix_rate+1.0);
	echo_buffer = new float[echo_buffer_size];
	for (int i=0;i<echo_buffer_size;i++) {
		
		echo_buffer[i]=0;	
	}
	
	echo_buffer_pos=0;
}


void Reverb::update_parameters() {
	
	//more freeverb derived constants
	static const float room_scale = 0.28f;
	static const float room_offset = 0.7f;
	
	for (int i=0;i<MAX_COMBS;i++) {
		
		Comb &c=comb[i];
		c.feedback=room_offset+params.room_size*room_scale;
		if (c.feedback<room_offset)
			c.feedback=room_offset;
		else if (c.feedback>(room_offset+room_scale))
			c.feedback=(room_offset+room_scale);
		
		float auxdmp=params.damp*params.damp;
		c.damp=expf(-2.0*M_PI*auxdmp*10000/params.mix_rate); // 0 .. 10khz
	}
	
}

void Reverb::clear_buffers() {
	
	if (echo_buffer)
		delete[] echo_buffer;
	
	for (int i=0;i<MAX_COMBS;i++) {
		
		if (comb[i].buffer)
			delete[] comb[i].buffer;
		
		comb[i].buffer=0;
		
	}

	for (int i=0;i<MAX_ALLPASS;i++) {
		
		if (allpass[i].buffer)
			delete[] allpass[i].buffer;
		
		allpass[i].buffer=0;
	}

}

Reverb::Reverb() {
	
	params.room_size=0.8;
	params.damp=0.5;
	params.dry=1.0;
	params.wet=0.0;
	params.mix_rate=44100;
	params.extra_spread=0;	
	params.predelay=150;
	params.predelay_fb=0.4;
	
	input_buffer=new float[INPUT_BUFFER_SIZE];
	echo_buffer=0;
	
	configure_buffers();
	update_parameters();	

	
}


Reverb::~Reverb() {
	
	delete[] input_buffer;
	clear_buffers();
}


}
