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


void LocalProperty::set_all(double p_val,double p_begin,double p_end,double p_default,double p_interval, DisplayMode p_dmode,String p_name,String p_caption,String p_postfix) {
	
	val=p_val;
	min=p_begin;
	max=p_end;
	default_v=p_default;
	interval=p_interval;
	display_mode=p_dmode;
	name=p_name;
	caption=p_caption;
	postfix=p_postfix;
		
	
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

String LocalProperty::get_text_value(int p_for_int) {
	
	return String(val) + postfix;
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
