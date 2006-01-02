//
// C++ Implementation: sound_plugin_ui_list
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sound_plugin_ui_list.h"

namespace ReShaked {


SoundPluginUI_List *SoundPluginUI_List::singleton=NULL;


SoundPluginUI_List *SoundPluginUI_List::get_singleton() {
	
	return singleton;	
}

void SoundPluginUI_List::add_creator(SoundPluginUI* (*p_creator)(SoundPlugin *,QWidget *)) {
	
	creator_funcs.push_back(p_creator);
}
SoundPluginUI* SoundPluginUI_List::create_UI(SoundPlugin *p_plugin,QWidget *p_parent) {


	for (int i=0;i<creator_funcs.size();i++) {
		
		SoundPluginUI *ui=creator_funcs[i](p_plugin,p_parent);
		if (ui)
			return ui;
		
	}
	
	return NULL;
}
SoundPluginUI_List::SoundPluginUI_List() {
	
	singleton=this;
}


SoundPluginUI_List::~SoundPluginUI_List()
{
}


}
