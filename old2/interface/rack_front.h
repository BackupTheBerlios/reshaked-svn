//
// C++ Interface: rack_front
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RACK_FRONT_H
#define RACK_FRONT_H


#include "bundles/scroll_box.h"
#include "containers/box_container.h"
#include "editor/editor.h"
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/

using namespace GUI;

namespace ReShaked {

class RackFront : public ScrollBox {




	struct PluginTop : public HBoxContainer {
	
		enum Operation {
			
			OP_MOVE_UP,
			OP_MOVE_DOWN,
			OP_TOGGLE_SKIP,
			OP_RENAME,
			OP_ERASE,
			OP_HELP
		};
	
		
		
		Signal< Method1< Operation > > operation_signal;
		
		PluginTop();
	};
	
public:

	void rebuild();

	RackFront(Editor *p_editor);
	~RackFront();

};

}
#endif
