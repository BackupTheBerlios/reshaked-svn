//
// C++ Interface: envelope
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef ENVELOPE_H
#define ENVELOPE_H

#include "typedefs.h"

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

/* one pole envelope detector */

class EnvelopeDetector {

	float mix_rate;
	float response_time;
	
	sample_t h;
	float c;
	
	void recompute();
public:

	
	void set_response(float p_time);
	void set_mix_rate(float p_mix_rate);
	
	_FORCE_INLINE_ void process(sample_t& p_sample) {
	
		p_sample+=c*(h-p_sample);		
		h=p_sample;
	}

	EnvelopeDetector();	

};



#endif
