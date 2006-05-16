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
#include <stdio.h>
#include "misc_funcs.h"

namespace ReShaked {



String Property::get_postfix() {
	
	return "";
}

String Property::get_text_value(bool p_no_postfix) {
	
	return get_text_value(get(),p_no_postfix);
}


double Property::get_coeff_value() {
	
	double coeff_val;
	
	double range=get_max()-get_min();
	double val=(get()-get_min());
	
	if (quad_coeff) {
		
		coeff_val=sqrt(val)/sqrt(range);
	} else {
		
		coeff_val=val/range;
	}
	return coeff_val;
}
bool Property::is_write_only() {
	
	return false;
}
void Property::set_coeff_value(double p_coeff) {
	
	set(get_value_from_coeff(p_coeff));
}

bool Property::is_quad_coeff() { return quad_coeff; }
void Property::set_quad_coeff(bool p_quad) { quad_coeff=p_quad; }

double Property::get_value_from_coeff(double p_coeff) {
	
	if (p_coeff==1)
		return get_max(); //avoid precision issues
	
	double range=get_max()-get_min();
	
	if (quad_coeff) {
		
		p_coeff*=sqrt(range);
		p_coeff*=p_coeff;
	} else {
		p_coeff*=range;		
	}
	
	p_coeff+=get_min();
	
	if (get_stepping()!=0) {
		
		p_coeff=floor( p_coeff / get_stepping() + 0.5) * get_stepping();
	}

	return p_coeff;
	
}

Property::Property() { quad_coeff=false; }

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
	
	if (p_val<min)
		p_val=min;
	if (p_val>max)
		p_val=max;

	if (interval!=0) {
	
		p_val=floor( p_val / interval + 0.5) * interval;

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

String LocalProperty::get_text_value(double p_for_value,bool p_no_postfix) {
	
	if (min_label!="" && p_for_value==min)
		return min_label;
	else if (max_label!="" && p_for_value==max)
		return max_label;
	
	int digits=(interval!=0)?get_decimal_count(interval):-1;
	
	String res=String::num(p_for_value,digits);
	if (!p_no_postfix)
		res+=postfix;
	return res;
}
bool LocalProperty::has_text_value() {
	
	return true;
}

String LocalProperty::get_postfix() {
	
	return postfix;
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


/****************************************/

void NoteProperty::config(String p_name,String p_caption) {
	
	name=p_name;
	caption=p_caption;
}

String NoteProperty::get_text_value(double p_for_value,bool p_no_postfix) {
	
	return get_note_str((int)p_for_value);
}

NoteProperty::NoteProperty() {
	
	set_all( 60,0,120,64,1,	DISPLAY_SPIN, "note","Note");
}


/****************************************************************************/



int OptionsProperty::get_current() {
	
	
	return current;
}
double OptionsProperty::get() {
	
	return (double)current;
}
void OptionsProperty::set(double p_val) {
	
	int which=(int)p_val;
	if (which<0)
		which=0;
	if (which>=options.size())
		which=options.size()-1;
	
	current=which;
	
}
double OptionsProperty::get_stepping() {
	
	return 1;
	
}
double OptionsProperty::get_max() {
	
	return options.size()-1;
	
}
double OptionsProperty::get_min() {
	
	return 0;
	
}
double OptionsProperty::get_default() {
	
	return 0;
	
}
String OptionsProperty::get_name() {
	
	
	return name;
}
String OptionsProperty::get_caption() {
	
	
	return caption;
}
String OptionsProperty::get_postfix() {
	
	return postfix;
	
}


String OptionsProperty::get_text_value(double p_for_value,bool p_no_postfix) {
	
	int which=(int)p_for_value;
	if (which<0 || which>=options.size())
		return "";
	
	return options[which] + " " + postfix;
	
}
bool OptionsProperty::has_text_value() {
	
	
	return true;
}

Property::DisplayMode OptionsProperty::get_display_mode() {
	
	
	return DISPLAY_OPTIONS;
}

void OptionsProperty::set_all(String p_name,String p_caption,const std::vector<String>& p_options,int p_default,String p_postfix) {
	
	name=p_name;
	caption=p_caption;
	options=p_options;
	postfix=p_postfix;
	set(p_default);
}

OptionsProperty::OptionsProperty() {}

/****************************************************************************/

Property *PropertyEditor::get_property() {
	
	return property;
	
}
	
void PropertyEditor::set(double p_val) {

	ERR_FAIL_COND(property==NULL);
	double old_val=property->get();
	property->set(p_val);
	last_value=property->get();
	
	if (group && group->locked)
		return;
	
	
	if (group) {
		
		group->locked=true;
		for (int i=0;i<group->other_editors.size();i++) {
			if (group->other_editors[i]==this)
				continue;
			group->other_editors[i]->check_if_changed();
		}
		group->locked=false;
	}	
	if (changed_by_editor)
		changed_by_editor(changed_by_editor_userdata,property,old_val);
		
}

double PropertyEditor::get() {
	
	return property->get();
}
	
void PropertyEditor::check_if_changed() {
	
	if (last_value!=property->get()) {
		last_value=property->get();
		changed();
	}
	
}
void PropertyEditor::set_property(Property *p_property) {
	
	property=p_property;
	config();
	check_if_changed();

}

void PropertyEditor::set_changed_by_editor_callback(void *p_userdata,void (*p_callback)(void*,Property*,double)) {
	
	changed_by_editor_userdata=p_userdata;
	changed_by_editor=p_callback;
}
void PropertyEditor::add_to_group(PropertyEditor *p_group) {
	
	ERR_FAIL_COND(p_group==this);
	
	release_group();

	if (p_group->group==NULL) {
		
		p_group->group = new Group;
		p_group->group->other_editors.push_back(p_group);
		p_group->group->locked=false;
		
	}
	
	group=p_group->group;
	group->other_editors.push_back(this);
}

PropertyEditor::PropertyEditor() {
	
	last_value=-1;
	property=NULL;
	group=NULL;
	changed_by_editor=NULL;
}
	
void PropertyEditor::config() {
	
	
}
void PropertyEditor::release_group() {
	
	if (!group)
		return;
		
	if (group->other_editors.size()<=1) {
		delete group;
	} else {
		
		
		for (int i=0;i<group->other_editors.size();i++) {
			
			if (group->other_editors[i]==this) {
				group->other_editors.erase( group->other_editors.begin() +i );
				break;
			}
		} 
	}	
	
	
	group=NULL;
}
	
PropertyEditor::~PropertyEditor() {
	
	release_group();
	
}


/************** Multi property editor ****************/


void MultiPropertyEditor::set(int p_property,double p_val) {
	
	ERR_FAIL_INDEX(p_property,property_list.size());
	ERR_FAIL_COND(property_list[p_property].property==NULL);
	
	double old_value=property_list[p_property].property->get();
	property_list[p_property].property->set(p_val);
	property_list[p_property].last_value=p_val;
	
	if (changed_by_editor)
		changed_by_editor(changed_by_editor_userdata,property_list[p_property].property,old_value);
}

double MultiPropertyEditor::get(int p_property) {
	
	ERR_FAIL_INDEX_V(p_property,property_list.size(),0);
	ERR_FAIL_COND_V(property_list[p_property].property==NULL,NULL);
	
	return property_list[p_property].property->get();
	
}

void MultiPropertyEditor::set_property(int p_which,Property *p_property) {
	
	ERR_FAIL_INDEX(p_which,property_list.size());
	
	property_list[p_which].property=p_property;
	property_list[p_which].last_value=p_property->get();
	
}
Property * MultiPropertyEditor::get_property(int p_which) {
	
	ERR_FAIL_INDEX_V(p_which,property_list.size(),NULL);
	
	return property_list[p_which].property;
	
}

MultiPropertyEditor::MultiPropertyEditor(int p_property_count) {
	
	property_list.resize(p_property_count);
	for (int i=0;i<p_property_count;i++) {
		
		property_list[i].last_value=-12304128; //random stuff
		property_list[i].property=NULL;
	}
	
	changed_by_editor_userdata=NULL;
	changed_by_editor=NULL;
}

void MultiPropertyEditor::set_changed_by_editor_callback(void *p_userdata,void (*p_callback)(void*,Property*,double)) {
	
	changed_by_editor_userdata=p_userdata;
	changed_by_editor=p_callback;
}

void MultiPropertyEditor::check_if_changed() {
	
	for (int i=0;i<property_list.size();i++) {
			
		if (!property_list[i].property)
			continue;
		
		if (property_list[i].property->get()!=property_list[i].last_value)
			changed(i);
	}
}


}
