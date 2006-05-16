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
#include "dsp/formulas.h"
namespace ReShaked {


void Track::add_property(String p_visual_path,Property *p_prop,TrackAutomation *p_automation,int p_pos,bool p_builtin) {
	
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
	p->builtin=p_builtin;
	
	if (p_pos<0 && p_pos>=base_private.property_list.size()) {
		
		base_private.property_list.push_back(p);	
	} else {
		
		base_private.property_list.insert(base_private.property_list.begin()+p_pos,p);	
		
	}
	
	
	AudioControl::mutex_unlock();	
}

void Track::remove_property(Property *p_prop) {
	
	AudioControl::mutex_lock();
	
	for (int i=0;i<get_property_count();i++) {
		
		if (get_property(i)==p_prop) {
			base_private.property_list.erase(		base_private.property_list.begin()+i );
			break;
		}
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
	
	if (base_private.audio.mute)
		return; //process all, just dont read from output
	
	AudioBuffer *track_output_buff=base_private.output_plug->get_buffer();
	AudioBuffer *output_buff=base_private.output_proxy.get_input_plug(0)->get_buffer();
	
	
	
	for (int i=0;i<output_buff->get_channels();i++) {
		
		float *src=output_buff->get_buffer(i);
		float *dst=track_output_buff->get_buffer(i);
		
		float max_chan_nrg=0;
		
		for (int j=0;j<p_frames;j++) {
			
			dst[j]=src[j]*base_private.audio.volume_ratio;
			float abs_dst=fabsf(dst[j]);
			if (abs_dst>max_chan_nrg)
				max_chan_nrg=abs_dst;
				
		}
		
		if (max_chan_nrg>base_private.audio.highest_energy[i])
			base_private.audio.highest_energy[i]=max_chan_nrg;
	}

}

float Track::read_highest_energy(int p_which) {
	
	ERR_FAIL_INDEX_V(p_which,base_private.audio.highest_energy.size(),0);
	
	float highest_nrg=base_private.audio.highest_energy[p_which];
	base_private.audio.highest_energy[p_which]=0;
	return highest_nrg;
}

void Track::set_mute(bool p_mute) {
	
	base_private.audio.mute=p_mute;
}
bool Track::is_mute() {
	
	return base_private.audio.mute;
}

void Track::process_automations_at_tick(Tick p_tick,bool p_process_swing) {
	
	/* update automations */
	Tick swinged_tick=get_swinged_tick( p_tick ); //do it here for speed
	
	for (int i=0;i<base_private.active_automation_cache.size();i++) {
		
		Tick local=p_tick;
		if (base_private.active_automation_cache[i]->is_swing_follow_enabled() && p_process_swing)
			local=swinged_tick;
		base_private.active_automation_cache[i]->apply(local);
	}
}

void Track::offline_process_automations(Tick p_tick) {
	
	if (base_private.song_playback->get_status()==SongPlayback::STATUS_PLAY)
		return; //if playing, then dont do this
	
	AudioControl::mutex_lock();
	process_automations_at_tick(p_tick);
	AudioControl::mutex_unlock();
	
}

void Track::process_automations(bool p_use_current_tick_to) {
	
	if (base_private.song_playback->get_status()!=SongPlayback::STATUS_PLAY)
		return; // only process if playing

	Tick tick = p_use_current_tick_to?base_private.song_playback->get_current_tick_to():base_private.song_playback->get_current_tick_from();
	
	process_automations_at_tick( tick,true );
	
	
}
void Track::process(int p_frames) {

	process_automations();
	track_pre_process(p_frames);
	base_private.plugin_graph.process( p_frames );
}

AudioNode *Track::get_record_node() {
	
	return &base_private.input_proxy;
}
AudioNode *Track::get_playback_node() {
	
	
	return &base_private.output_proxy;
}


void Track::swap_properties(int p_which,int p_with_which) {
	
	ERR_FAIL_INDEX(p_which,get_property_count());
	ERR_FAIL_INDEX(p_with_which,get_property_count());
	
	AudioControl::mutex_lock();
	
	PropertyRef * aux_p=base_private.property_list[p_which];
	
	base_private.property_list[p_which]=base_private.property_list[p_with_which];
	base_private.property_list[p_with_which]=aux_p;
	
	void rebuild_active_automation_cache();	
	AudioControl::mutex_unlock();

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

bool Track::is_property_builtin(int p_idx) { ///< property is builtin of track (as in, not from a plugin)
	
	ERR_FAIL_INDEX_V(p_idx,get_property_count(),false);
	
	return base_private.property_list[p_idx]->builtin;
	
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
	
//	printf("ADD- BEFORE\n");
//	for (int i=0;i<get_property_count();i++)
//		printf("%i - %s\n",i,get_property(i)->get_caption().ascii().get_data());
	
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
	
	//add existing ones first, so they are added in proper order
	foreach(I,p_plugin->automated_tracks) {
		
		add_property(path,I->automation->get_property(),I->automation,I->pos,false);	
	}
		
	/* now add any property that may not have been included */
	for (int i=0;i<p_plugin->plugin->get_port_count();i++) {
		if (p_plugin->plugin->get_port_type(i)==SoundPlugin::TYPE_READ)
			continue;
		
		Property *prop=&p_plugin->plugin->get_port(i);
		bool included=false;// was included, so it was already added
		
		foreach(I,p_plugin->automated_tracks) {
		
			if (I->automation->get_property()==prop) {
				
				included=true;
				break;
			}
		}
		
		if (included)
			continue;
		add_property(path,prop,NULL,-1,false);	
	}
	

	/* Finally, restore connections */
	
	base_private.plugin_graph.add_node( p_plugin->plugin, &p_plugin->connections );
	
	rebuild_active_automation_cache();
	plugin_added_notify( p_plugin->plugin );
	
	p_plugin->plugin->set_mixing_rate( get_song_playback()->get_mix_rate() );
	
	AudioControl::mutex_unlock();
	
//	printf("ADD - AFTER\n");
//	for (int i=0;i<get_property_count();i++)
//		printf("%i - %s\n",i,get_property(i)->get_caption().ascii().get_data());
	
}

void Track::remove_plugin(int p_pos,PluginInsertData* p_plugin_recovery) {
	
//	printf("REM - BEFORE\n");
//	for (int i=0;i<get_property_count();i++)
//		printf("%i - %s\n",i,get_property(i)->get_caption().ascii().get_data());
	
	ERR_FAIL_INDEX(p_pos,get_plugin_count());
	
	AudioControl::mutex_lock();
	
	SoundPlugin *node=get_plugin( p_pos );
	
	/* Save Connections */
	
	p_plugin_recovery->plugin=node;
	p_plugin_recovery->pos=p_pos;
	base_private.plugin_graph.erase_node( base_private.plugin_graph.get_node_index(node) , &p_plugin_recovery->connections );
	
	
	/* Erase properties and save automations*/
	
	for (int i=(base_private.property_list.size()-1);i>=0;i--) {
		//go backwards, since we need to erase stuff + remember position of erased element
	
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
		
		//push front because we go backwards
		p_plugin_recovery->automated_tracks.push_front(atdata);
		
		delete base_private.property_list[i]; //erase property ref
		base_private.property_list.erase( base_private.property_list.begin() + i ); //and erase from array
		
	}
	
	base_private.sound_plugins.erase( base_private.sound_plugins.begin() + p_pos );
	
	rebuild_active_automation_cache();
		
	AudioControl::mutex_unlock();
	
//	printf("REM - AFTER\n");
//	for (int i=0;i<get_property_count();i++)
//		printf("%i - %s\n",i,get_property(i)->get_caption().ascii().get_data());
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
		
		base_private.property_list[i]->automation->property_idx=i;
		
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
		if (a->get_property()->is_write_only())
			continue; //some properties of some plugins (VST in this case) dont allow writing,
		//so automating is primitive in them
		a->get_property()->set( a->get_initial_value() );
	}
}

void Track::reset_plugins() {
	
	for (int i=0;i<base_private.sound_plugins.size();i++) 
		base_private.sound_plugins[i]->reset();
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


int Track::find_plugin_idx_for_property(Property *p_property) {

	for (int i=0;i<get_plugin_count();i++) {
	
		SoundPlugin *p=get_plugin(i);
		for (int j=0;j<p->get_port_count();j++) {
		
			if (&p->get_port(j)==p_property)
				return i;
		
		}
	
	}

	return -1;
}

int Track::find_recording_property(int p_chan,int p_control) {
	
	for (int i=0;i<base_private.active_automation_cache.size();i++) {
		
		TrackAutomation *ta=base_private.active_automation_cache[i];
		
		int prop_idx=ta->property_idx;
		
		ERR_CONTINUE(prop_idx<0 || prop_idx>=base_private.property_list.size());
		
		//printf("prop %i enabled %i - %i,%i against %i,%i - visible %i\n",i,ta->is_recording_enabled(),ta->get_recording_channel(),ta->get_recording_control(),p_chan,p_control,base_private.property_list[prop_idx]->automation_visible);
		if (!ta->visible)
			continue;
		
		if (ta->is_recording_enabled() && ta->get_recording_channel()==p_chan && ta->get_recording_control()==p_control)
			return prop_idx;
		
	}
	
	return -1;
}


void Track::set_rack_file(String p_file) {
	
	base_private.rack_file=p_file;
}
String Track::get_rack_file() {
	
	return base_private.rack_file;
}

Tick Track::get_swinged_tick(Tick p_base_tick) {
	
	float swing=(base_private.swing_local.get()>0.5)?base_private.swing.get():base_private.global_props->get_swing().get();
	
	if (swing==0)
		return p_base_tick;
	
	swing/=100.0; //% -> ratio
	
	int base=base_private.swing_base.get_current();
	ERR_FAIL_INDEX_V(base,MAX_DIVISORS,p_base_tick);
	base=divisors[base];
	
	return get_swing_pos(p_base_tick,base,swing);
}

Property &Track::swing() {
	
	return base_private.swing;
}
Property &Track::swing_base() {
	
	return base_private.swing_base;
}
Property &Track::swing_local() {
	
	return base_private.swing_local;
}

Property &Track::volume() {
	
	return base_private.volume;
	
}


Track::Track(int p_channels,BlockType p_type,GlobalProperties *p_global_props,SongPlayback *p_song_playback) : BlockList(p_type) {
	
	base_private.global_props=p_global_props;
	
	base_private.audio.highest_energy.resize(p_channels);
	
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
	
	
	
	for (int i=0;i<get_channels();i++)
		base_private.audio.highest_energy[i]=0;
	base_private.audio.volume_ratio=1;
	base_private.audio.mute=0;
	
	base_private.plugin_graph.set_visual_node_order( this );
	
	base_private.swing.set_all( 0, 0, 100, 0, 1, Property::DISPLAY_KNOB, "swing","Swing","%","Disabled");
	base_private.swing_local.set_all( 0, 0, 1, 0, 1, Property::DISPLAY_CHECKBOX, "swing_local","Swing local","","Off","On");
	
	std::vector<String> base_divisors;
	
	for (int i=0;i<MAX_DIVISORS;i++) {
		
		base_divisors.push_back( String::num( divisors[i] ) );
	}
	
	base_private.swing_base.set_all("swing_base","Swing Base",base_divisors,0);
	base_private.volume.set_all( 0, -60, 24, 0, 0.1, Property::DISPLAY_SLIDER, "volume","Volume","dB");
	
	add_property("Track/",&base_private.swing);
	add_property("Track/",&base_private.swing_local);
	add_property("Track/",&base_private.swing_base);
	add_property("Track/",&base_private.volume);
	
}


Track::~Track()
{
	//delete plugins
	for (int i=0;i<base_private.sound_plugins.size();i++)
		delete base_private.sound_plugins[i];
	
	//delete property list
	for (int i=0;i<base_private.property_list.size();i++) {
		
		delete base_private.property_list[i]->automation;
		delete base_private.property_list[i];
	}
	//delete plugs
	delete base_private.input_plug;
	delete base_private.output_plug;
}


}
