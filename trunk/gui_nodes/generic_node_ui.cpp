//
// C++ Implementation: generic_node_ui
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "generic_node_ui.h"

BaseNodeUI *ui_create_func(AudioNode *p_node) {

	return new GenericNodeUI(p_node);
}

GenericNodeUI::GenericNodeUI(AudioNode *p_node) : BaseNodeUI(p_node)
{
}


GenericNodeUI::~GenericNodeUI()
{
}


