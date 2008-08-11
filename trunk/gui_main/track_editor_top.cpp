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

void TrackEditorTop::editing_vars_changed() {

	updating=true;

	cursor_step->get_range()->set( Editor::get_singleton()->get_cursor_step() );
	snap_combo->select( Editor::get_singleton()->get_cursor_zoom_divisor() );

	updating=false;
}
void TrackEditorTop::volume_mask_toggled(bool p_to) {

}
void TrackEditorTop::volume_mask_spin_changed(double p_to_val) {

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


void TrackEditorTop::set_in_window() {

	GUI::Button *button_play_pattern = add( new GUI::MenuButton( bitmap( BITMAP_CONTROL_PLAY_PATTERN ) ) );
	GUI::Button *button_play_cursor = add( new GUI::MenuButton( bitmap( BITMAP_CONTROL_PLAY_CURSOR ) ) );
	add( new GUI::VSeparator );
	
	selection_menu = add( new GUI::MenuBox( "Selection" ) );
	
	selection_menu->add_item("Block Begin",SELECTION_BEGIN);
	selection_menu->add_item("Block end",SELECTION_END);
	selection_menu->add_item("Block/Column",SELECTION_BLOCK_COLUMN);
	selection_menu->add_item("Clear",SELECTION_CLEAR);
	selection_menu->add_separator();
	selection_menu->add_item("Create Block",SELECTION_CREATE_BLOCKS);
	selection_menu->add_item("Set to Loop Begin/End",SELECTION_MAKE_LOOP);
	selection_menu->add_separator();
	selection_menu->add_item("Scale Volumes",SELECTION_SCALE_VOLUMES);
	selection_menu->add_item("Apply Volume Mask",SELECTION_APPLY_VOLUME_MASK);
	selection_menu->add_separator();
	selection_menu->add_item("Quantize Up",SELECTION_QUANTIZE_UP);
	selection_menu->add_item("Quantize Nearest",SELECTION_QUANTIZE_NEAREST);
	selection_menu->add_item("Quantize Down",SELECTION_QUANTIZE_DOWN);
	
	add( new GUI::VSeparator );
	
	edit_menu = add( new GUI::MenuBox("Edit" ) );
	
	edit_menu->add_item("Copy",EDIT_COPY);
	edit_menu->add_item("Cut",EDIT_CUT);
	edit_menu->add_item("Paste",EDIT_PASTE);
	edit_menu->add_item("Paste Insert",EDIT_PASTE_INSERT);
	edit_menu->add_item("Paste Mix",EDIT_PASTE_MIX);
	edit_menu->add_separator();
	edit_menu->add_item("Marker",EDIT_MARKER);
	edit_menu->add_item("Bar Length",EDIT_BAR_LENTH);
	edit_menu->add_separator();
	edit_menu->add_item("Set Loop Begin",EDIT_SET_LOOP_BEGIN);
	edit_menu->add_item("Set Loop End",EDIT_SET_LOOP_END);
	edit_menu->add_separator();
	edit_menu->add_item("Transpose Up Semitone",EDIT_TRANSPOSE_UP_SEMITONE);
	edit_menu->add_item("Transpose Down Semitone",EDIT_TRANSPOSE_DOWN_SEMITONE);
	edit_menu->add_item("Transpose Up Octave",EDIT_TRANSPOSE_UP_OCTAVE);
	edit_menu->add_item("Transpose Down Octave",EDIT_TRANSPOSE_DOWN_OCTAVE);
	                      	
	                      	
	add( new GUI::VSeparator );
	volume_mask_toggle = add( new GUI::MenuButton( bitmap( BITMAP_ICON_VOLUME_MASK ) ) );
	volume_mask_toggle->set_toggle_mode( true );
	volume_mask = add( new GUI::SpinBox );
	volume_mask->get_range()->config(0,99,99,1);
	add( new GUI::VSeparator );
	add( new GUI::Icon( bitmap( BITMAP_ICON_CURSOR_STEP ) ) );
	cursor_step = add( new GUI::SpinBox );
	cursor_step->get_range()->config(0,16,1,1);
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
	
}

TrackEditorTop::TrackEditorTop(GUI_UpdateNotify *p_update_notify) {

	p_update_notify->cursor_step_changed_signal.connect( this, &TrackEditorTop::editing_vars_changed );
	p_update_notify->window_snap_changed_signal.connect( this, &TrackEditorTop::editing_vars_changed );
	updating=false;
}


TrackEditorTop::~TrackEditorTop()
{
}


