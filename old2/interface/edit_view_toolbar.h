//
// C++ Interface: edit_view_toolbar
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RESHAKEDEDIT_VIEW_TOOLBAR_H
#define RESHAKEDEDIT_VIEW_TOOLBAR_H

#include "containers/box_container.h"
#include "bundles/menu_box.h"
#include "bundles/combo_box.h"
#include "bundles/spin_box.h"
#include "bundles/input_dialog.h"
#include "widgets/menu_button.h"

#include "interface/gui_update_notify.h"
#include "editor/editor.h"

using namespace GUI;

namespace ReShaked {

/**
	@author Juan Linietsky <reshaked@gmail.com>
*/
class EditViewToolbar : public HBoxContainer {
	
	
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
		
	
	Editor *editor;
	
	MenuBox *selection_menu;
	MenuBox *edit_menu;
	
	MenuButton *volume_mask_enabled;
	SpinBox *volume_mask;
	ComboBox *cursor_step;
	ComboBox *midi_mode;
	ComboBox *snap;
	
	NumericInputDialog *input_bar_len;
	NumericInputDialog *input_volume_scale;
	StringInputDialog *input_marker;
	
	
	double last_scale;
	
	void set_in_window();
	
	void action_callback(int p_action);
	void cursor_set_step_callback(int p_step);
	void volume_set_mask_callback(double p_mask);
	void snap_changed_callback(int p_step);
	
	void marker_changed_callback(String p_text);
	void volume_scale_callback(double p_val);
	void bar_len_changed_callback(double p_val);
	
	bool updating;
	
	void selection_menu_show();
	void edit_menu_show();
public:
	void update_components();
	
	void scale_volumes();
	void edit_marker_at_cursor();
	void change_bar_len_at_cursor();
	
	EditViewToolbar(GUI_UpdateNotify *p_notifier,Editor *p_editor);
	
	~EditViewToolbar();

};

}

#endif
