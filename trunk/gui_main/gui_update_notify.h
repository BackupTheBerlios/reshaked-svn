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
	virtual void track_list_changed();
	virtual void track_list_repaint();	
	virtual void cursor_track_changed(int p_from,int p_to);	
	virtual void track_changed(Track* p_track);	
	virtual void cursor_step_changed();
	virtual void window_snap_changed();
	virtual void volume_mask_changed();
	virtual void window_offset_changed();
	virtual void selection_changed();
	virtual void track_block_changed(Track::Block *p_block);
	// requestors
	virtual void editor_volume_scale_request();	
	virtual void editor_change_bar_len_request();	
	virtual void editor_change_marker_request();	
	
	GUI::Signal<> sound_driver_changed_signal;
	GUI::Signal<> audio_graph_changed_signal;
	GUI::Signal<> track_list_changed_signal;
	GUI::Signal<> track_list_repaint_signal;
		
	GUI::Signal< GUI::Method2<int,int> > cursor_track_changed_signal;
	GUI::Signal< GUI::Method1<Track*> > track_changed_signal;
	GUI::Signal<> cursor_step_changed_signal;
	GUI::Signal<> window_snap_changed_signal;
	GUI::Signal<> volume_mask_changed_signal;
	GUI::Signal<> window_offset_changed_signal;
	GUI::Signal<> selection_changed_signal;
	GUI::Signal< GUI::Method1<Track::Block*> > track_block_changed_signal;

	GUI::Signal<> editor_volume_scale_request_signal;
	GUI::Signal<> editor_change_bar_len_request_signal;
	GUI::Signal<> editor_change_marker_request_signal;


	GUI_UpdateNotify();
	~GUI_UpdateNotify();

};


#endif
