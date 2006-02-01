//
// C++ Implementation: plugin_preset_manager
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "plugin_preset_manager.h"

namespace ReShaked {



PluginPresetManager* PluginPresetManager::singleton=NULL;
	
	
String PluginPresetManager::get_preset_dir() {
	
	return plugin_dir;
}
	
PluginPresetManager* PluginPresetManager::get_singleton() {
	
	return singleton;
}
	

PluginPresetManager::PluginPresetManager(String p_plugin_dir) {
	
	plugin_dir=p_plugin_dir;
	singleton=this;
}


PluginPresetManager::~PluginPresetManager()
{
}


}
