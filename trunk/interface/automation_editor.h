//
// C++ Interface: automation_editor
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDAUTOMATION_EDITOR_H
#define RESHAKEDAUTOMATION_EDITOR_H

#include "editor/editor.h"
#include "base/window.h"
#include "widgets/tree.h"
#include "bundles/scroll_box.h"
#include "bundles/window_box.h"

using namespace GUI;

namespace ReShaked {

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class AutomationEditor : public Window {
	
	Editor *editor;
	Track *track;
	Tree *tree;
	
	void automation_checked(bool p_checked,int p_which);
public:
	
	void show(Track *p_track);
	
	AutomationEditor(Window *p_parent,Editor *p_editor);
	~AutomationEditor();

};

}

#endif
