//
// C++ Implementation: qaction_keybind
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "qaction_keybind.h"

namespace ReShaked {


void QAction_Keybind::set_bind(int p_key) {
	
	if (p_key==Keyboard_Input::NO_KEY)
		action->setShortcut(0); //disable
	else
		action->setShortcut(p_key);
}

QAction_Keybind::QAction_Keybind(QAction *p_action)
{
	action=p_action;
}


QAction_Keybind::~QAction_Keybind()
{
}


}
