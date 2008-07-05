//
// C++ Implementation: register_node_uis
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "register_node_uis.h"

#include "generic_node_ui.h"
#include "gui_common/node_ui_registry.h"

void register_node_uis() {

	NodeUI_Registry::add_create_ui_func(GenericNodeUI::ui_create_func);
}
