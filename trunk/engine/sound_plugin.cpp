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

SoundPlugin::SoundPlugin(const SoundPluginInfo *p_info,int p_channels) {
	
	_plugin_info=p_info;
	_channels_created=p_channels;
	_skip_processing=false;
	_duplicate=0;
}


SoundPlugin::~SoundPlugin()
{
}


}
