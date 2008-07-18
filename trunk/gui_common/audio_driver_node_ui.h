//
// C++ Interface: audio_driver_node_ui
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef AUDIO_DRIVER_NODE_UI_H
#define AUDIO_DRIVER_NODE_UI_H

#include "gui_common/base_node_ui.h"
#include "bundles/combo_box.h"
#include "widgets/button_group.h"
#include "engine/audio_driver_node.h"

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class AudioDriverNodeUI : public BaseNodeUI {

	AudioDriverNode *driver_node;

	GUI::ButtonGroup bgroup;
	GUI::ComboBox **combo_named;
	GUI::ComboBox **combo_fixed;
	
	
	void combo_named_selected(int idx, int combo_idx);
	void combo_fixed_selected(int idx, int combo_idx);
	
	bool scanning;
	void rescan();
public:

	static BaseNodeUI *ui_create_func(AudioNode *p_node);

	AudioDriverNodeUI(AudioDriverNode* p_node);	
	~AudioDriverNodeUI();

};

#endif
