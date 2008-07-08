//
// C++ Interface: preset_browser
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PRESET_BROWSER_H
#define PRESET_BROWSER_H


#include "base/window.h"
#include "engine/audio_node.h"
#include "widgets/tree.h"
#include "bundles/window_box.h"

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class PresetBrowser : public GUI::Window {

	GUI::WindowBox *window_box;
	GUI::Tree *tree;

public:

	void edit_preset( AudioNode *p_node );

	PresetBrowser(GUI::Window *p_parent);
	~PresetBrowser();

};

#endif
