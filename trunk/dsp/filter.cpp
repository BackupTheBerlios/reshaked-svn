//
// C++ Implementation: iir_svf
//
// Description:
//
//
// Author: Juan Linietsky <coding@reduz.com.ar>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "filter.h"
#include <math.h>
#include <stdio.h>


void Filter::set_mode(Mode p_mode) {

	mode = p_mode;
}
void Filter::set_cutoff(float p_cutoff) {

	cutoff=p_cutoff;
}
void Filter::set_resonance(float p_resonance) {

	resonance=p_resonance;
}


void Filter::set_sampling_rate(float p_srate) {

	sampling_rate=p_srate;
}


void Filter::prepare_coefficients(Coeffs *p_coeffs) {

	
	double final_cutoff=(cutoff>=(sampling_rate/2))?(sampling_rate/2-1):cutoff;
	if (final_cutoff<20) //avoid crapness
		final_cutoff=20; //i dont allow less than this
	
	
		
	double omega=2.0*M_PI*final_cutoff/sampling_rate;
	
	double sin_v=sin(omega);
	double cos_v=cos(omega);
	
	float Q=resonance;
	if (Q<=0.0) { 
		Q=0.0001;
	}
	

	if (mode==BANDPASS) 
		Q*=2.0;
	
	if (stages>1) {
		
		Q=(Q>1.0 ? pow(Q,1.0/stages) : Q);
		
	}
	double alpha = sin_v/(2*Q);
	
	double a0 = 1.0 + alpha;
	
	switch (mode) {


		case LOWPASS: {	    
	
			p_coeffs->b0=  (1.0 - cos_v)/2.0 ;
			p_coeffs->b1=   1.0 - cos_v    ;
			p_coeffs->b2=  (1.0 - cos_v)/2.0 ;
			p_coeffs->a1=  -2.0*cos_v      ;
			p_coeffs->a2=   1.0 - alpha  ;
		} break;
	
	
		case HIGHPASS: {
	
			p_coeffs->b0 =  (1.0 + cos_v)/2.0;
			p_coeffs->b1 = -(1.0 + cos_v);
			p_coeffs->b2 =  (1.0 + cos_v)/2.0;
			p_coeffs->a1 =  -2.0*cos_v;
			p_coeffs->a2 =   1.0 - alpha;
		} break;
		
		case BANDPASS: {
	
			p_coeffs->b0 =  alpha*sqrt(Q+1);
			p_coeffs->b1 =  0.0 ;
			p_coeffs->b2 =  -alpha*sqrt(Q+1);
			p_coeffs->a1 =  -2.0*cos_v;
			p_coeffs->a2 =   1.0 - alpha;
		} break;
	
		case NOTCH: {
	
			p_coeffs->b0 =   1.0;
			p_coeffs->b1 =  -2.0*cos_v;
			p_coeffs->b2 =   1.0;
			p_coeffs->a1 =  -2.0*cos_v;
			p_coeffs->a2 =   1.0 - alpha;
		} break;
    
    };

    p_coeffs->b0/=a0;
    p_coeffs->b1/=a0;
    p_coeffs->b2/=a0;
    p_coeffs->a1/=0.0-a0;
    p_coeffs->a2/=0.0-a0;
    
    //undenormalise
    p_coeffs->b0=undenormalise(p_coeffs->b0);
    p_coeffs->b1=undenormalise(p_coeffs->b1);
    p_coeffs->b2=undenormalise(p_coeffs->b2);
    p_coeffs->a1=undenormalise(p_coeffs->a1);
    p_coeffs->a2=undenormalise(p_coeffs->a2);
    
}

void Filter::set_stages(int p_stages) { //adjust for multiple stages

	stages=p_stages;
}

/* Fouriertransform kernel to obtain response */

float Filter::get_response(float p_freq,Coeffs *p_coeffs) {
	
	float freq=p_freq / sampling_rate * M_PI * 2.0f;
	
	float cx=p_coeffs->b0,cy=0.0;

	cx += cos(freq) * p_coeffs->b1;
	cy -= sin(freq) * p_coeffs->b1;
	cx += cos(2*freq) * p_coeffs->b2;
	cy -= sin(2*freq) * p_coeffs->b2;
        
    
	float H=cx*cx+cy*cy;
	cx=1.0;
	cy=0.0;
    

	cx -= cos(freq) * p_coeffs->a1;
	cy += sin(freq) * p_coeffs->a1;
	cx -= cos(2*freq) * p_coeffs->a2;
	cy += sin(2*freq) * p_coeffs->a2;

    
	H=H/(cx*cx+cy*cy);
	return H;
}


Filter::Filter() {


	sampling_rate=44100;
	resonance=0.5;
	cutoff=5000;
	mode=LOWPASS;
	stages=1;
}

Filter::Processor::Processor() {
	
	set_filter(NULL);
	
}

void Filter::Processor::set_filter(Filter * p_filter) {
	
	ha1=ha2=hb1=hb2=0;
	filter=p_filter;	
}

void Filter::Processor::update_coeffs() {
	
	if (!filter)
		return;
	
	filter->prepare_coefficients(&coeffs);
	
}

void Filter::Processor::process(float *p_samples,int p_amount, int p_stride) {
	
	if (!filter)
		return;
	
	for (int i=0;i<p_amount;i++) {
		
		process_one(*p_samples);
		p_samples+=p_stride;
	}
	
	
}
