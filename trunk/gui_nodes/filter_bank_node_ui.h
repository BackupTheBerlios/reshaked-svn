//
// C++ Interface: filterbanknodeui
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FILTERBANKNODEUI_H
#define FILTERBANKNODEUI_H

#include "gui_common/base_node_ui.h"
#include "gui_common/filter_bank_fr.h"
#include "nodes/filter_bank_node.h"
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class FilterBankNodeUI : public BaseNodeUI {

	FilterBankNode *node;
	FilterBankFR *fr;
	void set_in_window();
	void check_ranges_user();
public:

	static BaseNodeUI *ui_create_func(AudioNode *p_node);
	

	FilterBankNodeUI(FilterBankNode *p_filter_bank);	
	~FilterBankNodeUI();

};

#endif
