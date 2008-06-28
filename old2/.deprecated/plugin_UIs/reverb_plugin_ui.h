//
// C++ Interface: reverb_plugin_ui
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDREVERB_PLUGIN_UI_H
#define RESHAKEDREVERB_PLUGIN_UI_H

#include "interface/sound_plugin_ui.h"
#include "plugins/reverb_plugin.h"

namespace ReShaked {


/**
	@author red <red@killy>
*/
class ReverbPluginUI : public SoundPluginUI {
	
public:
		
	static SoundPluginUI* create_this(SoundPlugin *p_plugin,QWidget *p_parent);
	ReverbPluginUI(QWidget *p_parent,ReverbPlugin *p_chorus);
	~ReverbPluginUI();

};

}

#endif
