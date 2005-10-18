#include "automation.h"





namespace ReShaked {


void Automation::set_default_value(float p_default) {
	
	default_value=p_default;	
}

float Automation::get_default_value() {
	
	return default_value;
}

Automation::Automation() {
	
	
	default_value=0;
}


};