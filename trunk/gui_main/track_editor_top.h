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
	
	void volume_mask_toggled(bool p_to);
	void volume_mask_spin_changed(double p_to_val);	
	void step_spin_changed(double p_to_val);
	void combo_snap_changed(int p_to_val);
	
	void editing_vars_changed();
	bool updating;
	
	void set_in_window();
public:

	TrackEditorTop(GUI_UpdateNotify *p_update_notify);
	~TrackEditorTop();

};

#endif
