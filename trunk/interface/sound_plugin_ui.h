//
// C++ Interface: sound_plugin_ui
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDSOUND_PLUGIN_UI_H
#define RESHAKEDSOUND_PLUGIN_UI_H

#include <Qt/qwidget.h>
#include "engine/sound_plugin.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
class SoundPluginUI : public QWidget {
public:
	
	SoundPluginUI(QWidget *p_parent,SoundPlugin *p_plugin);
	virtual ~SoundPluginUI();

};

}

#endif
