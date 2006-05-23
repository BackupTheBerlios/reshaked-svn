//
// C++ Interface: panner_plugin_ui
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDPANNER_PLUGIN_UI_H
#define RESHAKEDPANNER_PLUGIN_UI_H

#include "interface/sound_plugin_ui.h"

namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
 */
class PannerPluginUI : public SoundPluginUI {
	
public:


	static SoundPluginUI* create_this(SoundPlugin *p_plugin,QWidget *p_parent);

	PannerPluginUI(QWidget *p_parent,SoundPlugin *p_plugin);
	~PannerPluginUI();

};

}

#endif
