//
// C++ Interface: property_edit_updater
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2006
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDPROPERTY_EDIT_UPDATER_H
#define RESHAKEDPROPERTY_EDIT_UPDATER_H


#include "property.h"

namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class PropertyEditUpdater{
				       
	std::vector<PropertyEditorBase*> editor_list;
public:
	
	void add_editor(PropertyEditorBase* p_editor);
	void remove_editor(PropertyEditorBase* p_editor);
	void clear();
	
	void update_editors();
	
	PropertyEditUpdater();
	~PropertyEditUpdater();

};

}

#endif
