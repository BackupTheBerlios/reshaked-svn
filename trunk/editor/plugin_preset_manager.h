//
// C++ Interface: plugin_preset_manager
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDPLUGIN_PRESET_MANAGER_H
#define RESHAKEDPLUGIN_PRESET_MANAGER_H

#include "typedefs.h"
#include "engine/sound_plugin.h"
namespace ReShaked {

/**
	@author red <red@killy>
*/
class PluginPresetManager {
	
	String plugin_dir;
	
	static PluginPresetManager* singleton;
public:
	
	bool save_plugin_preset(String p_path,SoundPlugin *p_plugin);
	
	String get_preset_dir();
	
	static PluginPresetManager* get_singleton();
	
	PluginPresetManager(String p_plugin_dir);
	~PluginPresetManager();
};

}

#endif
