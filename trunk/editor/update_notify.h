//
// C++ Interface: update_notify
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef UPDATE_NOTIFY_H
#define UPDATE_NOTIFY_H

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class UpdateNotify {

	static UpdateNotify *singleton;
public:

	static UpdateNotify *get_singleton();

	virtual void sound_driver_changed()=0;
	virtual void audio_graph_changed()=0;

	UpdateNotify();
	virtual ~UpdateNotify();

};

#endif
