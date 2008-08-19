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

#include "engine/track.h"

/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class UpdateNotify {

	static UpdateNotify *singleton;
public:

	static UpdateNotify *get_singleton();

	virtual void sound_driver_changed()=0;
	virtual void audio_graph_changed()=0;

	virtual void track_list_changed()=0;
	virtual void track_list_repaint()=0;
	virtual void cursor_track_changed(int p_from,int p_to)=0;
	virtual void track_changed(Track* p_track)=0;
	virtual void cursor_step_changed()=0;
	virtual void window_snap_changed()=0;
	virtual void volume_mask_changed()=0;
	virtual void window_offset_changed()=0;
	virtual void selection_changed()=0;
	virtual void track_block_changed(Track::Block *p_block)=0;
	virtual void song_initial_tempo_changed()=0;
	
	virtual void editor_volume_scale_request()=0;
	virtual void editor_change_bar_len_request()=0;
	virtual void editor_change_marker_request()=0;

	UpdateNotify();
	virtual ~UpdateNotify();

};

#endif
