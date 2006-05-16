//
// C++ Interface: filter_bank_plugin_ui
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDFILTER_BANK_PLUGIN_UI_H
#define RESHAKEDFILTER_BANK_PLUGIN_UI_H

#include "interface/sound_plugin_ui.h"
#include "plugins/filter_bank_plugin.h"

namespace ReShaked {


/**
	@author red <red@killy>
 */
class FilterBankPluginUI : public SoundPluginUI {
	
public:
	
	static SoundPluginUI* create_this(SoundPlugin *p_plugin,QWidget *p_parent);
	FilterBankPluginUI(QWidget *p_parent,FilterBankPlugin *p_filterbank);
	~FilterBankPluginUI();

};

}

#endif
