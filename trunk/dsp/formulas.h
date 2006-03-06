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


namespace ReShaked {


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






}


#endif