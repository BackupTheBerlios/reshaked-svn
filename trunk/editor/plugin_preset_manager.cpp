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
#include "tree_saver_disk.h"
namespace ReShaked {




PluginPresetManager* PluginPresetManager::singleton=NULL;
	
bool PluginPresetManager::save_plugin_preset(String p_path,SoundPlugin *p_plugin) {
		
	TreeSaverDisk tsd("RESHAKED_PLUGIN_"+p_plugin->get_info()->unique_ID,0);
	
	if (tsd.open_file(p_path))
		return true;
	
	p_plugin->save( &tsd );
	
	tsd.close_file();
	
	return false;
	
}

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
