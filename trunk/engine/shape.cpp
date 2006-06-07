//
// C++ Implementation: shape
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "shape.h"
#include "dsp/formulas.h"

namespace ReShaked {

void Shape::set_interpolation(InterpolationType p_type) {
	
	interpolation=p_type;
}

Shape::InterpolationType Shape::get_interpolation() {
	
	return interpolation;
}
void Shape::set_max(float p_max) {
	
	max=p_max;
}
float Shape::get_max() {
	
	return max;
}
void Shape::set_min(float p_min) {
	
	min=p_min;
}
float Shape::get_min() {
	
	return min;
}

float Shape::get_value(float p_at) {
	
	int stream_size=get_stream_size();	
	int prev = get_prev_index( p_at );
	int next = get_next_index( p_at );

	if (prev==-1) 
		return 0;
	
	float val=0;
	float lfo_val=0;
	
	if (next==stream_size) {
		val=get_index_value( stream_size -1 );

	} else if (prev==next) { //we got an exact index, just return it, no interp. needed
		val=get_index_value(prev);

	} else {
		
		switch (interpolation) {
		
			case INTERP_NONE: {
						
				val=get_index_value(prev);
						
			} break;
			case INTERP_LINEAR: {
						
				float prev_pos=get_index_pos( prev);
				float prev_val=get_index_value( prev);

						
				float next_pos=get_index_pos( next);
				float next_val=get_index_value( next);
						
				val=prev_val+((p_at-prev_pos)*(next_val-prev_val))/(next_pos-prev_pos); //linear interpolation
			} break;
			case INTERP_CUBIC: {
						
						
						
				float prev_pos=get_index_pos( prev);
				float prev_val=get_index_value( prev);

						
				float pre_prev_val=(prev>0)?get_index_value( prev-1):prev_val;

						
				float next_pos=get_index_pos( next);
				float next_val=get_index_value( next);

						
				float post_next_val=((next+1)<get_stream_size())?get_index_value( next+1):next_val;
						
				float coeff=(p_at-prev_pos)/(next_pos-prev_pos);
						
				val = cubic_interpolate_points(pre_prev_val,prev_val,next_val,post_next_val,coeff);
				
		
						
			} break;
		}
	}	
	
	return val;
}

bool Shape::has_display_inverted() {
	
	return display_inverted;
}
void Shape::set_display_inverted(bool p_inv) {
	
	display_inverted=p_inv;
}

Shape::Shape() {
	
	max=1.0;
	min=0;
	insert( 0, 0 );
	interpolation=INTERP_CUBIC;
	display_inverted=false;
}
Shape::~Shape() {
	
	
}



}
