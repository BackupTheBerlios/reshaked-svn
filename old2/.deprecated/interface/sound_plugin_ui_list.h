//
// C++ Interface: sound_plugin_ui_list
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSOUND_PLUGIN_UI_LIST_H
#define RESHAKEDSOUND_PLUGIN_UI_LIST_H



#include "interface/sound_plugin_ui.h"
#include <vector>

namespace ReShaked {

/**
	@author red <red@killy>
*/

class SoundPluginUI_List {

			
	static SoundPluginUI_List *singleton;
	std::vector<SoundPluginUI* (*)(SoundPlugin *,QWidget *)> creator_funcs;			    
public:
	
	static SoundPluginUI_List *get_singleton();
	
	void add_creator(SoundPluginUI* (*p_creator)(SoundPlugin *,QWidget *));
	SoundPluginUI* create_UI(SoundPlugin *p_plugin,QWidget *p_parent);
	
	SoundPluginUI_List();
	~SoundPluginUI_List();

};

}

#endif
