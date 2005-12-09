//
// C++ Implementation: track
//
// Description: 
//
//
// Author: Juan Linietsky <coding@reduz.com.ar>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "track.h"

namespace ReShaked {



void Track::add_property(String p_path,Property *p_prop) {
	
	
	if (p_path.length()==0 || p_path[0]!='/')
		p_path="/"+p_path;
	
	if (p_path[p_path.length()-1]!='/')
		p_path+="/";
	p_path+=p_prop->get_name();
	
	for (int i=0;i<base_private.property_list.size();i++) {
			
		if (base_private.property_list[i]->path==p_path) {
			//trying to add the same property twice?
			ERR_FAIL_COND( p_prop==base_private.property_list[i]->property );
			//@TODO rename to p_path+(num)
		}
		
	}
	
	PropertyRef * p = new PropertyRef;
	p->path=p_path;
	p->property=p_prop;
	p->automated=NULL; //not automated!
	base_private.property_list.push_back(p);
	
}

Property &Track::swing() {
	
	return base_private.swing;
}
Property &Track::volume() {
	
	return base_private.volume;
	
}
Property &Track::balance() {
	
	return base_private.balance;
	
}

void Track::set_sequencer_event_buffer(const EventBuffer *p_seq) {
	
	base_private.seq_events=p_seq;	
}


const EventBuffer& Track::get_seq_event_buffer() {
	
	return *base_private.seq_events;
}


/* Automations */
int Track::get_idx_by_path(String p_path) {
		
	for (int i=0;i<get_property_count();i++) 
		if (get_property_path(i)==p_path)
			return i;
	 
	return -1;
		
}
void Track::show_automation(String p_path) {

	
	int idx=get_idx_by_path(p_path);
	if (idx==-1)
		return;
			
	if (has_property_automation(idx)) { //already has automation 
		
		base_private.property_list[ idx ]->automated->visible=true;
	} else {
		
		base_private.automations.push_back(new TrackAutomation(p_path,get_property(idx)));
		base_private.property_list[idx]->automated=base_private.automations[ base_private.automations.size()-1 ];
		base_private.automations[ base_private.automations.size()-1 ]->visible=true;
	}
}

void Track::hide_automation(String p_path) {
	
	int idx=get_idx_by_path(p_path);
	if (idx==-1)
		return;
			
	if (!has_property_automation(idx))
		return; //nothing to do
	
	if (base_private.property_list[ idx ]->automated->get_block_count()==0) { //no block
		
		/* This doesnt have any blocks, so simply we screw it */
		
		for (int i=0;i<base_private.automations.size();i++) {
			
			if (base_private.automations[i]==base_private.property_list[ idx ]->automated) {
				
				delete base_private.automations[i]; //it's al doup
				base_private.automations.erase(base_private.automations.begin()+i);
				base_private.property_list[ idx ]->automated=NULL;
				printf("deleted automation since it was empty!\n");
				break;
			}
		}
		
		
	} else {
		
		
		base_private.property_list[ idx ]->automated->visible=false;
		
		
	}
	
}

int Track::get_visible_automation_count() {
	
	int count=0;
	for (int i=0;i<base_private.automations.size();i++) {
		
		if (base_private.automations[i]->visible)
			count++;
	}
	
	return count;
}
Automation *Track::get_visible_automation(int p_index) {
	
	int count=0;
	for (int i=0;i<base_private.automations.size();i++) {
		
		
		if (base_private.automations[i]->visible) {
			if (count==p_index)
				return base_private.automations[i];
			count++;
		}
	}
	
	return NULL;
	
}

/* Some day i'll need this again?
int Track::get_automation_count() {
	
	return base_private.automations.size();	
}
Automation *Track::get_automation(int p_index) {
	
	ERR_FAIL_INDEX_V(p_index,(int)base_private.automations.size(),NULL);
	return base_private.automations[p_index];
}
*/
int Track::get_input_plug_count() {

    return 1;
}
int Track::get_output_plug_count() {


    return 1;
}

AudioPlug *Track::get_input_plug(int p_index) {

    if (p_index!=0)
	return NULL;
    return base_private.input_plug;
}

AudioPlug *Track::get_output_plug(int p_index) {


    if (p_index!=0)
	return NULL;
    return base_private.output_plug;

}

void Track::process(int p_frames) {


	process_track(base_private.input_plug->get_buffer(),base_private.output_plug->get_buffer(),p_frames);
}


int Track::get_property_count() {
	
	return base_private.property_list.size();	
}
Property *Track::get_property(int p_idx) {
	
	ERR_FAIL_INDEX_V(p_idx,get_property_count(),NULL);
	
	return base_private.property_list[p_idx]->property;
}

Automation *Track::get_property_automation(int p_idx) {
	
	ERR_FAIL_INDEX_V(p_idx,get_property_count(),NULL);
	
	return base_private.property_list[p_idx]->automated;
	
}
bool Track::has_property_automation(int p_idx) {
	
	ERR_FAIL_INDEX_V(p_idx,get_property_count(),false);
	
	return (base_private.property_list[p_idx]->automated!=NULL);
	
}
	
bool Track::has_property_visible_automation(int p_idx) {

	ERR_FAIL_INDEX_V(p_idx,get_property_count(),false);

	return (base_private.property_list[p_idx]->automated!=NULL && base_private.property_list[p_idx]->automated->visible);

}

String Track::get_property_path(int p_idx) {
	
	ERR_FAIL_INDEX_V(p_idx,get_property_count(),"");
	
	return base_private.property_list[p_idx]->path;
	
}	

void Track::set_name(String p_name) {
	
	base_private.name=p_name;
}
String Track::get_name() {
	
	return base_private.name;
}

Track::Track(int p_channels,BlockType p_type) : BlockList(p_type) {
	
	base_private.seq_events=NULL;
	base_private.input_plug=new AudioPlug(p_channels,AudioPlug::TYPE_INPUT,this);
	base_private.output_plug=new AudioPlug(p_channels,AudioPlug::TYPE_OUTPUT,this);
	base_private.swing.set_all( 0, 0, 100, 0, 1, Property::DISPLAY_KNOB, "swing","Swing","%","Disabled");
	base_private.volume.set_all( 0, -60, 24, 0, 0.1, Property::DISPLAY_SLIDER, "volume","Volume","dB");
	base_private.balance.set_all( 0, -1.0, 1.0, 0, 0.01, Property::DISPLAY_KNOB,"balance","Balance","","Left","Right");
	
	add_property("track/",&base_private.swing);
	add_property("track/",&base_private.volume);
	add_property("track/",&base_private.balance);
}


Track::~Track()
{

	
	delete base_private.input_plug;
	delete base_private.output_plug;
}


}
