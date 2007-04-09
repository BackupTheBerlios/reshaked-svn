//
// C++ Implementation: gui_update_notify
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "gui_update_notify.h"

namespace ReShaked {

	
void GUI_UpdateNotify::cursor_moved() {
	
	cursor_moved_signal.call();
	
}
void GUI_UpdateNotify::edit_window_changed(){
	
	edit_window_changed_signal.call();
}
void GUI_UpdateNotify::edit_window_moved(){
	
	edit_window_moved_signal.call();
}
void GUI_UpdateNotify::cursor_changed_blocklist(){
	
	cursor_changed_blocklist_signal.call();
}
void GUI_UpdateNotify::track_list_changed() {
	
	track_list_changed_signal.call();
}
void GUI_UpdateNotify::editing_octave_changed() {
	
	editing_octave_changed_signal.call();
}
void GUI_UpdateNotify::rack_changed() {
	
	rack_changed_signal.call();
}
void GUI_UpdateNotify::rack_repaint() {
	
	rack_repaint_signal.call();
}

void GUI_UpdateNotify::current_automation_status_changed() {
	
	current_automation_status_changed_signal.call();
}

void GUI_UpdateNotify::update_blocklist_list(const std::list<int>& p_which) {
	
	update_blocklist_list_signal.call(p_which);	
}

void GUI_UpdateNotify::block_layout_changed() {
	
	block_layout_changed_signal.call();
}

void GUI_UpdateNotify::track_names_changed() {
	
	track_names_changed_signal.call();
}

void GUI_UpdateNotify::volume_mask_changed() {
	
	volume_mask_changed_signal.call();
}
void GUI_UpdateNotify::cursor_step_changed() {
	
	cursor_step_changed_signal.call();
}

void GUI_UpdateNotify::notify_action(String p_action) {
	
	notify_action_signal.call(p_action);
}

void GUI_UpdateNotify::editor_marker_edit_request() {
	
	editor_marker_edit_request_signal.call();
}
void GUI_UpdateNotify::editor_volume_scale_request() {
	
	editor_volume_scale_request_signal.call();
}

/* MVC Hacks */
void GUI_UpdateNotify::current_track_add_column() {
	
	current_track_add_column_signal.call();
}
void GUI_UpdateNotify::current_track_remove_column(){
	
	current_track_remove_column_signal.call();
}
void GUI_UpdateNotify::automation_editor_popup(int p_track){
	
	automation_editor_popup_signal.call(p_track);
}
void GUI_UpdateNotify::automation_options(int p_blocklist){
	
	automation_options_signal.call(p_blocklist);
}
	
	
GUI_UpdateNotify::GUI_UpdateNotify() {
	
	
}


GUI_UpdateNotify::~GUI_UpdateNotify()
{
}


}
