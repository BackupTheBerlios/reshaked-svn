//
// C++ Interface: base_nod_eui
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BASE_NODE_UI_H
#define BASE_NODE_UI_H


#include "containers/box_container.h"
#include "widgets/line_edit.h"
#include "widgets/button.h"
#include "engine/audio_node.h"
#include "gui_common/control_port_range.h"
#include <list>

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class BaseNodeUI : public GUI::VBoxContainer {

	std::list< ControlPortRange * > updatable_ranges;

	AudioNode *_node;	
	void presets_callback();
		
protected:
	
	void register_range_for_updates(ControlPortRange *p_range);
public:

	GUI::Signal< GUI::Method1< AudioNode* > > edited_signal;

	void check_ranges();

	BaseNodeUI(AudioNode *p_node);	
	~BaseNodeUI();

};

#endif
