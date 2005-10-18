#ifndef AUTOMATION_H
#define AUTOMATION_H

#include <vector>
#include "types/value_stream.h"
#include "types/typedefs.h"

namespace ReShaked {


struct AutomationValue {
	
	enum InterpolationMethod {
		
		INTERPOLATION_NONE,
		INTERPOLATION_LINEAR,
		INTERPOLATION_SPLINE,		
	};
	
	float value;	
	InterpolationMethod interpolation;
};

class Automation : public ValueStream<Tick,AutomationValue> {

public:	
private:
	
	float default_value; 
	
public:	
	
	void set_default_value(float p_default);
	float get_default_value(); /* defalult value for the automation */
	Automation();

};


}; /* end of namespace */

#endif
