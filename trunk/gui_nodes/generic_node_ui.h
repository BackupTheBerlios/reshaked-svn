//
// C++ Interface: generic_node_ui
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GENERIC_NODE_UI_H
#define GENERIC_NODE_UI_H

#include "gui_common/base_node_ui.h"

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class GenericNodeUI : public BaseNodeUI {

	AudioNode *node;
	void set_in_window();
public:

	static BaseNodeUI *ui_create_func(AudioNode *p_node);

	GenericNodeUI(AudioNode *p_node);	
	~GenericNodeUI();

};

#endif
