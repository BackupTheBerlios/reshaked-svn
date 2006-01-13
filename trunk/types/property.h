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
#include <vector>


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
		DISPLAY_CHECKBOX,
		DISPLAY_VU,
			
	};
	
	
	virtual double get()=0;
	virtual void set(double p_val)=0;
	virtual double get_stepping()=0;
	virtual double get_max()=0;
	virtual double get_min()=0;
	virtual double get_default()=0;
	virtual String get_name()=0;
	virtual String get_caption()=0;
	virtual String get_postfix();
	
	virtual String get_text_value(double p_for_value,bool p_no_postfix=false)=0; //useful for precomputnig ranges
	virtual bool has_text_value()=0;
	
	virtual DisplayMode get_display_mode()=0;
	
	/* helpers */	
	String get_text_value(bool p_no_postfix=false); 
	float get_coeff_value(); ///< return value in range 0 .. 1
	void set_coeff_value(float p_coeff); ///< return value in range 0 .. 1
	float get_value_from_coeff(float p_coeff);
	 
	
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
	
	String max_label;
	String min_label;
public:
	
	
	void set_all(double p_val,double p_begin,double p_end,double p_default,double p_interval, DisplayMode p_dmode,String p_name,String p_caption,String postfix="",String p_min_label="", String p_max_label="");
	
	
	double get();
	void set(double p_val);
	double get_stepping();
	double get_max();
	double get_min();
	double get_default();
	
	
	String get_name();
	String get_caption();
	String get_postfix();
	
	String get_text_value(double p_for_value,bool p_no_postfix=false);
	bool has_text_value();
	
	DisplayMode get_display_mode();
	
	LocalProperty();
	~LocalProperty();

};


class PropertyEditor {
	
	
	double last_value;	
	Property *property;
protected:	
	
	struct Group {
		
		bool locked;
		std::vector<PropertyEditor*> other_editors;	
	};
	
	Group *group;
	
	
	void set(double p_val);
	double get();
	
	
	virtual void changed()=0;
	virtual void config();	
	void release_group();
	
	void (*changed_by_editor)(void*,PropertyEditor*,float);
	void* changed_by_editor_userdata;
public:	
	
	Property *get_property();
		
	void add_to_group(PropertyEditor *p_group);
	
	void check_if_changed();
	void set_property(Property *p_property);
	
	void set_changed_by_editor_callback(void *p_userdata,void (*p_callback)(void*,PropertyEditor*,float));

	PropertyEditor();
	virtual ~PropertyEditor();
	
};


}

#endif
