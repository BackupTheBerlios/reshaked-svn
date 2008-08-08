//
// C++ Implementation: gui_update_notify
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "gui_update_notify.h"

GUI_UpdateNotify* GUI_UpdateNotify::singleton=NULL;

GUI_UpdateNotify *GUI_UpdateNotify::get_singleton() {

	return singleton;
}

void GUI_UpdateNotify::sound_driver_changed() {

	sound_driver_changed_signal.call();
}

void GUI_UpdateNotify::audio_graph_changed() {

	audio_graph_changed_signal.call();
}

void GUI_UpdateNotify::track_list_changed() {

	track_list_changed_signal.call();
}


GUI_UpdateNotify::GUI_UpdateNotify(){

	singleton=this;
}


GUI_UpdateNotify::~GUI_UpdateNotify()
{
}


