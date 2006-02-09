//
// C++ Implementation: main_window_keys
//
// Description: 
//
//
// Author: red <red@killy>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//


#include "main_window.h"
#include "pianokeys.h"

namespace ReShaked {

static const int default_pianokey_bind[MAX_KEYS] = {

	Qt::Key_Z,
	Qt::Key_S,
	Qt::Key_X,
	Qt::Key_D,
	Qt::Key_C,
	Qt::Key_V,
	Qt::Key_G,
	Qt::Key_B,
	Qt::Key_H,
	Qt::Key_N,
	Qt::Key_J,
	Qt::Key_M,

	Qt::Key_Q,
	Qt::Key_2,
	Qt::Key_W,
	Qt::Key_3,
	Qt::Key_E,
	Qt::Key_R,
	Qt::Key_5,
	Qt::Key_T,
	Qt::Key_6,
	Qt::Key_Y,
	Qt::Key_7,
	Qt::Key_U,

	Qt::Key_I,
	Qt::Key_9,
	Qt::Key_O,
	Qt::Key_0,
	Qt::Key_P
};

void MainWindow::create_keybindings() {


	for (int i=0;i<10;i++)
		data.keyboard_codes.set_number(i,Qt::Key_0+i);

	
	data.keyboard_codes.add_key_bind("up","Move Up",Qt::Key_Up,true);
	data.keyboard_codes.add_key_bind("down","Move Down",Qt::Key_Down,true);
	data.keyboard_codes.add_key_bind("left","Move Left",Qt::Key_Left,true);
	data.keyboard_codes.add_key_bind("right","Move Right",Qt::Key_Right,true);
	data.keyboard_codes.add_key_bind("next_track","Next Track",Qt::Key_Tab,true);
	data.keyboard_codes.add_key_bind("prev_track","Prev Track",Qt::Key_Backtab,true);
	data.keyboard_codes.add_key_bind("page_up","Page Up",Qt::Key_PageUp,true);
	data.keyboard_codes.add_key_bind("page_down","Page Down",Qt::Key_PageDown,true);
	data.keyboard_codes.add_key_bind("home","Home",Qt::Key_Home,true);
	data.keyboard_codes.add_key_bind("end","End",Qt::Key_End,true);

	data.keyboard_codes.add_key_bind("editor/page_up_blocksnap","Page Up BlockSnapped",Qt::ALT+Qt::Key_Up);
	data.keyboard_codes.add_key_bind("editor/page_down_blocksnap","Page Down BlockSnapped",Qt::ALT+Qt::Key_Down);
	
	data.keyboard_codes.add_key_bind("editor/insert","Insert",Qt::Key_Insert);
	data.keyboard_codes.add_key_bind("editor/delete","Delete",Qt::Key_Delete);

	
	data.keyboard_codes.add_key_bind("editor/transpose_up","Transpose Up",Qt::ALT+Qt::Key_Q);
	data.keyboard_codes.add_key_bind("editor/transpose_down","Transpose Down",Qt::ALT+Qt::Key_A);
	
	data.keyboard_codes.add_key_bind("editor/selection_begin","Selection Begin",Qt::ALT+Qt::Key_B);
	data.keyboard_codes.add_key_bind("editor/selection_end","Selection Begin",Qt::ALT+Qt::Key_E);
	data.keyboard_codes.add_key_bind("editor/selection_copy","Selection Copy",Qt::ALT+Qt::Key_C);
	data.keyboard_codes.add_key_bind("editor/selection_paste_insert","Selection Paste Insert",Qt::ALT+Qt::Key_I);
	data.keyboard_codes.add_key_bind("editor/selection_paste_overwrite","Selection Paste Overwrite",Qt::ALT+Qt::Key_P);
	data.keyboard_codes.add_key_bind("editor/selection_paste_mix","Selection Paste Mix",Qt::ALT+Qt::Key_M);
	data.keyboard_codes.add_key_bind("editor/select_column_block_all","Select Column/Block/All",Qt::ALT+Qt::Key_L);
	data.keyboard_codes.add_key_bind("editor/selection_disable","Selection Disable",Qt::ALT+Qt::Key_U);
	data.keyboard_codes.add_key_bind("editor/selection_zap","Erase Data Under Selection",Qt::ALT+Qt::Key_Z);
	
	data.keyboard_codes.add_key_bind("editor/selection_up","Selection Move Up",Qt::SHIFT+Qt::Key_Up,true);
	data.keyboard_codes.add_key_bind("editor/selection_down","Selection Move Down",Qt::SHIFT+Qt::Key_Down,true);
	data.keyboard_codes.add_key_bind("editor/selection_left","Selection Move Left",Qt::SHIFT+Qt::Key_Left,true);
	data.keyboard_codes.add_key_bind("editor/selection_right","Selection Move Right",Qt::SHIFT+Qt::Key_Right,true);
	data.keyboard_codes.add_key_bind("editor/selection_page_up","Selection Page Up",Qt::SHIFT+Qt::Key_PageUp,true);
	data.keyboard_codes.add_key_bind("editor/selection_page_down","Selection Page Down",Qt::SHIFT+Qt::Key_PageDown,true);
	data.keyboard_codes.add_key_bind("editor/selection_move_home","Selection Home",Qt::SHIFT+Qt::Key_Home,true);
	data.keyboard_codes.add_key_bind("editor/selection_move_end","Selection End",Qt::SHIFT+Qt::Key_End,true);
	
	
	data.keyboard_codes.add_key_bind("editor/delete","Delete",Qt::Key_Delete);
	
	data.keyboard_codes.add_key_bind("editor/move_note_up","Move Note Up",Qt::CTRL+Qt::Key_Up,true);
	data.keyboard_codes.add_key_bind("editor/move_note_down","Move Note Down",Qt::CTRL+Qt::Key_Down,true);

	data.keyboard_codes.add_key_bind("editor/next_pattern","Next Pattern",Qt::Key_Plus);
	

	data.keyboard_codes.add_key_bind("editor/previous_pattern","Prev Pattern",Qt::Key_Minus);
	
	data.keyboard_codes.add_key_bind("editor/add_column","Add Column",Qt::ALT+Qt::Key_BracketRight);
	data.keyboard_codes.add_key_bind("editor/remove_column","Remove Column",Qt::ALT+Qt::Key_BracketLeft);


	data.keyboard_codes.add_key_bind("editor/automation_mode_stream","Stream Automation Edit",Qt::CTRL+Qt::Key_T,false,NULL,new QWidget_KeybindDescription(blui_list->automation_stream));
	data.keyboard_codes.add_key_bind("editor/automation_mode_envelope","Envelope Automation Edit",Qt::CTRL+Qt::Key_E,false,NULL,new QWidget_KeybindDescription(blui_list->automation_envelope));
	data.keyboard_codes.add_key_bind("editor/midi_input_assign","Midi Input Assign",Qt::CTRL+Qt::Key_M,false,NULL,new QWidget_KeybindDescription(blui_list->midi_in_config));
	data.keyboard_codes.add_key_bind("editor/quantize","Quantize",Qt::ALT+Qt::Key_T);
	
	data.keyboard_codes.add_key_bind("note_entry/toggle_volume_mask","Toggle Volume Mask",Qt::Key_Comma,false,NULL,new QWidget_KeybindDescription(blui_list->edit_mask));
	

	data.keyboard_codes.add_key_bind("global/raise_octave","Raise Octave",Qt::Key_Asterisk);

	data.keyboard_codes.add_key_bind("global/lower_octave","Lower Octave",Qt::Key_Slash);

	data.keyboard_codes.add_key_bind("note_entry/clear_field","Clear Field",Qt::Key_Period);

	data.keyboard_codes.add_key_bind("note_entry/note_off","Note Off",Qt::Key_QuoteLeft);
	data.keyboard_codes.add_key_bind("note_entry/toggle_note_edit","Note Edit Mode Toggle",Qt::Key_Space);
	for (int i=0;i<MAX_KEYS;i++)
		data.keyboard_codes.add_key_bind((String)"note_entry/"+key_name[i],key_name[i],default_pianokey_bind[i]);
	
	data.keyboard_codes.add_key_bind("note_entry/play_note_at_cursor","Play Note Under Cursor",Qt::Key_4);

	data.keyboard_codes.add_key_bind("note_entry/play_row_at_cursor","Play Row Under Cursor",Qt::Key_8);
	data.keyboard_codes.set_key_bind_key_by_name("actions/undo",Qt::CTRL+Qt::Key_Z);
	data.keyboard_codes.set_key_bind_key_by_name("actions/redo",Qt::SHIFT+Qt::CTRL+Qt::Key_Z);

	data.keyboard_codes.set_key_bind_key_by_name("actions/play",Qt::Key_F5);
	data.keyboard_codes.set_key_bind_key_by_name("actions/stop",Qt::Key_F8);
	
	data.keyboard_codes.set_key_bind_key_by_name("actions/pause",Qt::SHIFT+Qt::Key_F8);
	data.keyboard_codes.set_key_bind_key_by_name("actions/play_loop",Qt::SHIFT+Qt::Key_F5);
	
	data.keyboard_codes.set_key_bind_key_by_name("actions/record",Qt::SHIFT+Qt::Key_F6);
	data.keyboard_codes.set_key_bind_key_by_name("actions/record_automations",Qt::SHIFT+Qt::Key_F7);
	
	data.keyboard_codes.set_key_bind_key_by_name("actions/play_block",Qt::Key_F6);
	data.keyboard_codes.set_key_bind_key_by_name("actions/play_from_cursor",Qt::Key_F7);
	
	data.keyboard_codes.set_key_bind_key_by_name("actions/add_track",Qt::CTRL+Qt::Key_T);
	
	data.keyboard_codes.set_key_bind_key_by_name("actions/toggle_rack",Qt::Key_F4);
	
	data.keyboard_codes.set_key_bind_key_by_name("actions/delete_blocks",Qt::Key_Delete);
	
	data.keyboard_codes.set_key_bind_key_by_name("actions/song_view",Qt::Key_F1);
	data.keyboard_codes.set_key_bind_key_by_name("actions/edit_view",Qt::Key_F2);
	data.keyboard_codes.set_key_bind_key_by_name("actions/mix_view",Qt::Key_F3);
	
}






}