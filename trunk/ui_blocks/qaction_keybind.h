//
// C++ Interface: qaction_keybind
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDQACTION_KEYBIND_H
#define RESHAKEDQACTION_KEYBIND_H

#include "editor/keyboard_input.h"
#include <Qt/qaction.h>
namespace ReShaked {

/**
	@author red <red@killy>
*/
class QAction_Keybind : public Keyboard_Input::External {
			    
	QAction *action;
public:
	void set_bind(int p_key);
	QAction_Keybind(QAction *p_action);
	
	~QAction_Keybind();

};

}

#endif
