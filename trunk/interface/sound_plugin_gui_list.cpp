//
// C++ Implementation: sound_plugin_gui_list
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sound_plugin_gui_list.h"

namespace ReShaked {


SoundPluginGUIList *SoundPluginGUIList::singleton=NULL;


SoundPluginGUIList *SoundPluginGUIList::get_singleton() {

	return singleton;
}

SoundPluginGUI * SoundPluginGUIList::create_GUI(SoundPlugin *p_plugin) {

	SoundPluginGUI *res=NULL;
	
	std::list<SoundPluginGUICreateFunc>::iterator I = creation_funcs.begin();
	
	for (;I!=creation_funcs.end();I++) {
	
		SoundPluginGUICreateFunc f=*I;
		res=f(p_plugin);
	
		if (res)
			break;
	}
	
	return res;
}


SoundPluginGUIList::SoundPluginGUIList()
{
	singleton=this;
}

void SoundPluginGUIList::append_create_func(SoundPluginGUICreateFunc p_func) {

	creation_funcs.push_front(p_func);
}



SoundPluginGUIList::~SoundPluginGUIList()
{
}


}
