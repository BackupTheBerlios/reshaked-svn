//
// C++ Interface: main_window
//
// Description: 
//
//
// Author: Juan Linietsky <reduzio@gmail.com>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "main_window.h"
#include "editor/piano_keys.h"

static const int _default_pianokey_bind[MAX_PIANO_KEYS] = {

	GUI::KEY_z,
	GUI::KEY_s,
	GUI::KEY_x,
	GUI::KEY_d,
	GUI::KEY_c,
	GUI::KEY_v,
	GUI::KEY_g,
	GUI::KEY_b,
	GUI::KEY_h,
	GUI::KEY_n,
	GUI::KEY_j,
	GUI::KEY_m,

	GUI::KEY_q,
	GUI::KEY_2,
	GUI::KEY_w,
	GUI::KEY_3,
	GUI::KEY_e,
	GUI::KEY_r,
	GUI::KEY_5,
	GUI::KEY_t,
	GUI::KEY_6,
	GUI::KEY_y,
	GUI::KEY_7,
	GUI::KEY_u,

	GUI::KEY_i,
	GUI::KEY_9,
	GUI::KEY_o,
	GUI::KEY_0,
	GUI::KEY_p
};


void MainWindow::init_key_bindings() {


	for (int i=0;i<10;i++)
		key_bindings.set_number(i,GUI::KEY_0+i);

	
	key_bindings.add_key_bind("up","Move Up",GUI::KEY_UP,true);
	key_bindings.add_key_bind("down","Move Down",GUI::KEY_DOWN,true);
	key_bindings.add_key_bind("left","Move Left",GUI::KEY_LEFT,true);
	key_bindings.add_key_bind("right","Move Right",GUI::KEY_RIGHT,true);
	key_bindings.add_key_bind("next_track","Next Track",GUI::KEY_MASK_ALT+GUI::KEY_RIGHT,true);
	key_bindings.add_key_bind("prev_track","Prev Track",GUI::KEY_MASK_ALT+GUI::KEY_LEFT,true);
	key_bindings.add_key_bind("next_column","Next Column",GUI::KEY_TAB,true);
	key_bindings.add_key_bind("prev_column","Prev Column",GUI::KEY_MASK_SHIFT+GUI::KEY_TAB,true);
	key_bindings.add_key_bind("page_up","Page Up",GUI::KEY_PAGEUP,true);
	key_bindings.add_key_bind("page_down","Page Down",GUI::KEY_PAGEDOWN,true);
	key_bindings.add_key_bind("home","Home",GUI::KEY_HOME,true);
	key_bindings.add_key_bind("end","End",GUI::KEY_END,true);

	key_bindings.add_key_bind("editor/block_up","Block Up",GUI::KEY_MASK_ALT+GUI::KEY_UP);
	key_bindings.add_key_bind("editor/block_down","Block Down",GUI::KEY_MASK_ALT+GUI::KEY_DOWN);
	
	key_bindings.add_key_bind("editor/insert","Insert",GUI::KEY_INSERT);
	key_bindings.add_key_bind("editor/delete","Delete",GUI::KEY_DELETE);

	
	key_bindings.add_key_bind("editor/transpose_up","Transpose Up",GUI::KEY_MASK_ALT+GUI::KEY_q);
	key_bindings.add_key_bind("editor/transpose_down","Transpose Down",GUI::KEY_MASK_ALT+GUI::KEY_a);
	key_bindings.add_key_bind("editor/transpose_up_octave","Transpose Up Octave",GUI::KEY_MASK_SHIFT+GUI::KEY_MASK_ALT+GUI::KEY_q);
	key_bindings.add_key_bind("editor/transpose_down_octave","Transpose Down Octave",GUI::KEY_MASK_SHIFT+GUI::KEY_MASK_ALT+GUI::KEY_a);
	
	key_bindings.add_key_bind("editor/selection_block_begin","Selection Block Begin",GUI::KEY_MASK_ALT+GUI::KEY_b);
	key_bindings.add_key_bind("editor/selection_block_end","Selection Block Begin",GUI::KEY_MASK_ALT+GUI::KEY_e);
	key_bindings.add_key_bind("editor/selection_copy","Selection Copy",GUI::KEY_MASK_ALT+GUI::KEY_c);
	key_bindings.add_key_bind("editor/selection_paste_insert","Selection Paste Insert",GUI::KEY_MASK_ALT+GUI::KEY_p);
	key_bindings.add_key_bind("editor/selection_paste_overwrite","Selection Paste Overwrite",GUI::KEY_MASK_ALT+GUI::KEY_o);
	key_bindings.add_key_bind("editor/selection_paste_mix","Selection Paste Mix",GUI::KEY_MASK_ALT+GUI::KEY_m);
	key_bindings.add_key_bind("editor/select_column_block","Select Column/Block",GUI::KEY_MASK_ALT+GUI::KEY_l);
	key_bindings.add_key_bind("editor/selection_disable","Selection Disable",GUI::KEY_MASK_ALT+GUI::KEY_u);
	key_bindings.add_key_bind("editor/selection_zap","Erase Data Under Selection",GUI::KEY_MASK_ALT+GUI::KEY_z);
	
	key_bindings.add_key_bind("editor/selection_up","Selection Move Up",GUI::KEY_MASK_SHIFT+GUI::KEY_UP,true);
	key_bindings.add_key_bind("editor/selection_down","Selection Move Down",GUI::KEY_MASK_SHIFT+GUI::KEY_DOWN,true);
	key_bindings.add_key_bind("editor/selection_left","Selection Move Left",GUI::KEY_MASK_SHIFT+GUI::KEY_LEFT,true);
	key_bindings.add_key_bind("editor/selection_right","Selection Move Right",GUI::KEY_MASK_SHIFT+GUI::KEY_RIGHT,true);
	key_bindings.add_key_bind("editor/selection_page_up","Selection Page Up",GUI::KEY_MASK_SHIFT+GUI::KEY_PAGEUP,true);
	key_bindings.add_key_bind("editor/selection_page_down","Selection Page Down",GUI::KEY_MASK_SHIFT+GUI::KEY_PAGEDOWN,true);
	key_bindings.add_key_bind("editor/selection_home","Selection Home",GUI::KEY_MASK_SHIFT+GUI::KEY_HOME,true);
	key_bindings.add_key_bind("editor/selection_end","Selection End",GUI::KEY_MASK_SHIFT+GUI::KEY_END,true);
	
	key_bindings.add_key_bind("editor/scale_volumes","Scale Volumes",GUI::KEY_MASK_ALT+GUI::KEY_j);
	
	key_bindings.add_key_bind("editor/apply_volume_mask","Apply Volume Mask",GUI::KEY_MASK_ALT+GUI::KEY_v);
		
	key_bindings.add_key_bind("editor/move_note_up","Move Note Up",GUI::KEY_MASK_CTRL+GUI::KEY_UP,true);
	key_bindings.add_key_bind("editor/move_note_down","Move Note Down",GUI::KEY_MASK_CTRL+GUI::KEY_DOWN,true);

	key_bindings.add_key_bind("editor/next_marker","Goto Next Marker",GUI::KEY_PLUS);
	key_bindings.add_key_bind("editor/previous_marker","Goto Prev Marker",GUI::KEY_MINUS);
	
	key_bindings.add_key_bind("editor/track_mute","Mute/Unmute Track",GUI::KEY_F9);
	key_bindings.add_key_bind("editor/track_solo","Solo Track",GUI::KEY_F10);
	
	key_bindings.add_key_bind("editor/add_column","Add Column",GUI::KEY_MASK_ALT+GUI::KEY_RIGHTBRACKET);
	key_bindings.add_key_bind("editor/remove_column","Remove Column",GUI::KEY_MASK_ALT+GUI::KEY_LEFTBRACKET);


	/*key_bindings.add_key_bind("editor/automation_mode_stream","Stream Automation Edit",GUI::KEY_MASK_CTRL+GUI::KEY_T,false,NULL,new GUI_KeybindDescription(blui_list->automation_stream));
	key_bindings.add_key_bind("editor/automation_mode_envelope","Envelope Automation Edit",GUI::KEY_MASK_CTRL+GUI::KEY_E,false,NULL,new GUI_KeybindDescription(blui_list->automation_envelope));
	*/
	key_bindings.add_key_bind("editor/midi_input_insert","Insert Notes from MIDI",GUI::KEY_MASK_CTRL+GUI::KEY_m,false,NULL/*,new GUI_KeybindDescription(blui_list->midi_in_edit)*/);
	key_bindings.add_key_bind("editor/quantize","Quantize",GUI::KEY_MASK_ALT+GUI::KEY_t);
	
	key_bindings.add_key_bind("note_entry/toggle_volume_mask","Toggle Volume Mask",GUI::KEY_COMMA,false,NULL/*,new GUI_KeybindDescription(blui_list->edit_mask)*/);
	
	key_bindings.add_key_bind("editor/edit_marker","Edit Marker",GUI::KEY_MASK_CTRL+GUI::KEY_k);
	key_bindings.add_key_bind("editor/set_loop_begin","Set Loop Begin",GUI::KEY_MASK_CTRL+GUI::KEY_b);
	key_bindings.add_key_bind("editor/set_loop_end","Set Loop End",GUI::KEY_MASK_CTRL+GUI::KEY_e);
	key_bindings.add_key_bind("editor/selection_to_loop","Selection -> Loop",GUI::KEY_MASK_CTRL+GUI::KEY_l);
	key_bindings.add_key_bind("editor/selection_create_blocks","Selection -> Create Blocks",GUI::KEY_MASK_CTRL+GUI::KEY_c);
	key_bindings.add_key_bind("editor/toggle_block_repeat","Toggle Block Repeat",GUI::KEY_MASK_ALT+GUI::KEY_r);
	key_bindings.add_key_bind("editor/delete_block","Delete Block",GUI::KEY_MASK_CTRL+GUI::KEY_MASK_SHIFT+GUI::KEY_d);
	key_bindings.add_key_bind("editor/resize_block","Resize Block",GUI::KEY_MASK_CTRL+GUI::KEY_r);
	
	
	key_bindings.add_key_bind("note_entry/clear_field","Clear Field",GUI::KEY_PERIOD);

	key_bindings.add_key_bind("note_entry/note_off","Note Off",GUI::KEY_BACKQUOTE);
	key_bindings.add_key_bind("editor/toggle_front_back_edit","Toggle Front/Back Edit",GUI::KEY_SPACE);
	for (int i=0;i<MAX_PIANO_KEYS;i++)
		key_bindings.add_key_bind((String)"note_entry/"+piano_key_name[i],piano_key_name[i],_default_pianokey_bind[i]);
	
	key_bindings.add_key_bind("note_entry/play_note_at_cursor","Play Note Under Cursor",GUI::KEY_4);

	key_bindings.add_key_bind("note_entry/play_row_at_cursor","Play Row Under Cursor",GUI::KEY_8);
	
	key_bindings.add_key_bind("editor/cursor_step_0","Set Cursor Step to 0",GUI::KEY_MASK_ALT+GUI::KEY_0);
	key_bindings.add_key_bind("editor/cursor_step_1","Set Cursor Step to 1",GUI::KEY_MASK_ALT+GUI::KEY_1);
	key_bindings.add_key_bind("editor/cursor_step_2","Set Cursor Step to 2",GUI::KEY_MASK_ALT+GUI::KEY_2);
	key_bindings.add_key_bind("editor/cursor_step_3","Set Cursor Step to 3",GUI::KEY_MASK_ALT+GUI::KEY_3);
	key_bindings.add_key_bind("editor/cursor_step_4","Set Cursor Step to 4",GUI::KEY_MASK_ALT+GUI::KEY_4);
	key_bindings.add_key_bind("editor/cursor_step_5","Set Cursor Step to 5",GUI::KEY_MASK_ALT+GUI::KEY_5);
	key_bindings.add_key_bind("editor/cursor_step_6","Set Cursor Step to 6",GUI::KEY_MASK_ALT+GUI::KEY_6);
	key_bindings.add_key_bind("editor/cursor_step_7","Set Cursor Step to 7",GUI::KEY_MASK_ALT+GUI::KEY_7);
	key_bindings.add_key_bind("editor/cursor_step_8","Set Cursor Step to 8",GUI::KEY_MASK_ALT+GUI::KEY_8);
	key_bindings.add_key_bind("editor/cursor_step_9","Set Cursor Step to 9",GUI::KEY_MASK_ALT+GUI::KEY_9);
	
	key_bindings.add_key_bind("editor/window_snap_1","Set Window Snap to 1",GUI::KEY_MASK_CTRL+GUI::KEY_MASK_SHIFT+GUI::KEY_0);
	key_bindings.add_key_bind("editor/window_snap_2","Set Window Snap to 2",GUI::KEY_MASK_CTRL+GUI::KEY_MASK_SHIFT+GUI::KEY_1);
	key_bindings.add_key_bind("editor/window_snap_3","Set Window Snap to 3",GUI::KEY_MASK_CTRL+GUI::KEY_1);
	key_bindings.add_key_bind("editor/window_snap_4","Set Window Snap to 4",GUI::KEY_MASK_CTRL+GUI::KEY_2);
	key_bindings.add_key_bind("editor/window_snap_6","Set Window Snap to 6",GUI::KEY_MASK_CTRL+GUI::KEY_3);
	key_bindings.add_key_bind("editor/window_snap_8","Set Window Snap to 8",GUI::KEY_MASK_CTRL+GUI::KEY_4);
	key_bindings.add_key_bind("editor/window_snap_12","Set Window Snap to 12",GUI::KEY_MASK_CTRL+GUI::KEY_5);
	key_bindings.add_key_bind("editor/window_snap_16","Set Window Snap to 16",GUI::KEY_MASK_CTRL+GUI::KEY_6);
	key_bindings.add_key_bind("editor/window_snap_24","Set Window Snap to 24",GUI::KEY_MASK_CTRL+GUI::KEY_7);
	key_bindings.add_key_bind("editor/window_snap_32","Set Window Snap to 32",GUI::KEY_MASK_CTRL+GUI::KEY_8);
	key_bindings.add_key_bind("editor/window_snap_48","Set Window Snap to 48",GUI::KEY_MASK_CTRL+GUI::KEY_9);
	key_bindings.add_key_bind("editor/window_snap_64","Set Window Snap to 64",GUI::KEY_MASK_CTRL+GUI::KEY_0);
	
	key_bindings.add_key_bind("editor/window_snap_half","Half Size Snap",GUI::KEY_MASK_CTRL+GUI::KEY_w);
	key_bindings.add_key_bind("editor/window_snap_double","Double Size Snap",GUI::KEY_MASK_CTRL+GUI::KEY_s);
	
	key_bindings.add_key_bind("editor/quantize_up","Quantize Up",GUI::KEY_MASK_CTRL+GUI::KEY_u);
	key_bindings.add_key_bind("editor/quantize_nearest","Quantize Nearest",GUI::KEY_MASK_CTRL+GUI::KEY_i);
	key_bindings.add_key_bind("editor/quantize_down","Quantize Down",GUI::KEY_MASK_CTRL+GUI::KEY_o);
	
	key_bindings.add_key_bind("editor/bar_length_at_pos","Set Bar Length",GUI::KEY_MASK_CTRL+GUI::KEY_MASK_SHIFT+GUI::KEY_b);
		
	key_bindings.add_key_bind("global/raise_octave","Raise Octave",GUI::KEY_KP_MULTIPLY);
	
	key_bindings.add_key_bind("global/lower_octave","Lower Octave",GUI::KEY_KP_DIVIDE);
	key_bindings.add_key_bind("global/add_node","Add Node",GUI::KEY_MASK_CTRL+GUI::KEY_a);
		
	key_bindings.set_key_bind_key_by_name("global/undo",GUI::KEY_MASK_CTRL+GUI::KEY_z);
	key_bindings.set_key_bind_key_by_name("global/redo",GUI::KEY_MASK_SHIFT+GUI::KEY_MASK_CTRL+GUI::KEY_z);

	key_bindings.set_key_bind_key_by_name("global/play",GUI::KEY_F5);
	key_bindings.set_key_bind_key_by_name("global/stop",GUI::KEY_F8);
	
	key_bindings.set_key_bind_key_by_name("global/pause",GUI::KEY_MASK_SHIFT+GUI::KEY_F8);
	key_bindings.set_key_bind_key_by_name("global/play_loop",GUI::KEY_MASK_SHIFT+GUI::KEY_F5);
	
	key_bindings.set_key_bind_key_by_name("global/record",GUI::KEY_MASK_SHIFT+GUI::KEY_F6);
	key_bindings.set_key_bind_key_by_name("global/record_automations",GUI::KEY_MASK_SHIFT+GUI::KEY_F7);
	
	key_bindings.set_key_bind_key_by_name("global/play_block",GUI::KEY_F6);
	key_bindings.set_key_bind_key_by_name("global/play_from_cursor",GUI::KEY_F7);
	
//	key_bindings.set_key_bind_key_by_name("global/add_track",GUI::KEY_MASK_CTRL+GUI::KEY_t);
	
	
//	key_bindings.set_key_bind_key_by_name("global/delete_blocks",GUI::KEY_DELETE);
	
	key_bindings.add_key_bind("global/song_view","Song View",GUI::KEY_F1);
	key_bindings.add_key_bind("global/track_view","Track View",GUI::KEY_F2);
	
	key_bindings.add_key_bind("global/graph_view","Graph View",GUI::KEY_F3);
	
	
	
}
