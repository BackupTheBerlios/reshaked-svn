//
// C++ Interface: sound_plugin_gui
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSOUND_PLUGIN_GUI_H
#define RESHAKEDSOUND_PLUGIN_GUI_H


#include "containers/box_container.h"
#include "engine/sound_plugin.h"

using namespace GUI;

namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class SoundPluginGUI : public VBoxContainer {


	SoundPlugin *plugin;
	virtual void set_in_window();

protected:

	SoundPlugin *get_plugin();
	virtual void create(SoundPlugin *p_plugin);
	
public:

	SoundPluginGUI(SoundPlugin *p_plugin);
	~SoundPluginGUI();

};

}

#endif
