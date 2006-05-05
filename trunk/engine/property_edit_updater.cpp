//
// C++ Implementation: property_edit_updater
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "property_edit_updater.h"

namespace ReShaked {

void PropertyEditUpdater::add_editor(PropertyEditorBase* p_editor) {
	
	for (int i=0;i<editor_list.size();i++) {
		
		ERR_FAIL_COND( editor_list[i]==p_editor );
	}
	
	editor_list.push_back(p_editor);
}
void PropertyEditUpdater::remove_editor(PropertyEditorBase* p_editor) {
	
	for (int i=0;i<editor_list.size();i++) {
		
		if(editor_list[i]==p_editor) {
			
			editor_list.erase( editor_list.begin() + i);
			return;
		}
	}
	
}
void PropertyEditUpdater::clear() {
	
	editor_list.clear();
}

void PropertyEditUpdater::update_editors() {
	
	for (int i=0;i<editor_list.size();i++) {
	
		editor_list[i]->check_if_changed();
	}
}


PropertyEditUpdater::PropertyEditUpdater()
{
}


PropertyEditUpdater::~PropertyEditUpdater()
{
}


}
