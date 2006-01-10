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
#include "engine/property_edit_updater.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
class SoundPluginUI : public QWidget {
			    
	PropertyEditUpdater * editor_updater;
	std::vector<PropertyEditor*> property_editors;
protected:			    
			    
	void set_periodic_updates(PropertyEditor* p_property_editor);
	void remove_periodic_updates(PropertyEditor* p_property_editor);
public:
	void set_property_editor_updater(PropertyEditUpdater *p_property_edit_updater);
	SoundPluginUI(QWidget *p_parent,SoundPlugin *p_plugin);
	virtual ~SoundPluginUI();

};

}

#endif
