//
// C++ Interface: sound_plugin_ui_generic
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSOUND_PLUGIN_UI_GENERIC_H
#define RESHAKEDSOUND_PLUGIN_UI_GENERIC_H

#include "interface/sound_plugin_ui.h"

namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class SoundPluginUI_Generic : public SoundPluginUI {
public:
	
	static SoundPluginUI* create_this(SoundPlugin *p_plugin,QWidget *p_parent);
	
	SoundPluginUI_Generic(QWidget *p_parent,SoundPlugin *p_plugin);
	~SoundPluginUI_Generic();

};

}

#endif
