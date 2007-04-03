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
#include <Qt/qkeysequence.h>
#include "ui_blocks/helpers.h"

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

/******************/




void QWidget_KeybindDescription::update_tooltip() {
	
	QString qtext=QStrify(text);
	
	if (key!=Keyboard_Input::NO_KEY) {
		qtext+="("+(QString)QKeySequence(key)+")";
	}
	
	widget->setToolTip(qtext);
	
}

void QWidget_KeybindDescription::set_description(String p_descr) {
	
	text=p_descr;
	update_tooltip();
}

void QWidget_KeybindDescription::set_bind(int p_key) {
	
	key=p_key;
	update_tooltip();
	
}

QWidget_KeybindDescription::QWidget_KeybindDescription(QWidget *p_widget)
{
	widget=p_widget;
}


QWidget_KeybindDescription::~QWidget_KeybindDescription()
{
}


}
