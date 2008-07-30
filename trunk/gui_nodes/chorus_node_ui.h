//
// C++ Interface: chorus_node_ui
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef CHORUS_NODE_UI_H
#define CHORUS_NODE_UI_H

#include "gui_common/base_node_ui.h"
#include "nodes/chorus_node.h"
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class ChorusNodeUI : public BaseNodeUI {

	struct ChorusUIData : public AudioNodeUIData {
	
		int current_page;
		
		ChorusUIData() { current_page=0; }
	};

	void tab_changed_slot(int p_to);
	ChorusUIData *chorus_ui_data;
	ChorusNode *node;
	void set_in_window();

public:

	static BaseNodeUI *ui_create_func(AudioNode *p_node);
	

	ChorusNodeUI(ChorusNode *p_hq_reverb);	
	~ChorusNodeUI();

};


#endif
