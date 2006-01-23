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
#include <math.h>

namespace ReShaked {


void Track::add_property(String p_visual_path,Property *p_prop,TrackAutomation *p_automation,int p_pos) {
	
	AudioControl::mutex_lock();
	
	if (p_visual_path.length()==0 || p_visual_path[0]!='/')
		p_visual_path="/"+p_visual_path;
	
	if (p_visual_path[p_visual_path.length()-1]!='/')
		p_visual_path=p_visual_path+"/";
	
	p_visual_path+=p_prop->get_caption();
	
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
	
	rebuild_active_automation_cache();
		
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
	rebuild_active_automation_cache();	
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
	AudioBuffer *track_output_buff=base_private.output_proxy.get_input_plug(0)->get_buffer();
	
	float max_nrg=0;
	
	for (int i=0;i<output_buff->get_channels();i++) {
		
		float *src=output_buff->get_buffer(i);
		float *dst=track_output_buff->get_buffer(i);
		for (int j=0;j<p_frames;j++) {
			
			dst[j]=src[j]*base_private.audio.volume_ratio;
			float abs_dst=fabsf(dst[j]);
			if (abs_dst>max_nrg)
				base_private.audio.highest_energy=max_nrg;
		}
	}

	if (max_nrg>base_private.audio.highest_energy)
		base_private.audio.highest_energy=max_nrg;
}

float Track::read_highest_energy() {
	
	float highest_nrg=base_private.audio.highest_energy;
	base_private.audio.highest_energy=0;
	return highest_nrg;
}
void Track::process_automations(bool p_use_current_tick_to) {
	
	if (base_private.song_playback->get_status()==SongPlayback::STATUS_PLAY) {
		/* update automations */
		for (int i=0;i<base_private.active_automation_cache.size();i++) {
			
			Tick tick = p_use_current_tick_to?base_private.song_playback->get_current_tick_to():base_private.song_playback->get_current_tick_from();
			base_private.active_automation_cache[i]->apply(tick);
		}
		
	}
	
}
void Track::process(int p_frames) {

	process_automations();
	track_pre_process(p_frames);
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
void Track::plugin_added_notify(SoundPlugin *p_plugin) {};

void Track::move_plugin_left(int p_index) {
	
	ERR_FAIL_INDEX(p_index,base_private.sound_plugins.size());
	ERR_FAIL_COND(p_index==0);

	SWAP(base_private.sound_plugins[p_index-1],base_private.sound_plugins[p_index]);
}
void Track::move_plugin_right(int p_index) {
	
	ERR_FAIL_INDEX(p_index,(base_private.sound_plugins.size()-1));
	SWAP(base_private.sound_plugins[p_index+1],base_private.sound_plugins[p_index]);
	
}


void Track::validate_plugin_duplicate(SoundPlugin *p_plugin) {
	
	bool exists=false;
	for (int i=0;i<get_plugin_count();i++) {
			
		if (get_plugin(i)->get_info()->unique_ID==p_plugin->get_info()->unique_ID && get_plugin(i)->get_duplicate()==p_plugin->get_duplicate()) {
			exists=true;
			break;
		}
		
	}
	
	if (!exists)
		return;
	
	int free_duplicate=-1;
	
	while(true) {
		
		free_duplicate++;
		bool found_duplicate=false;
		for (int i=0;i<get_plugin_count();i++) {
			if (get_plugin(i)->get_info()->unique_ID==p_plugin->get_info()->unique_ID && get_plugin(i)->get_duplicate()==free_duplicate) 
				found_duplicate=true;
		}

		if (found_duplicate)
			continue;
		
		break;
	}
	
	p_plugin->set_duplicate( free_duplicate );
}

void Track::add_plugin(PluginInsertData* p_plugin) {
	
	AudioControl::mutex_lock();
	
	/* valudate plugin duplicate (before insert) */
	
	validate_plugin_duplicate(p_plugin->plugin);		
	
	/* insert */
	if (p_plugin->pos==-1 || p_plugin->pos==base_private.sound_plugins.size()) {
		base_private.sound_plugins.push_back(p_plugin->plugin);
	} else {
		base_private.sound_plugins.insert( base_private.sound_plugins.begin()+p_plugin->pos,p_plugin->plugin );
	}
			
	
	/* determine path */
	String path=p_plugin->plugin->get_info()->is_synth?"Synths/":"Effects/";
	
	String desired_name=p_plugin->plugin->get_caption();
	
	
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
	
	rebuild_active_automation_cache();
	plugin_added_notify( p_plugin->plugin );
	
	p_plugin->plugin->set_mixing_rate( get_song_playback()->get_mix_rate() );
	
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
	
	rebuild_active_automation_cache();
		
	AudioControl::mutex_unlock();
}

void Track::update_plugins_mix_rate() {
	
	AudioControl::mutex_lock();
		
	for (int i=0;i<base_private.sound_plugins.size();i++) {
		
		base_private.sound_plugins[i]->set_mixing_rate( get_song_playback()->get_mix_rate() );
	}
	AudioControl::mutex_unlock();
	
}

AudioGraph& Track::get_plugin_graph() {
	
	return base_private.plugin_graph;
}

int Track::get_channels() {
	
	return base_private.channels;
}

void Track::rebuild_active_automation_cache() {
	
	AudioControl::mutex_lock();
	
	base_private.active_automation_cache.clear();
	
	for (int i=0;i<get_property_count();i++) {
		
		if (has_property_visible_automation(i) || base_private.property_list[i]->automation->get_block_count() > 0 ) {
			base_private.active_automation_cache.push_back( base_private.property_list[i]->automation);
		
		} 
		
	}
	
	AudioControl::mutex_unlock();
	
}

SongPlayback* Track::get_song_playback() {
	
	return base_private.song_playback;	
}

void Track::track_pre_process(int p_frames) {
	
}

void Track::reset_automations() {
	
	for (int i=0;i<get_property_count();i++) {
		
		Automation *a=get_property_automation(i);
		a->get_property()->set( a->get_initial_value() );
	}
}

AudioNode *Track::get_node_at_visual_pos(int p_pos) {
	if (p_pos<0)
		return NULL;
	if (p_pos==0)
		return &base_private.input_proxy;	
	else 
		p_pos--;
	if (p_pos<base_private.sound_plugins.size())
		return base_private.sound_plugins[p_pos];
	
	if (p_pos==base_private.sound_plugins.size())
		return &base_private.output_proxy;
	
	return NULL;
	
}
Track::Track(int p_channels,BlockType p_type,GlobalProperties *p_global_props,SongPlayback *p_song_playback) : BlockList(p_type) {
	
	base_private.channels=p_channels;
	base_private.seq_events=NULL;
	base_private.input_plug=new AudioPlug(p_channels,AudioPlug::TYPE_INPUT,this);
	base_private.output_plug=new AudioPlug(p_channels,AudioPlug::TYPE_OUTPUT,this);
	base_private.song_playback=p_song_playback;
	
	/* input <-> output flipping because we are OUTSIDE the graph */
	base_private.input_proxy.add_output_plug( p_channels );
	base_private.output_proxy.add_input_plug( p_channels);
	base_private.plugin_graph.add_node( &base_private.input_proxy );
	base_private.plugin_graph.add_node( &base_private.output_proxy );
	base_private.input_proxy.set_process_method( this, (void (ProxyNodeBase::*)(int))&Track::feed_input );
	base_private.output_proxy.set_process_method( this, (void (ProxyNodeBase::*)(int)) &Track::read_output );
	base_private.output_proxy.set_system(true);
	base_private.input_proxy.set_system(true);
	base_private.output_proxy.set_caption("Play");
	base_private.input_proxy.set_caption("Rec");
	
	
	
	base_private.audio.highest_energy=0;
	base_private.audio.volume_ratio=1;
	
	base_private.plugin_graph.set_visual_node_order( this );
	
}


Track::~Track()
{

	
	delete base_private.input_plug;
	delete base_private.output_plug;
}


}
