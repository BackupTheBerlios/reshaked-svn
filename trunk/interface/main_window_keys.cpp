//
// C++ Implementation: main_window_keys
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "main_window.h"


#include "pianokeys.h"

namespace ReShaked {

static const int default_pianokey_bind[MAX_KEYS] = {

	KEY_z,
	KEY_s,
	KEY_x,
	KEY_d,
	KEY_c,
	KEY_v,
	KEY_g,
	KEY_b,
	KEY_h,
	KEY_n,
	KEY_j,
	KEY_m,

	KEY_q,
	KEY_2,
	KEY_w,
	KEY_3,
	KEY_e,
	KEY_r,
	KEY_5,
	KEY_t,
	KEY_6,
	KEY_y,
	KEY_7,
	KEY_u,

	KEY_i,
	KEY_9,
	KEY_o,
	KEY_0,
	KEY_p
};

void MainWindow::create_keybindings() {


	for (int i=0;i<10;i++)
		data.keyboard_codes.set_number(i,KEY_0+i);

	
	data.keyboard_codes.add_key_bind("up","Move Up",KEY_UP,true);
	data.keyboard_codes.add_key_bind("down","Move Down",KEY_DOWN,true);
	data.keyboard_codes.add_key_bind("left","Move Left",KEY_LEFT,true);
	data.keyboard_codes.add_key_bind("right","Move Right",KEY_RIGHT,true);
	data.keyboard_codes.add_key_bind("next_track","Next Track",KEY_MASK_ALT+KEY_RIGHT,true);
	data.keyboard_codes.add_key_bind("prev_track","Prev Track",KEY_MASK_ALT+KEY_LEFT,true);
	data.keyboard_codes.add_key_bind("next_column","Next Column",KEY_TAB,true);
	data.keyboard_codes.add_key_bind("prev_column","Prev Column",KEY_MASK_SHIFT+KEY_TAB,true);
	data.keyboard_codes.add_key_bind("page_up","Page Up",KEY_PAGEUP,true);
	data.keyboard_codes.add_key_bind("page_down","Page Down",KEY_PAGEDOWN,true);
	data.keyboard_codes.add_key_bind("home","Home",KEY_HOME,true);
	data.keyboard_codes.add_key_bind("end","End",KEY_END,true);

	data.keyboard_codes.add_key_bind("editor/page_up_blocksnap","Page Up BlockSnapped",KEY_MASK_ALT+KEY_UP);
	data.keyboard_codes.add_key_bind("editor/page_down_blocksnap","Page Down BlockSnapped",KEY_MASK_ALT+KEY_DOWN);
	
	data.keyboard_codes.add_key_bind("editor/insert","Insert",KEY_INSERT);
	data.keyboard_codes.add_key_bind("editor/delete","Delete",KEY_DELETE);

	
	data.keyboard_codes.add_key_bind("editor/transpose_up","Transpose Up",KEY_MASK_ALT+KEY_q);
	data.keyboard_codes.add_key_bind("editor/transpose_down","Transpose Down",KEY_MASK_ALT+KEY_a);
	
	data.keyboard_codes.add_key_bind("editor/selection_begin","Selection Begin",KEY_MASK_ALT+KEY_b);
	data.keyboard_codes.add_key_bind("editor/selection_end","Selection Begin",KEY_MASK_ALT+KEY_e);
	data.keyboard_codes.add_key_bind("editor/selection_copy","Selection Copy",KEY_MASK_ALT+KEY_c);
	data.keyboard_codes.add_key_bind("editor/selection_paste_insert","Selection Paste Insert",KEY_MASK_ALT+KEY_p);
	data.keyboard_codes.add_key_bind("editor/selection_paste_overwrite","Selection Paste Overwrite",KEY_MASK_ALT+KEY_o);
	data.keyboard_codes.add_key_bind("editor/selection_paste_mix","Selection Paste Mix",KEY_MASK_ALT+KEY_m);
	data.keyboard_codes.add_key_bind("editor/select_column_block","Select Column/Block",KEY_MASK_ALT+KEY_l);
	data.keyboard_codes.add_key_bind("editor/selection_disable","Selection Disable",KEY_MASK_ALT+KEY_u);
	data.keyboard_codes.add_key_bind("editor/selection_zap","Erase Data Under Selection",KEY_MASK_ALT+KEY_z);
	
	data.keyboard_codes.add_key_bind("editor/selection_up","Selection Move Up",KEY_MASK_SHIFT+KEY_UP,true);
	data.keyboard_codes.add_key_bind("editor/selection_down","Selection Move Down",KEY_MASK_SHIFT+KEY_DOWN,true);
	data.keyboard_codes.add_key_bind("editor/selection_left","Selection Move Left",KEY_MASK_SHIFT+KEY_LEFT,true);
	data.keyboard_codes.add_key_bind("editor/selection_right","Selection Move Right",KEY_MASK_SHIFT+KEY_RIGHT,true);
	data.keyboard_codes.add_key_bind("editor/selection_page_up","Selection Page Up",KEY_MASK_SHIFT+KEY_PAGEUP,true);
	data.keyboard_codes.add_key_bind("editor/selection_page_down","Selection Page Down",KEY_MASK_SHIFT+KEY_PAGEDOWN,true);
	data.keyboard_codes.add_key_bind("editor/selection_move_home","Selection Home",KEY_MASK_SHIFT+KEY_HOME,true);
	data.keyboard_codes.add_key_bind("editor/selection_move_end","Selection End",KEY_MASK_SHIFT+KEY_END,true);
	
	data.keyboard_codes.add_key_bind("editor/selection_scale_volumes","Selection Scale Volumes",KEY_MASK_ALT+KEY_j);
	
	data.keyboard_codes.add_key_bind("editor/selection_apply_volume_mask","Selection Apply Volume Mask",KEY_MASK_ALT+KEY_v);
	
	data.keyboard_codes.add_key_bind("editor/delete","Delete",KEY_DELETE);
	
	data.keyboard_codes.add_key_bind("editor/move_note_up","Move Note Up",KEY_MASK_CTRL+KEY_UP,true);
	data.keyboard_codes.add_key_bind("editor/move_note_down","Move Note Down",KEY_MASK_CTRL+KEY_DOWN,true);

	data.keyboard_codes.add_key_bind("editor/next_marker","Goto Next Marker",KEY_PLUS);
	data.keyboard_codes.add_key_bind("editor/previous_marker","Goto Prev Marker",KEY_MINUS);
	
	data.keyboard_codes.add_key_bind("editor/track_mute","Mute/Unmute Track",KEY_F9);
	data.keyboard_codes.add_key_bind("editor/track_solo","Solo Track",KEY_F10);
	
	data.keyboard_codes.add_key_bind("editor/add_column","Add Column",KEY_MASK_ALT+KEY_RIGHTBRACKET);
	data.keyboard_codes.add_key_bind("editor/remove_column","Remove Column",KEY_MASK_ALT+KEY_LEFTBRACKET);


	/*data.keyboard_codes.add_key_bind("editor/automation_mode_stream","Stream Automation Edit",KEY_MASK_CTRL+KEY_T,false,NULL,new GUI_KeybindDescription(blui_list->automation_stream));
	data.keyboard_codes.add_key_bind("editor/automation_mode_envelope","Envelope Automation Edit",KEY_MASK_CTRL+KEY_E,false,NULL,new GUI_KeybindDescription(blui_list->automation_envelope));
	*/
	data.keyboard_codes.add_key_bind("editor/midi_input_insert","Insert Notes from MIDI",KEY_MASK_CTRL+KEY_m,false,NULL/*,new GUI_KeybindDescription(blui_list->midi_in_edit)*/);
	data.keyboard_codes.add_key_bind("editor/quantize","Quantize",KEY_MASK_ALT+KEY_t);
	
	data.keyboard_codes.add_key_bind("note_entry/toggle_volume_mask","Toggle Volume Mask",KEY_COMMA,false,NULL/*,new GUI_KeybindDescription(blui_list->edit_mask)*/);
	
	data.keyboard_codes.add_key_bind("editor/edit_marker","Edit Marker",KEY_MASK_CTRL+KEY_k);
	data.keyboard_codes.add_key_bind("editor/set_loop_begin","Set Loop Begin",KEY_MASK_CTRL+KEY_b);
	data.keyboard_codes.add_key_bind("editor/set_loop_end","Set Loop End",KEY_MASK_CTRL+KEY_r);
	data.keyboard_codes.add_key_bind("editor/selection_to_loop","Selection -> Loop",KEY_MASK_CTRL+KEY_l);
	data.keyboard_codes.add_key_bind("editor/selection_create_blocks","Selection -> Create Blocks",KEY_MASK_CTRL+KEY_a);
	data.keyboard_codes.add_key_bind("editor/toggle_block_repeat","Toggle Block Repeat",KEY_MASK_CTRL+KEY_MASK_SHIFT+KEY_r);
	
	
	data.keyboard_codes.add_key_bind("global/raise_octave","Raise Octave",KEY_ASTERISK);

	data.keyboard_codes.add_key_bind("global/lower_octave","Lower Octave",KEY_SLASH);

	data.keyboard_codes.add_key_bind("note_entry/clear_field","Clear Field",KEY_PERIOD);

	data.keyboard_codes.add_key_bind("note_entry/note_off","Note Off",KEY_BACKQUOTE);
	data.keyboard_codes.add_key_bind("note_entry/toggle_note_edit","Note Edit Mode Toggle",KEY_SPACE);
	for (int i=0;i<MAX_KEYS;i++)
		data.keyboard_codes.add_key_bind((String)"note_entry/"+key_name[i],key_name[i],default_pianokey_bind[i]);
	
	data.keyboard_codes.add_key_bind("note_entry/play_note_at_cursor","Play Note Under Cursor",KEY_4);

	data.keyboard_codes.add_key_bind("note_entry/play_row_at_cursor","Play Row Under Cursor",KEY_8);
	
	data.keyboard_codes.add_key_bind("note_entry/cursor_step_0","Set Cursor Step to 0",KEY_MASK_ALT+KEY_0);
	data.keyboard_codes.add_key_bind("note_entry/cursor_step_1","Set Cursor Step to 1",KEY_MASK_ALT+KEY_1);
	data.keyboard_codes.add_key_bind("note_entry/cursor_step_2","Set Cursor Step to 2",KEY_MASK_ALT+KEY_2);
	data.keyboard_codes.add_key_bind("note_entry/cursor_step_3","Set Cursor Step to 3",KEY_MASK_ALT+KEY_3);
	data.keyboard_codes.add_key_bind("note_entry/cursor_step_4","Set Cursor Step to 4",KEY_MASK_ALT+KEY_4);
	data.keyboard_codes.add_key_bind("note_entry/cursor_step_5","Set Cursor Step to 5",KEY_MASK_ALT+KEY_5);
	data.keyboard_codes.add_key_bind("note_entry/cursor_step_6","Set Cursor Step to 6",KEY_MASK_ALT+KEY_6);
	data.keyboard_codes.add_key_bind("note_entry/cursor_step_7","Set Cursor Step to 7",KEY_MASK_ALT+KEY_7);
	data.keyboard_codes.add_key_bind("note_entry/cursor_step_8","Set Cursor Step to 8",KEY_MASK_ALT+KEY_8);
	data.keyboard_codes.add_key_bind("note_entry/cursor_step_9","Set Cursor Step to 9",KEY_MASK_ALT+KEY_9);
	
	data.keyboard_codes.add_key_bind("editor/window_snap_1","Set Window Snap to 1",KEY_MASK_CTRL+KEY_MASK_SHIFT+KEY_0);
	data.keyboard_codes.add_key_bind("editor/window_snap_2","Set Window Snap to 2",KEY_MASK_CTRL+KEY_MASK_SHIFT+KEY_1);
	data.keyboard_codes.add_key_bind("editor/window_snap_3","Set Window Snap to 3",KEY_MASK_CTRL+KEY_1);
	data.keyboard_codes.add_key_bind("editor/window_snap_4","Set Window Snap to 4",KEY_MASK_CTRL+KEY_2);
	data.keyboard_codes.add_key_bind("editor/window_snap_6","Set Window Snap to 6",KEY_MASK_CTRL+KEY_3);
	data.keyboard_codes.add_key_bind("editor/window_snap_8","Set Window Snap to 8",KEY_MASK_CTRL+KEY_4);
	data.keyboard_codes.add_key_bind("editor/window_snap_12","Set Window Snap to 12",KEY_MASK_CTRL+KEY_5);
	data.keyboard_codes.add_key_bind("editor/window_snap_16","Set Window Snap to 16",KEY_MASK_CTRL+KEY_6);
	data.keyboard_codes.add_key_bind("editor/window_snap_24","Set Window Snap to 24",KEY_MASK_CTRL+KEY_7);
	data.keyboard_codes.add_key_bind("editor/window_snap_32","Set Window Snap to 32",KEY_MASK_CTRL+KEY_8);
	data.keyboard_codes.add_key_bind("editor/window_snap_48","Set Window Snap to 48",KEY_MASK_CTRL+KEY_9);
	data.keyboard_codes.add_key_bind("editor/window_snap_64","Set Window Snap to 64",KEY_MASK_CTRL+KEY_0);
	
	data.keyboard_codes.add_key_bind("editor/window_snap_half","Half Size Snap",KEY_MASK_CTRL+KEY_w);
	data.keyboard_codes.add_key_bind("editor/window_snap_double","Double Size Snap",KEY_MASK_CTRL+KEY_s);
	
	data.keyboard_codes.add_key_bind("editor/quantize_up","Quantize Up",KEY_MASK_CTRL+KEY_u);
	data.keyboard_codes.add_key_bind("editor/quantize_nearest","Quantize Nearest",KEY_MASK_CTRL+KEY_i);
	data.keyboard_codes.add_key_bind("editor/quantize_down","Quantize Down",KEY_MASK_CTRL+KEY_o);
	
	data.keyboard_codes.add_key_bind("editor/bar_length_at_pos","Set Bar Length",KEY_MASK_CTRL+KEY_MASK_SHIFT+KEY_b);
		
	data.keyboard_codes.set_key_bind_key_by_name("actions/undo",KEY_MASK_CTRL+KEY_z);
	data.keyboard_codes.set_key_bind_key_by_name("actions/redo",KEY_MASK_SHIFT+KEY_MASK_CTRL+KEY_z);

	data.keyboard_codes.set_key_bind_key_by_name("actions/play",KEY_F5);
	data.keyboard_codes.set_key_bind_key_by_name("actions/stop",KEY_F8);
	
	data.keyboard_codes.set_key_bind_key_by_name("actions/pause",KEY_MASK_SHIFT+KEY_F8);
	data.keyboard_codes.set_key_bind_key_by_name("actions/play_loop",KEY_MASK_SHIFT+KEY_F5);
	
	data.keyboard_codes.set_key_bind_key_by_name("actions/record",KEY_MASK_SHIFT+KEY_F6);
	data.keyboard_codes.set_key_bind_key_by_name("actions/record_automations",KEY_MASK_SHIFT+KEY_F7);
	
	data.keyboard_codes.set_key_bind_key_by_name("actions/play_block",KEY_F6);
	data.keyboard_codes.set_key_bind_key_by_name("actions/play_from_cursor",KEY_F7);
	
	data.keyboard_codes.set_key_bind_key_by_name("actions/add_track",KEY_MASK_CTRL+KEY_t);
	
	data.keyboard_codes.set_key_bind_key_by_name("actions/toggle_rack",KEY_F4);
	
	data.keyboard_codes.set_key_bind_key_by_name("actions/delete_blocks",KEY_DELETE);
	
	data.keyboard_codes.set_key_bind_key_by_name("actions/song_view",KEY_F1);
	data.keyboard_codes.set_key_bind_key_by_name("actions/edit_view",KEY_F2);
	
	
}

}



