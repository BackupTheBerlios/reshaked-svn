//
// C++ Implementation: track_editor_top
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "track_editor_top.h"

#include "gui_common/common_skin.h"
#include "widgets/separator.h"
#include "typedefs.h"
#include "editor/editor.h"
#include "editor/key_bindings.h"

void TrackEditorTop::editing_vars_changed() {


	updating=true;

	cursor_step->get_range()->set( Editor::get_singleton()->get_cursor_step() );
	snap_combo->select( Editor::get_singleton()->get_cursor_zoom_divisor() );
	volume_mask_toggle->set_pressed( Editor::get_singleton()->is_volume_mask_active() );
	volume_mask->get_range()->set( Editor::get_singleton()->get_volume_mask() );

	updating=false;
}
void TrackEditorTop::volume_mask_toggled(bool p_to) {

	if (!updating)
		return;
	
	Editor::get_singleton()->set_volume_mask_active(p_to);
}
void TrackEditorTop::volume_mask_spin_changed(double p_to_val) {

	if (!updating)
		return;
	
	Editor::get_singleton()->set_volume_mask(p_to_val);
}

void TrackEditorTop::step_spin_changed(double p_to_val) {

	if (updating)
		return;
	Editor::get_singleton()->set_cursor_step((int)p_to_val);
}
void TrackEditorTop::combo_snap_changed(int p_to_val) {

	if (updating)
		return;
	Editor::get_singleton()->set_cursor_zoom_divisor( p_to_val );

}

void TrackEditorTop::volume_scale_dialog_callback(double p_val) {

	Editor::get_singleton()->selection_command( Editor::SELECTION_SCALE_VOLUMES, (int)p_val);
	volume_scale_last=p_val;
}

void TrackEditorTop::volume_scale_dialog_show() {

	scale_input_dialog->show("Volume Scale %",0,400,1,volume_scale_last);
}

void TrackEditorTop::barbeat_dialog_callback(double p_val) {

	Editor::get_singleton()->set_bar_len_at_beat( Editor::get_singleton()->get_cursor_tick() / TICKS_PER_BEAT, (int)p_val );
	barbeat_last=p_val;
}
void TrackEditorTop::barbeat_dialog_show() {

	barbeat_input_dialog->show("Set Bar Length:\n(1-16, 0 erases)",0,16,1,barbeat_last);
}

void TrackEditorTop::marker_dialog_callback(String p_text) {

	Editor::get_singleton()->set_marker_at_beat( Editor::get_singleton()->get_cursor_tick() / TICKS_PER_BEAT, p_text );
}

void TrackEditorTop::marker_dialog_show() {

	int beat = Editor::get_singleton()->get_cursor_tick() / TICKS_PER_BEAT;
	
	int marker_idx_exact = Editor::get_singleton()->get_song()->get_marker_map().find_exact( beat );
	
	String marker="New Marker (Beat " + String::num(beat)+")";
	if (marker_idx_exact!=-1) {
	
		marker = Editor::get_singleton()->get_song()->get_marker_map()[marker_idx_exact];
	}

	marker_input_dialog->show("Set Marker Name:              \n(empty erases)",marker);
}


void TrackEditorTop::menu_callback(int p_option) {

	switch( p_option ) {
		case PLAY_FROM_CURSOR: {
		
		} break;
		case PLAY_BLOCK_LOOPED: {
		
		} break;
		case SELECTION_BEGIN: {
		
			Editor::get_singleton()->set_selection_begin_at_cursor();
		} break;
		case SELECTION_END: {
		
			Editor::get_singleton()->set_selection_end_at_cursor();
		} break;
		case SELECTION_BLOCK_COLUMN: {
		
			Editor::get_singleton()->select_column_block_all();
		} break;
		case SELECTION_CLEAR: {
		
			Editor::get_singleton()->set_selection_disabled();
		} break;
		case SELECTION_CREATE_BLOCKS: {
		
		} break;
		case SELECTION_MAKE_LOOP: {
		
		} break;
		case SELECTION_SCALE_VOLUMES: {
		
			volume_scale_dialog_show();
		} break;
		case SELECTION_APPLY_VOLUME_MASK: {
		
			Editor::get_singleton()->selection_command( Editor::SELECTION_APPLY_VOLUME_MASK );
		} break;
		case SELECTION_QUANTIZE_UP: {
			
			Editor::get_singleton()->selection_command( Editor::SELECTION_QUANTIZE_UP );
		} break;
		case SELECTION_QUANTIZE_NEAREST: {
		
			Editor::get_singleton()->selection_command( Editor::SELECTION_QUANTIZE_NEAREST );
		} break;
		case SELECTION_QUANTIZE_DOWN: {
		
			Editor::get_singleton()->selection_command( Editor::SELECTION_QUANTIZE_DOWN );		
		} break;
		case EDIT_COPY: {
		
			Editor::get_singleton()->clipboard_copy();				
		} break;
		case EDIT_CUT: {
		
			Editor::get_singleton()->clipboard_cut();
		} break;
		case EDIT_PASTE: {
		
			Editor::get_singleton()->clipboard_paste( Editor::PASTE_OVERWRITE );
		} break;
		case EDIT_PASTE_INSERT: {
		
			Editor::get_singleton()->clipboard_paste( Editor::PASTE_INSERT );
		} break;
		case EDIT_PASTE_MIX: {
		
			Editor::get_singleton()->clipboard_paste( Editor::PASTE_MIX );
		} break;
		case EDIT_MARKER: {
					
			marker_dialog_show();
		} break;
		case EDIT_BAR_LENTH: {
			
			barbeat_dialog_show();
		} break;
		case EDIT_SET_LOOP_BEGIN: {
		
		} break;
		case EDIT_SET_LOOP_END: {
		
		} break;
		case EDIT_TOGGLE_REPEAT: {
		
		} break;
		case EDIT_DELETE_BLOCK: {
		
		} break;
		case EDIT_TRANSPOSE_UP_SEMITONE: {
		
			Editor::get_singleton()->selection_command( Editor::SELECTION_TRANSPOSE_UP );		
		} break;
		case EDIT_TRANSPOSE_DOWN_SEMITONE: {
		
			Editor::get_singleton()->selection_command( Editor::SELECTION_TRANSPOSE_DOWN );		
		} break;
		case EDIT_TRANSPOSE_UP_OCTAVE: {
		
			Editor::get_singleton()->selection_command( Editor::SELECTION_TRANSPOSE_UP_OCTAVE );		
		} break;
		case EDIT_TRANSPOSE_DOWN_OCTAVE: {
		
			Editor::get_singleton()->selection_command( Editor::SELECTION_TRANSPOSE_DOWN_OCTAVE );		
		} break;
	}
}

void TrackEditorTop::edit_menu_about_to_show() {

	edit_menu->clear();
	edit_menu->add_item("Copy",EDIT_COPY,KEYBIND("editor/selection_copy"),false);
	edit_menu->add_item("Cut",EDIT_CUT,KEYBIND("editor/selection_zap"),false);
	edit_menu->add_item("Paste",EDIT_PASTE,KEYBIND("editor/selection_paste_overwrite"),false);
	edit_menu->add_item("Paste Insert",EDIT_PASTE_INSERT,KEYBIND("editor/selection_paste_insert"),false);
	edit_menu->add_item("Paste Mix",EDIT_PASTE_MIX,KEYBIND("editor/selection_paste_mix"),false);
	edit_menu->add_separator();
	edit_menu->add_item("Change Marker",EDIT_MARKER,KEYBIND("editor/edit_marker"),false);
	edit_menu->add_item("Change Bar Length",EDIT_BAR_LENTH,KEYBIND("editor/bar_length_at_pos"),false);
	edit_menu->add_separator();
	edit_menu->add_item("Set Loop Begin",EDIT_SET_LOOP_BEGIN,KEYBIND("editor/set_loop_begin"),false);
	edit_menu->add_item("Set Loop End",EDIT_SET_LOOP_END,KEYBIND("editor/set_loop_end"),false);
	edit_menu->add_separator();
	edit_menu->add_item("Toggle Block Repeat",EDIT_TOGGLE_REPEAT,KEYBIND("editor/toggle_block_repeat"),false);
	edit_menu->add_item("Delete Block",EDIT_DELETE_BLOCK,KEYBIND("editor/delete_block"),false);
	edit_menu->add_separator();
	edit_menu->add_item("Transpose Up Semitone",EDIT_TRANSPOSE_UP_SEMITONE,KEYBIND("editor/transpose_up"),false);
	edit_menu->add_item("Transpose Down Semitone",EDIT_TRANSPOSE_DOWN_SEMITONE,KEYBIND("editor/transpose_down"),false);
	edit_menu->add_item("Transpose Up Octave",EDIT_TRANSPOSE_UP_OCTAVE,KEYBIND("editor/transpose_up_octave"),false);
	edit_menu->add_item("Transpose Down Octave",EDIT_TRANSPOSE_DOWN_OCTAVE,KEYBIND("editor/transpose_down_octave"),false);

}
void TrackEditorTop::selection_menu_about_to_show() {

	selection_menu->clear();
	selection_menu->add_item("Block Begin",SELECTION_BEGIN,KEYBIND("editor/selection_block_begin"),false);
	selection_menu->add_item("Block End",SELECTION_END,KEYBIND("editor/selection_block_end"),false);
	selection_menu->add_item("Block/Column",SELECTION_BLOCK_COLUMN,KEYBIND("editor/select_column_block"),false);
	selection_menu->add_item("Clear",SELECTION_CLEAR,KEYBIND("editor/selection_disable"),false);
	selection_menu->add_separator();
	selection_menu->add_item("Create Block",SELECTION_CREATE_BLOCKS,KEYBIND("editor/selection_create_blocks"),false);
	selection_menu->add_item("Set to Loop Begin/End",SELECTION_MAKE_LOOP,KEYBIND("editor/selection_to_loop"),false);
	selection_menu->add_separator();
	selection_menu->add_item("Scale Volumes",SELECTION_SCALE_VOLUMES,KEYBIND("editor/scale_volumes"),false);
	selection_menu->add_item("Apply Volume Mask",SELECTION_APPLY_VOLUME_MASK,KEYBIND("editor/apply_volume_mask"),false);
	selection_menu->add_separator();
	selection_menu->add_item("Quantize Up",SELECTION_QUANTIZE_UP,KEYBIND("editor/quantize_up"),false);
	selection_menu->add_item("Quantize Nearest",SELECTION_QUANTIZE_NEAREST,KEYBIND("editor/quantize_nearest"),false);
	selection_menu->add_item("Quantize Down",SELECTION_QUANTIZE_DOWN,KEYBIND("editor/quantize_down"),false);
	
}


void TrackEditorTop::set_in_window() {

	GUI::Button *button_play_pattern = add( new GUI::MenuButton( bitmap( BITMAP_CONTROL_PLAY_PATTERN ) ) );
	button_play_pattern->set_tooltip("Play Pattern:\n Block under cursor will\n be played in loop.");
	GUI::Button *button_play_cursor = add( new GUI::MenuButton( bitmap( BITMAP_CONTROL_PLAY_CURSOR ) ) );
	button_play_cursor->set_tooltip("Play from Cursor:\n Song will be played from\n the current cursor position.");
	add( new GUI::VSeparator );
	
	selection_menu = add( new GUI::MenuBox( "Selection" ) );
	
	selection_menu->item_selected_signal.connect( this, &TrackEditorTop::menu_callback );
	selection_menu->pre_show_signal.connect( this ,&TrackEditorTop::selection_menu_about_to_show );
	
	add( new GUI::VSeparator );
	
	edit_menu = add( new GUI::MenuBox("Edit" ) );
	
	edit_menu->item_selected_signal.connect( this, &TrackEditorTop::menu_callback );
	edit_menu->pre_show_signal.connect( this ,&TrackEditorTop::edit_menu_about_to_show );
	                      	
	                      	
	add( new GUI::VSeparator );
	volume_mask_toggle = add( new GUI::MenuButton( bitmap( BITMAP_ICON_VOLUME_MASK ) ) );
	volume_mask_toggle->set_toggle_mode( true );
	volume_mask_toggle->set_tooltip("Toggle Volume Mask Active");
	
	volume_mask = add( new GUI::SpinBox );
	volume_mask->get_range()->config(0,99,99,1);
	volume_mask_toggle->set_tooltip("Volume Mask:\n When active, new notes are \n entered with this volume.\n Otherwise, 99 is the default.");
	add( new GUI::VSeparator );
	add( new GUI::Icon( bitmap( BITMAP_ICON_CURSOR_STEP ) ) );
	cursor_step = add( new GUI::SpinBox );
	cursor_step->get_range()->config(0,16,1,1);
	cursor_step->set_tooltip("Cursor Step:\n Determines how many rows does the\n cursor skip when moving up/down.");
	add( new GUI::VSeparator );
	midi_in_toggle = add( new GUI::MenuButton( bitmap( BITMAP_ICON_MIDI ) ) );
	midi_in_toggle->set_toggle_mode( true );
	add( new GUI::Widget, 1 );
	add( new GUI::Icon( bitmap( BITMAP_ICON_ZOOM ) ) );
	snap_combo = add( new GUI::ComboBox );
	for (int i=0;i<MAX_DIVISORS;i++) {
		
		snap_combo->add_string("1/"+String::num(beat_divisors[i])+" Beat");	
	}
	snap_combo->select(3);
	
	volume_mask_toggle->toggled_signal.connect( this, &TrackEditorTop::volume_mask_toggled );
	volume_mask->get_range()->value_changed_signal.connect( this, &TrackEditorTop::volume_mask_spin_changed );
	cursor_step->get_range()->value_changed_signal.connect( this, &TrackEditorTop::step_spin_changed );
	snap_combo->selected_signal.connect(this, &TrackEditorTop::combo_snap_changed );
	
	scale_input_dialog = new GUI::NumericInputDialog(get_window());
	scale_input_dialog->entered_number_signal.connect( this, &TrackEditorTop::volume_scale_dialog_callback);
	
	barbeat_input_dialog = new GUI::NumericInputDialog(get_window());
	barbeat_input_dialog->entered_number_signal.connect( this, &TrackEditorTop::barbeat_dialog_callback);
	
	marker_input_dialog = new GUI::StringInputDialog(get_window());
	marker_input_dialog->entered_string_signal.connect( this, &TrackEditorTop::marker_dialog_callback);
	
}

TrackEditorTop::TrackEditorTop(GUI_UpdateNotify *p_update_notify) {

	p_update_notify->cursor_step_changed_signal.connect( this, &TrackEditorTop::editing_vars_changed );
	p_update_notify->window_snap_changed_signal.connect( this, &TrackEditorTop::editing_vars_changed );
	p_update_notify->volume_mask_changed_signal.connect( this, &TrackEditorTop::editing_vars_changed );
	updating=false;
	volume_scale_last=100;
	barbeat_last=4;
	
	p_update_notify->editor_volume_scale_request_signal.connect( this, &TrackEditorTop::volume_scale_dialog_show );
	p_update_notify->editor_change_bar_len_request_signal.connect( this, &TrackEditorTop::barbeat_dialog_show );
	p_update_notify->editor_change_marker_request_signal.connect( this, &TrackEditorTop::marker_dialog_show );
		
}


TrackEditorTop::~TrackEditorTop()
{
}


