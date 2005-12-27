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


String SoundPlugin::get_caption() {
	
	return _plugin_info->caption;
	
}

const SoundPluginInfo* SoundPlugin::get_info() {
	
	
	return _plugin_info;
}

int SoundPlugin::get_channels_created() {
	
	return _channels_created;
	
}


void SoundPlugin::set_event_buffer(const EventBuffer *p_buffer) {
	
	
}

/* Internal UI Methods */
void SoundPlugin::show_internal_UI() {
	
	
}
void SoundPlugin::hide_internal_UI() {
	
	
}

SoundPlugin::SoundPlugin(const SoundPluginInfo *p_info,int p_channels) {
	
	_plugin_info=p_info;
	_channels_created=p_channels;
}


SoundPlugin::~SoundPlugin()
{
}


}
