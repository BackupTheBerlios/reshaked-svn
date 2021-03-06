//
// C++ Interface: formulas
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef FORMULAS_H
#define FORMULAS_H

#include <math.h>
#include "typedefs.h"

#if __BIG_ENDIAN_
#define iman_ 1
#else
#define iman_ 0
#endif

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

union _diunion {

	double d;
	int i;
};

inline int fast_floor(double val) {
	
	val = val + (68719476736.0*1.5);
	_diunion diu;
	diu.d=val;
	return ((&diu.i)[iman_]>>16);
}

static inline float note_to_freq(float p_note) {
	
	return 440.0*pow(2.0,(p_note-69.0)/12.0); // 440 tuning, change to 415 if you feel baroque	
}

static inline Tick get_swing_pos(Tick p_src_tick,int p_swing_base,double p_swing) {
	
	Tick tick_frac_size=TICKS_PER_BEAT/p_swing_base;
	Tick tick_frac=p_src_tick%tick_frac_size;
	Tick tick_debased=p_src_tick-tick_frac;
	
	
	Tick swing_split=(int)( ((1.0+p_swing)*(double)tick_frac_size )/2.0 );
	
	if (tick_frac<=swing_split) {
		
		tick_frac=tick_frac*(tick_frac_size/2)/swing_split;
		
	} else {
				
		tick_frac=tick_frac_size-tick_frac; //invert
		
		Tick diff=tick_frac_size-swing_split;
		if (diff==0)
			tick_frac=tick_frac_size;
		else
			tick_frac=tick_frac*(tick_frac_size/2)/diff;
		
		tick_frac=tick_frac_size-tick_frac; //revert
		
	}
	
	return tick_frac+tick_debased;
}

static inline double energy_to_db(double p_nrg) {
	
	return log( p_nrg ) * 8.6858896380650365530225783783321;
}

static inline double db_to_energy(double p_db) {
	
	return exp( p_db * 0.11512925464970228420089957273422 );	
}

static inline double vel_sens_function(double p_velocity,double p_velocity_sens) {

	double x;
	x=pow(8,p_velocity_sens);
	if ( (p_velocity_sens>0.99) && (p_velocity>0.99) )
		return 1.0;
	else
		return pow(p_velocity,x);

}



static inline double cubic_interpolate_points(double h0,double h1,double h2,double h3, double coeff)
{
	double a0,a1,a2,a3,coeff2;

	coeff2 = coeff*coeff;
	a0 = h3 - h2 - h0 + h1;
	a1 = h0 - h1 - a0;
	a2 = h2 - h0;
	a3 = h1;

	return(a0*coeff*coeff2+a1*coeff2+a2*coeff+a3);
}



static inline double linear_interpolate_points(double p_x,double p_h,double p_x2,double p_h2, double p_coeff) {
	
	return p_h+p_coeff*(p_h2-p_h)/(p_x2-p_x);
	
}


/**
 * Scale volume according to note
 * @param p_note Linear from 0..1, usually converted from range 0 .. 127
 * @param p_scale -1 .. 1
 * @return 0 .. 1
 */
static inline float get_volume_scale(float p_note,float p_scale) { 

	float scale_mult=1;
	float vs=p_scale;
	float fnote=p_note;

	if (vs>0.0) {

		vs=1.0-vs;

		if (vs>=0.5) {
			float vsc=(vs-0.5)*2.0;

			scale_mult=(1.0-fnote)*(1.0-vsc) + vsc;
		} else {
			float vsc=vs*2.0;
			if (fnote>vsc)
				scale_mult=0;
			else if (vsc==0.0)
				scale_mult=0;
			else
				scale_mult=1.0-(fnote/vsc);
		}

	} else if (vs<0.0) {

		vs=1.0-fabsf(vs);

		if (vs>=0.5) {
			float vsc=(vs-0.5)*2.0;

			scale_mult=(fnote)*(1.0-vsc) + vsc;
		} else {
			float vsc=vs*2.0;
			if ((1.0-fnote)>vsc)
				scale_mult=0;
			else if (vsc==0.0)
				scale_mult=0;
			else
				scale_mult=1.0-((1.0-fnote)/vsc);
		}
	}

	return scale_mult;
}

// to avoid denormals
#define UNDENORMAL_OFFSET 1e-25


#endif
