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
#include "misc_funcs.h"
#include <vector>

#include <math.h>

namespace ReShaked {

/**
	@author red <red@killy>
*/
				    
    
class Property {
			    
	bool quad_coeff; //quadratic coeff
public:
	
	enum DisplayMode {
		DISPLAY_KNOB,
		DISPLAY_SLIDER,
		DISPLAY_SPIN,
		DISPLAY_OPTIONS,
		DISPLAY_CHECKBOX,
		DISPLAY_VU,
			
	};
	
	
	virtual double get()=0;
	virtual void set(double p_val)=0;
	virtual double get_stepping()=0;
	virtual double get_min()=0;
	virtual double get_max()=0;
	virtual double get_default()=0;
	virtual String get_name()=0;
	virtual String get_caption()=0;
	virtual String get_postfix();
	virtual bool is_quad_coeff();
	void set_quad_coeff(bool p_quad);
	virtual String get_text_value(double p_for_value,bool p_no_postfix=false)=0; //useful for precomputnig ranges
	virtual bool has_text_value()=0;
	
	virtual DisplayMode get_display_mode()=0;
	
	/* helpers */	
	virtual String get_text_value(bool p_no_postfix=false); 
	double get_coeff_value(); ///< return value in range 0 .. 1
	void set_coeff_value(double p_coeff); ///< return value in range 0 .. 1
	double get_value_from_coeff(double p_coeff);
	 
	Property();
	virtual ~Property(){};

};


class LocalProperty : public Property {
			    
protected:
	double max;
	double min;
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
	double get_min();
	double get_max();
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


class NoteProperty : public LocalProperty {
	
	
public:	
	
	void config(String p_name,String p_caption);
	String get_text_value(double p_for_value,bool p_no_postfix=false);
	NoteProperty();
	
};

class OptionsProperty : public Property {
	
	int current;
	std::vector<String> options;
	String name;
	String caption;
	String postfix;
public:	
	
	int get_current();
	double get();
	void set(double p_val);
	double get_stepping();
	double get_min();
	double get_max();
	double get_default();
	String get_name();
	String get_caption();
	String get_postfix();
	
	
	String get_text_value(double p_for_value,bool p_no_postfix=false); //useful for precomputnig ranges
	bool has_text_value();
	
	DisplayMode get_display_mode();
	
	void set_all(String p_name,String p_caption,const std::vector<String>& p_options,int p_default=0,String p_postfix="");
	OptionsProperty();
};

template<class T,class Type>
class PropertyClassFunc	: public Property { 

	typedef Type (T::*GetValFunc)()const;
	typedef void (T::*SetValFunc)(Type);
	typedef Type (T::*GetMaxFunc)()const;
	typedef Type (T::*GetMinFunc)()const;
	
	T* instance;
	GetValFunc get_val_func;
	SetValFunc set_val_func;
	GetMaxFunc get_min_func;
	GetMinFunc get_max_func;
	
	double stepping;
	double default_v;
	
	//alternates if funcs are not present
	Type min;
	Type max; 
	
	String name;
	String caption;
	String postfix;
	
	DisplayMode dispmode;
public:	
	
	double get() { return (instance->*get_val_func)(); }
	virtual void set(double p_val) { if (p_val>get_max()) p_val=get_max(); if (p_val<get_min()) p_val=get_min(); (instance->*set_val_func)((Type)p_val); }
	virtual double get_stepping() { return stepping; }
	virtual double get_min() { return (get_min_func)?(instance->*get_min_func)():min; }
	virtual double get_max() { return (get_max_func)?(instance->*get_max_func)():max; }
	virtual double get_default() { return default_v; }
	virtual String get_name() { return name; }
	virtual String get_caption() { return caption; }
	virtual String get_postfix() { return postfix; }
	
	virtual String get_text_value(double p_for_value,bool p_no_postfix=false) { 
		
		int digits=(stepping!=0)?get_decimal_count(stepping):-1;
	
		String res=String::num(p_for_value,digits);
		if (!p_no_postfix)
			res+=postfix;
		return res;
	}
	
	virtual bool has_text_value() { return true; }
	
	virtual DisplayMode get_display_mode() { return dispmode; }
		
	void set_instance(T *p_instance) { instance=p_instance; }
	void set_set_func(SetValFunc p_func) { set_val_func=p_func; }
	void set_get_func(GetValFunc p_func) { get_val_func=p_func; }
	void set_max_func(GetMaxFunc p_func) { get_max_func=p_func; }
	void set_min_func(GetMinFunc p_func) { get_min_func=p_func; }
	void set_min(Type p_min) { min=p_min; get_min_func=NULL; }
	void set_max(Type p_max) { max=p_max; get_max_func=NULL; }
	void config(String p_name,String p_caption,double p_stepping, double p_default,String p_postfix="") { name=p_name; caption=p_caption; stepping=p_stepping; default_v=p_default; postfix=p_postfix; };
	
	PropertyClassFunc() { instance=NULL; stepping=0; default_v=0; get_val_func=NULL; set_val_func=NULL; get_max_func=NULL; get_min_func=NULL; dispmode=DISPLAY_SLIDER; }

};

template<class T>
class PropertyVarPtr : public Property { 

	T* ptr;

	double stepping;
	T default_v;

	//alternates if funcs are not present
	T min;
	T max; 

	String name;
	String caption;
	String postfix;

	DisplayMode dispmode;
public:	

	double get() { return *ptr; }
	virtual void set(double p_val) { if (p_val>get_max()) p_val=get_max(); if (p_val<get_min()) p_val=get_min(); *ptr=(T)p_val; }
	virtual double get_stepping() { return stepping; }
	virtual double get_min() { return (T)min; }
	virtual double get_max() { return (T)max; }
	virtual double get_default() { return default_v; }
	virtual String get_name() { return name; }
	virtual String get_caption() { return caption; }
	virtual String get_postfix() { return postfix; }

	virtual String get_text_value(double p_for_value,bool p_no_postfix=false) { 

		int digits=(stepping!=0)?get_decimal_count(stepping):-1;
		
		String res=String::num(p_for_value,digits);
		if (!p_no_postfix)
			res+=postfix;
		return res;
	}

	virtual bool has_text_value() { return true; }

	virtual DisplayMode get_display_mode() { return dispmode; }

	void config(String p_name,String p_caption,T* p_var_ptr, T p_min, T p_max,double p_stepping, double p_default,String p_postfix="") { name=p_name; caption=p_caption; stepping=p_stepping; default_v=p_default; postfix=p_postfix; ptr=p_var_ptr; min=p_min; max=p_max; };

	PropertyVarPtr() { ptr=NULL; stepping=0; default_v=0;  min=0; max=0; dispmode=DISPLAY_SLIDER; }

};

class PropertyEditorBase {
protected:	
	
public:	
	
	virtual void check_if_changed()=0;	
	virtual void set_changed_by_editor_callback(void *p_userdata,void (*p_callback)(void*,Property*,double))=0;
	virtual ~PropertyEditorBase() {}
	
};

class PropertyEditor : public PropertyEditorBase {
	
	
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
	
	void (*changed_by_editor)(void*,Property*,double);
	void* changed_by_editor_userdata;
public:	
	
	Property *get_property();
		
	void add_to_group(PropertyEditor *p_group);
	
	void check_if_changed();
	void set_property(Property *p_property);
	
	void set_changed_by_editor_callback(void *p_userdata,void (*p_callback)(void*,Property*,double));

	PropertyEditor();
	virtual ~PropertyEditor();
	
};


class MultiPropertyEditor : public PropertyEditorBase {
	
	struct PropertyPtr {
		
		Property *property;
		double last_value;
	};
	
	std::vector<PropertyPtr> property_list;
	
	void (*changed_by_editor)(void*,Property*,double);
	void* changed_by_editor_userdata;
	
protected:	
	
	virtual void changed(int p_which)=0;
	
	void set(int p_property,double p_val);
	double get(int p_property);
	
	void set_property(int p_which,Property *p_property);
	Property * get_property(int p_which);
	
	MultiPropertyEditor(int p_property_count);
	
	
public:		
	
	void set_changed_by_editor_callback(void *p_userdata,void (*p_callback)(void*,Property*,double));
	void check_if_changed();
	
};


}

#endif
