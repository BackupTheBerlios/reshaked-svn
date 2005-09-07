

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



void MainWindow::add_default_key_binds() {


	for (int i=0;i<10;i++)
		engine.key_input.set_number(i,Qt::Key_0+i);

	engine.key_input.add_key_bind("up","Move Up",Qt::Key_Up,true);
	engine.key_input.add_key_bind("down","Move Down",Qt::Key_Down,true);
	engine.key_input.add_key_bind("left","Move Left",Qt::Key_Left,true);
	engine.key_input.add_key_bind("right","Move Right",Qt::Key_Right,true);
	engine.key_input.add_key_bind("tab","Tab",Qt::Key_Tab,true);
	engine.key_input.add_key_bind("backtab","BackTab",Qt::Key_Tab,true);
	engine.key_input.add_key_bind("page_up","Page Up",Qt::Key_PageUp,true);
	engine.key_input.add_key_bind("page_down","Page Down",Qt::Key_PageDown,true);

	engine.key_input.add_key_bind("home","Home",Qt::Key_Home,true);
	engine.key_input.add_key_bind("end","End",Qt::Key_End,true);

	engine.key_input.add_key_bind("insert","Insert",Qt::Key_Insert,true);
	engine.key_input.add_key_bind("delete","Delete",Qt::Key_Delete,true);


	engine.key_input.add_key_bind("navigation/next_pattern","Next Pattern",Qt::Key_Plus);

	engine.key_input.add_key_bind("navigation/previous_pattern","Prev Pattern",Qt::Key_Minus);


	engine.key_input.add_key_bind("global/raise_octave","Raise Octave",Qt::Key_Asterisk);

	engine.key_input.add_key_bind("global/lower_octave","Lower Octave",Qt::Key_Slash);

	engine.key_input.add_key_bind("note_entry/clear_field","Clear Field",Qt::Key_Period);

	engine.key_input.add_key_bind("note_entry/note_off","Note Off",Qt::Key_QuoteLeft);
	engine.key_input.add_key_bind("note_entry/toggle_note_edit","Note Edit Mode Toggle",Qt::Key_Space);
	for (int i=0;i<MAX_KEYS;i++)
		engine.key_input.add_key_bind((String)"note_entry/"+key_name[i],key_name[i],default_pianokey_bind[i]);



}

};