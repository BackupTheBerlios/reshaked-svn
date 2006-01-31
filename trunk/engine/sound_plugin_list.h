//
// C++ Interface: sound_plugin_list
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSOUND_PLUGIN_LIST_H
#define RESHAKEDSOUND_PLUGIN_LIST_H

#include <vector>
#include "engine/sound_plugin.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
class SoundPluginList {

	std::vector<const SoundPluginInfo*> info_list;
	
	static SoundPluginList *singleton;
public:
	
	static SoundPluginList *get_singleton();
	
	int get_plugin_count();
	const SoundPluginInfo* get_plugin_info(int p_idx);
	SoundPlugin *instance_plugin(int p_idx,int p_channels=-1);
	SoundPlugin *instance_plugin(String p_ID,int p_channels=-1);
	
	void add_info(const SoundPluginInfo* p_info);
	
	SoundPluginList();
	~SoundPluginList();

};

}

#endif
