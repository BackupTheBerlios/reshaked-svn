//
// C++ Interface: sound_plugin_gui_list
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSOUND_PLUGIN_GUI_LIST_H
#define RESHAKEDSOUND_PLUGIN_GUI_LIST_H

#include <list>
#include "interface/sound_plugin_gui.h"

namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

typedef SoundPluginGUI* (*SoundPluginGUICreateFunc)(SoundPlugin *p_plugin);

class SoundPluginGUIList {


	static SoundPluginGUIList *singleton;
	
	std::list<SoundPluginGUICreateFunc> creation_funcs;
	
public:

	static SoundPluginGUIList *get_singleton();
	
	SoundPluginGUI * create_GUI(SoundPlugin *p_plugin);

	void append_create_func(SoundPluginGUICreateFunc p_func);
	
	SoundPluginGUIList();
	
	~SoundPluginGUIList();

};

}

#endif
