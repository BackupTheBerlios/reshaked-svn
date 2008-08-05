//
// C++ Interface: gui_update_notify
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef GUI_UPDATE_NOTIFY_H
#define GUI_UPDATE_NOTIFY_H

#include "editor/update_notify.h"
#include "signals/signals.h"
#include <stddef.h>
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class GUI_UpdateNotify : public UpdateNotify {

	static GUI_UpdateNotify *singleton;
public:

	static GUI_UpdateNotify *get_singleton();

	virtual void sound_driver_changed();
	virtual void audio_graph_changed();
	
	GUI::Signal<> sound_driver_changed_signal;
	GUI::Signal<> audio_graph_changed_signal;

	GUI_UpdateNotify();
	~GUI_UpdateNotify();

};


#endif
