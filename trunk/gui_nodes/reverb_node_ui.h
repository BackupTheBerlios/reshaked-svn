//
// C++ Interface: reverb_node_ui
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef REVERB_NODE_UI_H
#define REVERB_NODE_UI_H

#include "gui_common/base_node_ui.h"

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
#if 0
class ReverbNodeUI : public BaseNodeUI {

	ReverbNode *node;
	void set_in_window();
	
public:

	static BaseNodeUI *ui_create_func(AudioNode *p_node);
	

	ReverbNodeUI(AudioNode *p_node);	
	~ReverbNodeUI();

};

#endif
#endif
