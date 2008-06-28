//
// C++ Implementation: adsr
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "adsr.h"

namespace ReShaked {


void ADSR::set_attack(double p_attack) {
	
	attack=p_attack;
}

void ADSR::set_decay(double p_decay) {
	
	decay=p_decay;
}
void ADSR::set_sustain(double p_sustain) {
	
	sustain=p_sustain;
}
void ADSR::set_release(double p_release) {
	
	release=p_release;
}

void ADSR::start() {
	
	sustaining=true;
	pos=0;	
	val=0;
	finished=false;
}

void ADSR::process(double p_amount) {
	
	pos+=p_amount;
	
	if (pos>(attack+decay) && sustaining)
		pos=attack+decay;
	
	double currentpos=pos;
		
	if (currentpos<attack) {
		
		val=currentpos/attack;

	} else {
		currentpos-=attack;
		
		if (currentpos<decay) {
			
			val=1.0-(currentpos/decay)*(1.0-sustain);

		} else if (sustaining) {
			
			val=sustain;
		} else {
				
			currentpos-=decay;
			
			if (currentpos<release ) { 
				
				
				val=sustain-(currentpos/release)*(sustain);
			} else {
				
				val=0;
				finished=true;
			}
		}
	}
	
}

bool ADSR::is_finished() {
	
	return finished;
}
void ADSR::release_begin() {
	
	sustaining=false;
	
}

double ADSR::get_val() {
	
	return val;	
}

ADSR::ADSR() {
	
	pos=val=attack=sustain=decay=release=0;
	sustaining=false;
	finished=false;
}


ADSR::~ADSR()
{
}


}
