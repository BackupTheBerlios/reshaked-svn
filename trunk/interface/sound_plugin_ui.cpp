//
// C++ Implementation: sound_plugin_ui
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "sound_plugin_ui.h"

namespace ReShaked {


void SoundPluginUI::set_periodic_updates(PropertyEditor* p_editor) {
	
	for (int i=0;i<property_editors.size();i++) {
		
		ERR_FAIL_COND( property_editors[i]==p_editor );
	}
	
	property_editors.push_back(p_editor);
	if (editor_updater)
		editor_updater->add_editor( p_editor );
	
}

void SoundPluginUI::remove_periodic_updates(PropertyEditor* p_editor) {
	
	
	
	for (int i=0;i<property_editors.size();i++) {
		
		if (property_editors[i]==p_editor ) {
			property_editors.erase(property_editors.begin()+i);
			break;
		}
			
	}
	
	if (editor_updater)
		editor_updater->remove_editor( p_editor );
}

void SoundPluginUI::set_property_editor_updater(PropertyEditUpdater *p_property_edit_updater) {
	
	if (editor_updater) {
		
		for (int i=0;i<property_editors.size();i++) {
		
			editor_updater->remove_editor( property_editors[i] );
		}
	}
	
	editor_updater=p_property_edit_updater;
	
	for (int i=0;i<property_editors.size();i++) {
		
		editor_updater->add_editor( property_editors[i] );
	}
	
}

SoundPluginUI::SoundPluginUI(QWidget *p_parent,SoundPlugin *p_plugin) : QWidget(p_parent) {
	
	editor_updater=NULL;
	
			
}


SoundPluginUI::~SoundPluginUI() {
	
	if (editor_updater) {
		for (int i=0;i<property_editors.size();i++) {
		
			editor_updater->remove_editor( property_editors[i] );
		}
	}
}


}
