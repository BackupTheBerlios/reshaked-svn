//
// C++ Interface: delay_line_ui
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDDELAY_LINE_UI_H
#define RESHAKEDDELAY_LINE_UI_H

#include "interface/sound_plugin_ui.h"
#include "plugins/delay_line_plugin.h"
namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
 */
class DelayLinePluginUI : public SoundPluginUI {
	
	DelayLinePlugin *plugin;
	void create_label_and_button(QWidget *p_parent,String p_prop,QPixmap p_label, bool p_first=false);
	void create_knob_and_display(QWidget *p_parent,String p_prop, bool p_above=false);
	
	void create_pan(QWidget *p_parent,String p_basename);
public:


	static SoundPluginUI* create_this(SoundPlugin *p_plugin,QWidget *p_parent);

	DelayLinePluginUI(QWidget *p_parent,DelayLinePlugin *p_plugin);
	~DelayLinePluginUI();

};

}

#endif
