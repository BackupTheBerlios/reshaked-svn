//
// C++ Implementation: input_dialog
//
// Description: 
//
//
// Author: Juan Linietsky <reshaked@gmail.com>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "input_dialog.h"
#include "containers/center_container.h"
#include "widgets/button.h"
#include "widgets/line_edit.h"

namespace GUI {

	
void NumericInputDialog::show(String p_question,double p_min,double p_max,double p_step,double p_default) {
	
	sb->get_range()->set_min(p_min);
	sb->get_range()->set_max(p_max);
	sb->get_range()->set_step(p_step);
	sb->get_range()->set(p_default);
	Window::show();
}

void NumericInputDialog::ok_pressed_text(String) {
	
	ok_pressed();
}
void NumericInputDialog::ok_pressed() {
	
	entered_number_signal.call( sb->get_range()->get() );
	hide();
}
NumericInputDialog::NumericInputDialog(Window *p_parent) : Window(p_parent,MODE_POPUP,SIZE_CENTER) {
	
	mg = new MarginGroup("Enter a Number:");
	set_root_frame( mg );
	sb = mg->add( new SpinBox );
	sb->get_line_edit()->text_enter_signal.connect( this, &NumericInputDialog::ok_pressed_text);
	
	mg->add( new CenterContainer)->set( new Button("Ok")) ->pressed_signal.connect( this, &NumericInputDialog::ok_pressed );
}


}
