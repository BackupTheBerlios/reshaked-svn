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

//	data.keyboard_codes.add_key_bind("home","Home",Qt::ALT+Qt::Key_Home,true);
	//data.keyboard_codes.add_key_bind("end","End",Qt::ALT+Qt::Key_End,true);

	data.keyboard_codes.add_key_bind("page_up_blocksnap","Page Up BlockSnapped",Qt::ALT+Qt::Key_Up,true);
	data.keyboard_codes.add_key_bind("page_down_blocksnap","Page Down BlockSnapped",Qt::ALT+Qt::Key_Down,true);
	
	data.keyboard_codes.add_key_bind("insert","Insert",Qt::Key_Insert,true);
	data.keyboard_codes.add_key_bind("delete","Delete",Qt::Key_Delete,true);

	data.keyboard_codes.add_key_bind("move_note_up","Move Note Up",Qt::CTRL+Qt::Key_Up,true);
	data.keyboard_codes.add_key_bind("move_note_down","Move Note Down",Qt::CTRL+Qt::Key_Down,true);

	data.keyboard_codes.add_key_bind("navigation/next_pattern","Next Pattern",Qt::Key_Plus);

	data.keyboard_codes.add_key_bind("navigation/previous_pattern","Prev Pattern",Qt::Key_Minus);


	data.keyboard_codes.add_key_bind("global/raise_octave","Raise Octave",Qt::Key_Asterisk);

	data.keyboard_codes.add_key_bind("global/lower_octave","Lower Octave",Qt::Key_Slash);

	data.keyboard_codes.add_key_bind("note_entry/clear_field","Clear Field",Qt::Key_Period);

	data.keyboard_codes.add_key_bind("note_entry/note_off","Note Off",Qt::Key_QuoteLeft);
	data.keyboard_codes.add_key_bind("note_entry/toggle_note_edit","Note Edit Mode Toggle",Qt::Key_Space);
	for (int i=0;i<MAX_KEYS;i++)
		data.keyboard_codes.add_key_bind((String)"note_entry/"+key_name[i],key_name[i],default_pianokey_bind[i]);


}






}