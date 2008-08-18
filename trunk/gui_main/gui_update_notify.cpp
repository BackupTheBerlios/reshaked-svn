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

void GUI_UpdateNotify::track_list_repaint() {

	track_list_repaint_signal.call();
}

void GUI_UpdateNotify::cursor_track_changed(int p_from,int p_to) {

	cursor_track_changed_signal.call(p_from,p_to);
}
void GUI_UpdateNotify::track_changed(Track* p_track) {

	track_changed_signal.call(p_track);
}
void GUI_UpdateNotify::cursor_step_changed() {

	cursor_step_changed_signal.call();
}

void GUI_UpdateNotify::window_snap_changed() {

	window_snap_changed_signal.call();
}

void GUI_UpdateNotify::volume_mask_changed() {

	volume_mask_changed_signal.call();
}

void GUI_UpdateNotify::window_offset_changed() {

	window_offset_changed_signal.call();
}

void GUI_UpdateNotify::selection_changed() {

	selection_changed_signal.call();
}

void GUI_UpdateNotify::track_block_changed(Track::Block *p_block) {

	track_block_changed_signal.call( p_block );
}


void GUI_UpdateNotify::editor_volume_scale_request() {

	editor_volume_scale_request_signal.call();
}

void GUI_UpdateNotify::editor_change_bar_len_request() {

	editor_change_bar_len_request_signal.call();
}

void GUI_UpdateNotify::editor_change_marker_request() {

	editor_change_marker_request_signal.call();
}

GUI_UpdateNotify::GUI_UpdateNotify(){

	singleton=this;
}


GUI_UpdateNotify::~GUI_UpdateNotify()
{
}


