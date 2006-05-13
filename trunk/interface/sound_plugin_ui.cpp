
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



void SoundPluginUI::property_editor_property_edited_callback(void *_this,Property* p_property,double p_old_val) {
	

	SoundPluginUI *instance=(SoundPluginUI*)_this;
	instance->property_edited_signal(p_property, p_old_val); 
	
}

void SoundPluginUI::register_property_editor(PropertyEditorBase* p_editor) {
	
	for (int i=0;i<property_editors.size();i++) {
		
		ERR_FAIL_COND( property_editors[i]==p_editor );
	}
	
	property_editors.push_back(p_editor);
	if (editor_updater)
		editor_updater->add_editor( p_editor );
	
	p_editor->set_changed_by_editor_callback(this,&SoundPluginUI::property_editor_property_edited_callback); 
	
	QWidget *w = dynamic_cast<QWidget*>(p_editor);
	
	if (w) {
		
		QObject::connect(w,SIGNAL(external_edit_signal(Property *)),this,SIGNAL(property_options_requested( Property* )));
	}
	
}

void SoundPluginUI::unregister_property_editor(PropertyEditorBase* p_editor) {
	
	
	
	for (int i=0;i<property_editors.size();i++) {
		
		if (property_editors[i]==p_editor ) {
			property_editors.erase(property_editors.begin()+i);
			break;
		}
			
	}
	
	if (editor_updater)
		editor_updater->remove_editor( p_editor );
	
	p_editor->set_changed_by_editor_callback(NULL,NULL);
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
	
	for (int i=0;i<property_editors.size();i++) {
		//make disable callback to this
		property_editors[i]->set_changed_by_editor_callback(NULL,NULL);
			
	}
	
	
	
	if (editor_updater) {
		for (int i=0;i<property_editors.size();i++) {
		
			editor_updater->remove_editor( property_editors[i] );
		}
	}
}


}
