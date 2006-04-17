//
// C++ Implementation: sound_plugin
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sound_plugin.h"

namespace ReShaked {


const EventBuffer *SoundPlugin::get_event_buffer() {
	
	return _event_buffer;
}

String SoundPlugin::get_caption() {
	
	if (_duplicate==0)
		return _plugin_info->caption;
	else
		return _plugin_info->caption+"("+String::num( _duplicate+1 )+")";
	
}

int SoundPlugin::get_duplicate() {
	
	return _duplicate;
}

void SoundPlugin::set_duplicate(int p_duplicate) {
	
	if (p_duplicate<0)
		return;
	_duplicate=p_duplicate;
}
const SoundPluginInfo* SoundPlugin::get_info() {
	
	
	return _plugin_info;
}

Property& SoundPlugin::get_port_by_name(String p_name) {
	
	Property *retprop=NULL;	
	
	for(int i=0;i<get_port_count();i++) {
		
		if (get_port(i).get_name()==p_name)
			retprop=&get_port(i);
	}

	return *retprop;
}

int SoundPlugin::get_channels_created() {
	
	return _channels_created;
	
}

void SoundPlugin::reset() {
	
	
}

void SoundPlugin::set_event_buffer(const EventBuffer *p_buffer) {
	
	_event_buffer=p_buffer;	
}

void SoundPlugin::set_skip_processing(bool p_skip) {
	
	_skip_processing=p_skip;
}
bool SoundPlugin::skips_processing() {
	
	return _skip_processing;	
}


/* Internal UI Methods */
void SoundPlugin::show_internal_UI() {
	
	
}
void SoundPlugin::hide_internal_UI() {
	
	
}


void SoundPlugin::save(TreeSaver *p_saver) {
	
	/* Default code for saving a sound plugin. As long as EVERY parameter can be automatable, this will work */
	for (int i=0;i<get_port_count();i++) {
		
		if (get_port_type(i)==TYPE_READ)
			continue;
		p_saver->add_float( get_port(i).get_name(), get_port(i).get() );
		
	}
	
}
void SoundPlugin::load(TreeLoader *p_loader) {
	

	
	for (int i=0;i<p_loader->get_var_count();i++) {
		
		String var_name=p_loader->get_var_name(i);
		

		for (int j=0;j<get_port_count();j++) {
		

			if (get_port(j).get_name()==var_name) {
				get_port(j).set( p_loader->get_float( var_name ) );
				break;
			}
		}	
		
	}
	
}

String SoundPlugin::get_current_file() {
	
	return _current_file;
	
}
void SoundPlugin::set_current_file(String p_current_file) {
	
	_current_file=p_current_file;	
}

void SoundPlugin::set_metadata(SoundPlugin_MetaData *p_metadata) {
	
	_metadata=p_metadata;
}
SoundPlugin_MetaData *SoundPlugin::get_metadata() {
	
	return _metadata;
}

SoundPlugin::SoundPlugin(const SoundPluginInfo *p_info,int p_channels) {
	
	_plugin_info=p_info;
	_channels_created=p_channels;
	_skip_processing=false;
	_duplicate=0;
	_metadata=NULL;
}


SoundPlugin::~SoundPlugin() {
	
	if (_metadata)
		delete _metadata;
}


}
