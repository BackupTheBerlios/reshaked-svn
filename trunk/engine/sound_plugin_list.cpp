//
// C++ Implementation: sound_plugin_list
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sound_plugin_list.h"

namespace ReShaked {


SoundPluginList *SoundPluginList::singleton=NULL;
SoundPluginList *SoundPluginList::get_singleton() {
	
	
	return singleton;
}


int SoundPluginList::get_plugin_count() {
	
	
	return info_list.size();
}

const SoundPluginInfo* SoundPluginList::get_plugin_info(int p_idx) {
	
	ERR_FAIL_INDEX_V(p_idx,info_list.size(),NULL);	
	
}
SoundPlugin *SoundPluginList::instance_plugin(int p_idx,int p_channels) {
	
	ERR_FAIL_INDEX_V(p_idx,info_list.size(),NULL);	
	ERR_FAIL_COND_V( info_list[p_idx]->can_custom_channels && p_channels<=0 , NULL);
	ERR_FAIL_COND_V( !info_list[p_idx]->can_custom_channels && p_channels>=0 , NULL);
	return info_list[p_idx]->creation_func( info_list[p_idx], p_channels );
}

void SoundPluginList::add_info(const SoundPluginInfo* p_info) {
	
	info_list.push_back(p_info);	
	
}

SoundPluginList::SoundPluginList() {
	
	ERR_FAIL_COND(singleton!=NULL);
	singleton=this;
}


SoundPluginList::~SoundPluginList()
{
}


}
