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
#include "engine/audio_control.h"

namespace ReShaked {


void Track::add_property(String p_visual_path,Property *p_prop,TrackAutomation *p_automation,int p_pos) {
	
	AudioControl::mutex_lock();
	
	if (p_visual_path.length()==0 || p_visual_path[0]!='/')
		p_visual_path="/"+p_visual_path;
	
	if (p_visual_path[p_visual_path.length()-1]!='/')
		p_visual_path+="/";
	p_visual_path+=p_prop->get_name();
	
	for (int i=0;i<base_private.property_list.size();i++) {
			
		if (p_prop==base_private.property_list[i]->property) {
			
			ERR_PRINT( "p_prop==base_private.property_list[i]->property" );
			AudioControl::mutex_unlock();
			return;
			//@TODO rename to p_visual_path+(num)
		}
		
	}
	
	PropertyRef * p = new PropertyRef;
	p->visual_path=p_visual_path;
	p->property=p_prop;
	p->automation=p_automation?p_automation:(new TrackAutomation(p_prop));
	if (p_pos<0 && p_pos>=base_private.property_list.size()) {
		
		base_private.property_list.push_back(p);	
	} else {
		
		base_private.property_list.insert(base_private.property_list.begin()+p_pos,p);	
		
	}
	
	
	AudioControl::mutex_unlock();	
}

void Track::set_sequencer_event_buffer(const EventBuffer *p_seq) {
	
	base_private.seq_events=p_seq;	
}


const EventBuffer& Track::get_seq_event_buffer() {
	
	return *base_private.seq_events;
}


void Track::property_show_automation(int p_idx) {

	ERR_FAIL_INDEX(p_idx,get_property_count());
	int idx=p_idx;
			
	AudioControl::mutex_lock();
	
	base_private.property_list[ idx ]->automation->visible=true;
	
	
	AudioControl::mutex_unlock();
	
}

void Track::property_hide_automation(int p_idx) {
	
	
	ERR_FAIL_INDEX(p_idx,get_property_count());
	int idx=p_idx;
	
	AudioControl::mutex_lock();
	
	
	base_private.property_list[ idx ]->automation->visible=false;
	
	/*
	if (base_private.property_list[ idx ]->automation->get_block_count()==0) { //no block
		
		// This doesnt have any blocks, so simply we screw it 
		
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
*/
	AudioControl::mutex_unlock();
	
}

int Track::get_visible_automation_count() {
	
	int count=0;
	for (int i=0;i<base_private.property_list.size();i++) {
		
		if (base_private.property_list[i]->automation->visible)
			count++;
	}
	
	return count;
}
Automation *Track::get_visible_automation(int p_index) {
	
	int count=0;
	for (int i=0;i<base_private.property_list.size();i++) {
		
		
		if (base_private.property_list[i]->automation->visible) {
			if (count==p_index)
				return base_private.property_list[i]->automation;
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

void Track::feed_input(int p_frames) {
	
	AudioBuffer *proxy_buff=base_private.input_proxy.get_output_plug(0)->get_buffer();
	proxy_buff->copy_from( base_private.input_plug->get_buffer(), p_frames );
}
void Track::read_output(int p_frames) {
	
	AudioBuffer *output_buff=base_private.output_plug->get_buffer();
	output_buff->copy_from( base_private.output_proxy.get_input_plug(0)->get_buffer(), p_frames );
}

void Track::process(int p_frames) {

	base_private.plugin_graph.process( p_frames );
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
	
	return base_private.property_list[p_idx]->automation;
	
}
	
bool Track::has_property_visible_automation(int p_idx) {

	ERR_FAIL_INDEX_V(p_idx,get_property_count(),false);

	return (base_private.property_list[p_idx]->automation->visible);

}

String Track::get_property_visual_path(int p_idx) {
	
	ERR_FAIL_INDEX_V(p_idx,get_property_count(),"");
	
	return base_private.property_list[p_idx]->visual_path;
	
}	

void Track::set_name(String p_name) {
	
	base_private.name=p_name;
}
String Track::get_name() {
	
	return base_private.name;
}

String Track::get_caption() {
	
	return get_name();	
}

bool Track::is_visible() {
	
	return true;
}

GlobalProperties &Track::get_global_props() {
	
	return *base_private.global_props;
}

int Track::get_plugin_count() {
	
	return base_private.sound_plugins.size();
}
SoundPlugin *Track::get_plugin(int p_index) {
	
	ERR_FAIL_INDEX_V(p_index,get_plugin_count(),NULL);
	return base_private.sound_plugins[p_index];
		
}
void Track::add_plugin(PluginInsertData* p_plugin) {
	
	AudioControl::mutex_lock();
	
	/* insert */
	if (p_plugin->pos==-1) {
		base_private.sound_plugins.push_back(p_plugin->plugin);
	} else {
		base_private.sound_plugins.insert( base_private.sound_plugins.begin()+p_plugin->pos,p_plugin->plugin );
	}
	
	/* determine path */
	String path=p_plugin->plugin->get_info()->is_synth?"Synths/":"Effects/";
	
	String desired_name=p_plugin->plugin->get_info()->caption;
	
	
	path+=desired_name+"/";
		
	
	/* add properties and automations */
	for (int i=0;i<p_plugin->plugin->get_port_count();i++) {
		if (p_plugin->plugin->get_port_type(i)==SoundPlugin::TYPE_READ)
			continue;
		
		TrackAutomation *automation=NULL;
		int pos=-1;
		Property *prop=&p_plugin->plugin->get_port(i);
		foreach(I,p_plugin->automated_tracks) {
		
			if (I->automation->get_property()==prop) {
				automation=I->automation;
				pos=I->pos;
				break;
			}
		}
		ERR_CONTINUE(automation==NULL); //no automation for prop?!
		add_property(path,&p_plugin->plugin->get_port(i),automation,pos );	
	}
	

	/* Finally, restore connections */
	
	base_private.plugin_graph.add_node( p_plugin->plugin, &p_plugin->connections );
	
	AudioControl::mutex_unlock();
	
}

void Track::remove_plugin(int p_pos,PluginInsertData* p_plugin_recovery) {
	
	ERR_FAIL_INDEX(p_pos,get_plugin_count());
	
	AudioControl::mutex_lock();
	
	SoundPlugin *node=get_plugin( p_pos );
	
	/* Save Connections */
	
	p_plugin_recovery->plugin=node;
	p_plugin_recovery->pos=p_pos;
	base_private.plugin_graph.erase_node( base_private.plugin_graph.get_node_index(node) , &p_plugin_recovery->connections );
	
	
	/* Erase properties and save automations*/
	
	for (int i=0;i<base_private.property_list.size();i++) {
	
		int port=-1;
		for (int j=0;j<node->get_port_count();j++) {
			
			if (node->get_port_type(j)==SoundPlugin::TYPE_READ)
				continue;
			if (&node->get_port(j)==base_private.property_list[i]->property) {
				port=j;
				break;
			}
		}
	
		if (port==-1)
			continue;
		

		PluginInsertData::AutomationTrack atdata;
		atdata.pos=i;
		atdata.automation=base_private.property_list[i]->automation;
		p_plugin_recovery->automated_tracks.push_front(atdata);
		
		delete base_private.property_list[i]; //erase property ref
		base_private.property_list.erase( base_private.property_list.begin() + i ); //and erase from array
		i--;
		
	}
	
	base_private.sound_plugins.erase( base_private.sound_plugins.begin() + p_pos );
	
	AudioControl::mutex_unlock();
}

AudioGraph& Track::get_plugin_graph() {
	
	return base_private.plugin_graph;
}

int Track::get_channels() {
	
	return base_private.channels;
}

Track::Track(int p_channels,BlockType p_type,GlobalProperties *p_global_props) : BlockList(p_type) {
	
	base_private.channels=p_channels;
	base_private.seq_events=NULL;
	base_private.input_plug=new AudioPlug(p_channels,AudioPlug::TYPE_INPUT,this);
	base_private.output_plug=new AudioPlug(p_channels,AudioPlug::TYPE_OUTPUT,this);
	
	/* input <-> output flipping because we are OUTSIDE the graph */
	base_private.input_proxy.add_output_plug( p_channels );
	base_private.output_proxy.add_input_plug( p_channels);
	base_private.plugin_graph.add_node( &base_private.input_proxy );
	base_private.plugin_graph.add_node( &base_private.output_proxy );
	base_private.input_proxy.set_process_method( this, (void (ProxyNodeBase::*)(int))&Track::feed_input );
	base_private.output_proxy.set_process_method( this, (void (ProxyNodeBase::*)(int)) &Track::read_output );
	
	
	
}


Track::~Track()
{

	
	delete base_private.input_plug;
	delete base_private.output_plug;
}


}
