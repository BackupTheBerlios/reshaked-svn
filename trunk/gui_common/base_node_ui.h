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



/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class BaseNodeUI : public GUI::VBoxContainer {

	AudioNode *_node;	
	
	void presets_callback();
public:

	GUI::Signal< GUI::Method1< AudioNode* > > edited_signal;

	BaseNodeUI(AudioNode *p_node);	
	~BaseNodeUI();

};

#endif
