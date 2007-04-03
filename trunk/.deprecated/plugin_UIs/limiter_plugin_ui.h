//
// C++ Interface: limiter_plugin_ui
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDLIMITER_PLUGIN_UI_H
#define RESHAKEDLIMITER_PLUGIN_UI_H

#include "interface/sound_plugin_ui.h"
#include "plugins/limiter_plugin.h"
#include "property_editors.h" //everything is in here anyway

namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
 */
class LimiterPluginUI : public SoundPluginUI {
	
	Q_OBJECT
	
	LimiterPlugin *limiter;
	PixmapVU * vu;
	PixmapLabel * label;
	PixmapLabel * peak;
	bool last_peak;
private slots:	
	
	void update_vu_and_peak();
	void clear_peak();
	
public:


	static SoundPluginUI* create_this(SoundPlugin *p_plugin,QWidget *p_parent);

	LimiterPluginUI(QWidget *p_parent,LimiterPlugin *p_limiter);
	~LimiterPluginUI();

};

}

#endif
