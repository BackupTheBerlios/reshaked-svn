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
#include "ui_blocks/helpers.h"

namespace ReShaked {

/**
	@author red <red@killy>
*/
class SoundPluginUI : public QWidget {
Q_OBJECT
					    
		
	SoundPlugin *_plugin;		
		
	PropertyEditUpdater * editor_updater;
	std::vector<PropertyEditorBase*> property_editors;
	
	static void property_editor_property_edited_callback(void *_this,Property* p_property,double p_old_val);
protected:			    
			    
	void register_property_editor(PropertyEditorBase* p_property_editor);
	void unregister_property_editor(PropertyEditorBase* p_property_editor);
	
	CHBox* generate_default_layout(bool p_generate_controls=false);
	
signals:	
	
	void property_edited_signal(Property *p_property,double p_old_value);
	void property_options_requested(Property *p_property);
	
public:
	void set_property_editor_updater(PropertyEditUpdater *p_property_edit_updater);
	SoundPluginUI(QWidget *p_parent,SoundPlugin *p_plugin);
	virtual ~SoundPluginUI();

};

}

#endif
