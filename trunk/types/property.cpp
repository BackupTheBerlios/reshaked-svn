//
// C++ Implementation: property
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "property.h"
#include <math.h>
#include <stdio.h>

namespace ReShaked {

String Property::get_text_value() {
	
	return get_text_value(get());
}


float Property::get_coeff_value() {
	
	return (get()-get_min())/(get_max()-get_min());
}

void Property::set_coeff_value(float p_coeff) {
	
	set(get_value_from_coeff(p_coeff));
}

float Property::get_value_from_coeff(float p_coeff) {
	
	if (p_coeff==1)
		return get_max(); //avoid precision issues
	
	p_coeff*=get_max()-get_min();		
	p_coeff+=get_min();
	
	if (get_stepping()!=0) {
		p_coeff-=get_min();
		p_coeff=p_coeff-fmod(p_coeff,get_stepping());
		p_coeff+=get_min();
	}

	return p_coeff;
	
}

void LocalProperty::set_all(double p_val,double p_begin,double p_end,double p_default,double p_interval, DisplayMode p_dmode,String p_name,String p_caption,String p_postfix,String p_min_label, String p_max_label) {
	
	val=p_val;
	min=p_begin;
	max=p_end;
	default_v=p_default;
	interval=p_interval;
	display_mode=p_dmode;
	name=p_name;
	caption=p_caption;
	postfix=p_postfix;
		
	min_label=p_min_label;
	max_label=p_max_label;
	
}


double LocalProperty::get() {
	
	return val;	
}
void LocalProperty::set(double p_val) {
	
	if (val<min || val>max)
		return;
	if (interval!=0) {
		p_val-=min;
		p_val=p_val-fmod(p_val,interval);
		p_val+=min;
	}
	val=p_val;
				
				
	
}
double LocalProperty::get_stepping() {
	
	return interval;
}
double LocalProperty::get_max() {
	
	return max;
}
double LocalProperty::get_min() {
	
	return min;
}
double LocalProperty::get_default() {
	
	return default_v;
}

String LocalProperty::get_name() {
	
	return name;
}
String LocalProperty::get_caption() {
	
	return caption;
}

String LocalProperty::get_text_value(double p_for_value) {
	
	if (min_label!="" && p_for_value==min)
		return min_label;
	else if (max_label!="" && p_for_value==max)
		return max_label;
	
	int digits=-1;
	
	if (interval!=0 || interval!=floorf(interval)) {
		
		digits=0;
		double step_aux=interval;
		do {
			step_aux*=10;
			//step_aux+=1e-10;
			digits++;
			if (step_aux-floor(step_aux) < 1e-6 )
				break;
			
		
		} while (true);
	} 
	
	return String::num(p_for_value,digits) + postfix;
}
bool LocalProperty::has_text_value() {
	
	return true;
}

LocalProperty::DisplayMode LocalProperty::get_display_mode() {
	
	return display_mode;	
}	

LocalProperty::LocalProperty() {
	
	min=0;
	max=1;
	val=0;
	default_v=0;
	interval=0.01;
	
	display_mode=DISPLAY_KNOB;
	
}


LocalProperty::~LocalProperty() {
	
	
}

}
