//
// C++ Interface: iir_svf
//
// Description:
//
//
// Author: Juan Linietsky <coding@reduz.com.ar>, (C) 2003
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKED_FILTER_H
#define RESHAKED_FILTER_H

/**
@author Juan Linietsky

*/

#include "typedefs.h"
#include <math.h>

class Filter {
public:

	struct Coeffs {

		float a1,a2;
		float b0,b1,b2;

		bool operator==(const Coeffs &p_rv) { return (FLOATS_EQ(a1,p_rv.a1) && FLOATS_EQ(a2,p_rv.a2) && FLOATS_EQ(b1,p_rv.b1) && FLOATS_EQ(b2,p_rv.b2) && FLOATS_EQ(b0,p_rv.b0) ); }
		Coeffs() { a1=a2=b0=b1=b2=0.0; }
	};

	enum Mode {
		BANDPASS,
		HIGHPASS,
		LOWPASS,
		NOTCH
	};
	
	class Processor { // simple filter processor
		
		Filter * filter;
		Coeffs coeffs;
		float ha1,ha2,hb1,hb2; //history
	public:	
		void set_filter(Filter * p_filter);
		void process(float *p_samples,int p_amount, int p_stride=1);
		void update_coeffs();
		inline void process_one(float& p_sample);
		
		Processor();
	};

private:


	float cutoff;
	float resonance;
	float sampling_rate;
	int stages;
	Mode mode;



public:

	float get_response(float p_freq,Coeffs *p_coeffs);
	
	void set_mode(Mode p_mode);
	void set_cutoff(float p_cutoff);
	void set_resonance(float p_resonance);
	void set_sampling_rate(float p_srate);
	void set_stages(int p_stages); //adjust for multiple stages

	void prepare_coefficients(Coeffs *p_coeffs);

	Filter();

};




/* inline methods */


void Filter::Processor::process_one(float &p_val) {
	
	float pre=p_val;
	p_val = (p_val * coeffs.b0 + hb1 * coeffs.b1  + hb2 * coeffs.b2 + ha1 * coeffs.a1 + ha2 * coeffs.a2); 	
	ha2=ha1;                                                             		
	hb2=hb1;                                                             		
	hb1=pre;
	ha1=p_val;
}



#endif
