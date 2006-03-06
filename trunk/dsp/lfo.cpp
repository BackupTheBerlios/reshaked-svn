//
// C++ Implementation: lfo
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "lfo.h"
#include <math.h>
#include "typedefs.h"

namespace ReShaked {

/* Random table for fast LFO randomization */
static float random_table[256]={0.3506,0.4434,0.1025,0.9123,0.7791,0.2490,0.0723,0.2305,0.7900,0.8002,0.1816,0.9170,0.4863,0.7451,0.4717,0.3184,0.1211,0.6895,0.0820,0.9922,0.7764,0.4766,0.4756,0.8877,0.8652,0.3379,0.2949,0.5967,0.5498,0.9053,0.1963,0.9004,0.8496,0.2988,0.0127,0.9287,0.5479,0.0859,0.1592,0.3379,0.2861,0.3408,0.2549,0.7725,0.0859,0.7275,0.0918,0.2070,0.4170,0.1738,0.2002,0.1934,0.6514,0.6758,0.0820,0.5166,0.0137,0.3770,0.1133,0.5635,0.2822,0.3096,0.4648,0.6318,0.6094,0.4775,0.0605,0.1572,0.5635,0.2197,0.4961,0.8506,0.5605,0.7510,0.6230,0.6475,0.4785,0.7148,0.8545,0.8965,0.8896,0.0547,0.0898,0.5410,0.7305,0.1719,0.0576,0.7451,0.5488,0.1709,0.3086,0.8320,0.4805,0.7734,0.9639,0.0898,0.2520,0.0254,0.2480,0.8154,0.2451,0.7441,0.6660,0.8066,0.4951,0.2900,0.4541,0.9746,0.0049,0.3086,0.8711,0.8945,0.3643,0.9609,0.4355,0.0947,0.1338,0.4932,0.8398,0.6826,0.6641,0.1494,0.5146,0.1455,0.9229,0.4795,0.2354,0.1748,0.5049,0.4834,0.9912,0.7500,0.2275,0.6572,0.5566,0.7236,0.9473,0.0107,0.6982,0.9531,0.3203,0.5693,0.8477,0.6846,0.5303,0.2842,0.7793,0.6641,0.7773,0.6201,0.3477,0.4424,0.7695,0.8623,0.5879,0.6924,0.3418,0.8232,0.8682,0.8467,0.3076,0.8594,0.5977,0.5352,0.5166,0.1543,0.2588,0.4648,0.1660,0.9570,0.4180,0.4863,0.5264,0.2656,0.1709,0.0576,0.5498,0.9502,0.7217,0.3281,0.5703,0.0693,0.7705,0.3398,0.9326,0.3584,0.0332,0.2744,0.1816,0.9014,0.1221,0.4893,0.7607,0.7197,0.0254,0.2773,0.8740,0.2842,0.7422,0.0400,0.2422,0.1602,0.5264,0.7686,0.4268,0.6973,0.8262,0.9766,0.6484,0.5488,0.3047,0.2188,0.6182,0.0752,0.5596,0.5508,0.4336,0.5928,0.8262,0.6162,0.4941,0.9482,0.1055,0.2549,0.6680,0.1309,0.5322,0.5420,0.4160,0.2754,0.5830,0.6582,0.4355,0.1094,0.4268,0.8623,0.8076,0.2539,0.8398,0.4561,0.8027,0.1445,0.6748,0.4209,0.2207,0.2344,0.9727,0.6543,0.8271,0.7988,0.2705,0.3213,0.7471,0.3770,0.5762,0.4150};

void LFO::set_delay(unsigned long p_delay) {
	
	delay=p_delay;
}
void LFO::set_rate(double p_rate) {
	
	rate=p_rate;
}
void LFO::set_depth(double p_depth) {
	
	depth=p_depth;
}
void LFO::set_phase(double p_phase) {
	
	phase=p_phase;
}
void LFO::set_random_depth(double p_amount) {
	
	random_depth=p_amount;
	
}
void LFO::set_mode(Mode p_mode) {
	
	mode=p_mode;
}

unsigned long LFO::get_delay() const {
	
	return delay;	
}
double LFO::get_rate() const {
	
	return rate;
}
double LFO::get_depth() const {
	
	return depth;
}
double LFO::get_phase() const {
	
	return phase;
}
double LFO::get_random_depth() const {
	
	return random_depth;
}

LFO::Mode LFO::get_mode() const {
	
	return mode;
}

void LFO::set_rate_unit_size(unsigned long p_size) {
	
	rate_unit_size=p_size;
}


double LFO::get_value(unsigned long p_for_time)  const {
	
	if (depth==0)
		return 0.0;
	
	if (p_for_time<delay)
		return 0.0;
	
	p_for_time-=delay;
	
	
	unsigned long cycle_size=lrintf( (double)rate_unit_size  / rate );
	
	p_for_time+=lrint(phase*cycle_size);
			
	double wave_phase=(double)( p_for_time % cycle_size ) / (double)cycle_size;
	
	double wave_val=0;
	
	switch(mode) {
		
		case MODE_SINE: {
			
			wave_val=sin(wave_phase*M_PI*2.0);
			
		} break;
		case MODE_SAW_UP: {
			
			
			wave_val=fmodf(wave_phase+0.5,1)*2.0-1.0;
		} break;
		case MODE_SAW_DOWN: {
			
			wave_val=(1.0-fmodf(wave_phase+0.5,1))*2.0-1.0;
			
		} break;
		case MODE_SQUARE: {
			
			if (wave_phase>0.5)
				wave_val=1.0;
			else
				wave_val=-1.0;
		} break;
	}
	
	if (random_depth>0) { // process RANDOM
		
		
		unsigned int cycle_count=p_for_time/cycle_size;
		double rnd_val=wave_val*random_table[(cycle_count+lrintf(random_depth*1024))&0xFF];
		wave_val=random_depth*rnd_val+(1.0-random_depth)*wave_val;
		
	}
	
	wave_val*=depth;
	
	return wave_val;
		
}


LFO::LFO() {
	
	delay=0;
	rate=1;
	depth=0;
	phase=0;
	random_depth=0;
	rate_unit_size=1;
}


LFO::~LFO() {
	
	
	
	
}


}
