//
// C++ Interface: property
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDPROPERTY_H
#define RESHAKEDPROPERTY_H

#include "typedefs.h"
namespace ReShaked {

/**
	@author red <red@killy>
*/
class Property {
			    
public:
	
	enum DisplayMode {
		DISPLAY_KNOB,
		DISPLAY_SLIDER,
		DISPLAY_SPIN,
		DISPLAY_COMBO,
				
	};
	
	
	virtual double get()=0;
	virtual void set(double p_val)=0;
	virtual double get_stepping()=0;
	virtual double get_max()=0;
	virtual double get_min()=0;
	virtual double get_default()=0;
	virtual String get_name()=0;
	virtual String get_caption()=0;
	
	virtual String get_text_value(int p_for_int)=0;
	virtual bool has_text_value()=0;
	
	virtual DisplayMode get_display_mode()=0;
	
	virtual ~Property(){};

};


class LocalProperty : public Property {
			    
	
	double min;
	double max;
	double val;
	double default_v;
	double interval;
	
	DisplayMode display_mode;
	String name;
	String caption;
	String postfix;
public:
	
	
	void set_all(double p_val,double p_begin,double p_end,double p_default,double p_interval, DisplayMode p_dmode,String p_name,String p_caption,String postfix);
	
	
	double get();
	void set(double p_val);
	double get_stepping();
	double get_max();
	double get_min();
	double get_default();
	
	String get_name();
	String get_caption();
	
	String get_text_value(int p_for_int);
	bool has_text_value();
	
	DisplayMode get_display_mode();
	
	
	LocalProperty();
	~LocalProperty();

};

}

#endif
