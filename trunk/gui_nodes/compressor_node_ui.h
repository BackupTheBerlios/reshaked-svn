//
// C++ Interface: compressor_node_ui
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef COMPRESSOR_NODE_UI_H
#define COMPRESSOR_NODE_UI_H

#include "gui_common/base_node_ui.h"
#include "nodes/compressor_node.h"
#include "gui_common/compressor_curve_display.h"
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class CompressorNodeUI : public BaseNodeUI {

	CompressorNode *node;
	CompressorCurveDisplay *display_curve;
	void set_in_window();
	void check_ranges_user();
public:

	static BaseNodeUI *ui_create_func(AudioNode *p_node);
	

	CompressorNodeUI(CompressorNode *p_compressor);	
	~CompressorNodeUI();

};

#endif
