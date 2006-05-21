//
// C++ Interface: moog_filter_plugin_ui
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDMOOG_FILTER_PLUGIN_UI_H
#define RESHAKEDMOOG_FILTER_PLUGIN_UI_H


#include "interface/sound_plugin_ui.h"
#include "plugins/moog_filter_plugin.h"
namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
 */
class MoogFilterPluginUI : public SoundPluginUI {
	
public:


	static SoundPluginUI* create_this(SoundPlugin *p_plugin,QWidget *p_parent);

	MoogFilterPluginUI(QWidget *p_parent,MoogFilterPlugin *p_plugin);
	~MoogFilterPluginUI();

};

}

#endif
