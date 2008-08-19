//
// C++ Interface: track_editor_top
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TRACK_EDITOR_TOP_H
#define TRACK_EDITOR_TOP_H

#include "containers/box_container.h"
#include "widgets/menu_button.h"
#include "bundles/menu_box.h"
#include "bundles/spin_box.h"
#include "bundles/combo_box.h"
#include "widgets/icon.h"
#include "gui_main/gui_update_notify.h"
#include "bundles/input_dialog.h"
/**
	@author Juan Linietsky <reduzio@gmail.com>
*/
class TrackEditorTop : public GUI::HBoxContainer {

	enum Action {
			
		PLAY_FROM_CURSOR,
		PLAY_BLOCK_LOOPED,
  		SELECTION_BEGIN,
    		SELECTION_END,
		SELECTION_BLOCK_COLUMN,
		SELECTION_CLEAR,
		SELECTION_CREATE_BLOCKS,
		SELECTION_MAKE_LOOP,
		SELECTION_SCALE_VOLUMES,
		SELECTION_APPLY_VOLUME_MASK,
		SELECTION_QUANTIZE_UP,
		SELECTION_QUANTIZE_NEAREST,
		SELECTION_QUANTIZE_DOWN,
		EDIT_COPY,
		EDIT_CUT,
		EDIT_PASTE, // paste overwrite
		EDIT_PASTE_INSERT,
		EDIT_PASTE_MIX,
		EDIT_MARKER,
		EDIT_BAR_LENTH,
		EDIT_SET_LOOP_BEGIN,
		EDIT_SET_LOOP_END,
		EDIT_TOGGLE_REPEAT,
		EDIT_DELETE_BLOCK,
		EDIT_RESIZE_BLOCK,
		EDIT_TRANSPOSE_UP_SEMITONE,
		EDIT_TRANSPOSE_DOWN_SEMITONE,
		EDIT_TRANSPOSE_UP_OCTAVE,
		EDIT_TRANSPOSE_DOWN_OCTAVE,
	};

	GUI::MenuBox * edit_menu;
	GUI::MenuBox *selection_menu;
	GUI::Button *volume_mask_toggle;
	GUI::SpinBox *volume_mask;
	GUI::SpinBox *cursor_step;
	GUI::Button *midi_in_toggle;
	GUI::ComboBox *snap_combo;
	GUI::NumericInputDialog *scale_input_dialog;
	GUI::NumericInputDialog *barbeat_input_dialog;
	GUI::StringInputDialog *marker_input_dialog;
	float volume_scale_last;
	float barbeat_last;
	
	void volume_mask_toggled(bool p_to);
	void volume_mask_spin_changed(double p_to_val);	
	void step_spin_changed(double p_to_val);
	void combo_snap_changed(int p_to_val);
	
	void volume_scale_dialog_callback(double p_val);
	void volume_scale_dialog_show();
	
	void barbeat_dialog_callback(double p_val);
	void barbeat_dialog_show();
	
	void marker_dialog_callback(String p_text);
	void marker_dialog_show();
	
	void menu_callback(int p_option);
	void editing_vars_changed();
	bool updating;
	
	void edit_menu_about_to_show();
	void selection_menu_about_to_show();	
	void set_in_window();
public:

	TrackEditorTop(GUI_UpdateNotify *p_update_notify);
	~TrackEditorTop();

};

#endif
