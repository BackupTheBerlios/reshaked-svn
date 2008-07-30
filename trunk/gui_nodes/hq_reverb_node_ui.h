//
// C++ Interface: hq_reverb_node_ui
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef HQ_REVERB_NODE_UI_H
#define HQ_REVERB_NODE_UI_H

#include "gui_common/base_node_ui.h"
#include "gui_common/reverb_display_ir.h"
#include "nodes/hq_reverb_node.h"
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class HQ_ReverbNodeUI : public BaseNodeUI {

	HQ_ReverbNode *node;
	ReverbDisplayIR *display_ir;
	void set_in_window();
	void check_ranges_user();
public:

	static BaseNodeUI *ui_create_func(AudioNode *p_node);
	

	HQ_ReverbNodeUI(HQ_ReverbNode *p_hq_reverb);	
	~HQ_ReverbNodeUI();

};

#endif
