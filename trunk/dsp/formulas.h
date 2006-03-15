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

namespace ReShaked {

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



}


#endif