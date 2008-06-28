//
// C++ Interface: shape
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSHAPE_H
#define RESHAKEDSHAPE_H


#include "value_stream.h"
namespace ReShaked {

/**
	@author Juan Linietsky <reshaked@gmail.com>
*/
class Shape : public ValueStream<float,float> {
public:
			
	enum InterpolationType {
		INTERP_NONE,
		INTERP_LINEAR,
		INTERP_CUBIC,
	};
private:	
	
	InterpolationType interpolation;
	float max;
	float min;
	bool display_inverted;
public:
	
	void set_interpolation(InterpolationType p_type);
	InterpolationType get_interpolation();
	void set_max(float p_max);
	float get_max();
	void set_min(float p_min);
	float get_min();
	bool has_display_inverted();
	void set_display_inverted(bool p_inv);
	
	float get_value(float p_at);
	
	Shape();
	~Shape();

};

}

#endif
