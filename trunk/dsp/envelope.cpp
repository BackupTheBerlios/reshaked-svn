//
// C++ Implementation: envelope
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "envelope.h"

#include <math.h>

void EnvelopeDetector::recompute() {

	c=expf( -1/(response_time*mix_rate) );
	h=0;
}
void EnvelopeDetector::set_response(float p_time) {

	response_time=p_time;
	recompute();
}
void EnvelopeDetector::set_mix_rate(float p_mix_rate) {

	mix_rate=p_mix_rate;
	recompute();

}

EnvelopeDetector::EnvelopeDetector() {

	response_time=0.01;
	mix_rate=44100;
	recompute();
}


