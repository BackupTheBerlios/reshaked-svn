//
// C++ Implementation: sound_plugin_gui
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sound_plugin_gui.h"

namespace ReShaked {


void SoundPluginGUI::set_in_window() {

	create(plugin);
}


SoundPlugin *SoundPluginGUI::get_plugin() {

	return plugin;
}
void SoundPluginGUI::create(SoundPlugin *p_plugin) {


}

SoundPluginGUI::SoundPluginGUI(SoundPlugin *p_plugin) {

	plugin=p_plugin;
}


SoundPluginGUI::~SoundPluginGUI()
{
}


}
